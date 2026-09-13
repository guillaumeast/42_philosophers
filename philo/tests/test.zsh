#!/usr/bin/env zsh

setopt pipe_fail
unsetopt bg_nice

readonly SCRIPT_DIR=${0:A:h}
readonly SCRIPT_NAME=${0:t}
readonly PROJECT_DIR=${SCRIPT_DIR:h}
readonly TEST_TMP_BASE=${TMPDIR:-/tmp}
readonly LOGS_DIR=$SCRIPT_DIR/logs
typeset -ir LEAK_TIMEOUT_FACTOR=4
typeset -ir LEAK_SURVIVAL_MEALS=10

typeset -i LEAK_MODE=0
typeset -i BINARY_SET=0
PHILO_BIN=$PROJECT_DIR/philo

usage()
{
	print -r -- "Usage: $SCRIPT_NAME [--leak] [philo_binary]"
}

while (( $# > 0 ))
do
	case "$1" in
		--leak)
			LEAK_MODE=1
			;;
		--help|-h)
			usage
			exit 0
			;;
		--)
			shift
			if (( $# != 1 || BINARY_SET != 0 ))
			then
				usage >&2
				exit 2
			fi
			PHILO_BIN=$1
			BINARY_SET=1
			;;
		-*)
			print -ru2 -- "Unknown option: $1"
			usage >&2
			exit 2
			;;
		*)
			if (( BINARY_SET != 0 ))
			then
				print -ru2 -- "Only one philo binary may be specified."
				usage >&2
				exit 2
			fi
			PHILO_BIN=$1
			BINARY_SET=1
			;;
	esac
	shift
done

readonly PHILO_BIN
TEST_TMP_DIR=$(mktemp -d "$TEST_TMP_BASE/philo-tests.XXXXXX") || exit 1

typeset -i TEST_COUNT=0
typeset -i PASS_COUNT=0
typeset -i FAIL_COUNT=0
typeset -i RUN_EXIT_CODE=0
typeset -i RUN_HAS_VALGRIND_ERRORS=0
RUN_OUTPUT=""
RUN_ERROR=""
RUN_VALGRIND_REPORT=""
RUN_TEST_DIR=""

readonly GREEN=$'\033[32m'
readonly RED=$'\033[31m'
readonly RESET=$'\033[0m'

cleanup()
{
	if [[ -n "$TEST_TMP_DIR" && -d "$TEST_TMP_DIR" ]]
	then
		rm -rf -- "$TEST_TMP_DIR"
	fi
}

trap cleanup EXIT INT TERM

pass_test()
{
	PASS_COUNT+=1
	print -r -- "${GREEN}✔${RESET} $1"
}

fail_test()
{
	FAIL_COUNT+=1
	print -ru2 -- "${RED}✖${RESET} $1: $2"
}

test_directory_name()
{
	local label=$1
	local slug

	slug=$(print -r -- "$label" | LC_ALL=C sed \
		-e 's/[^A-Za-z0-9._-]/_/g' \
		-e 's/__*/_/g' \
		-e 's/^_//' \
		-e 's/_$//')
	[[ -n "$slug" ]] || slug=test
	printf 'test_%02d_%s' "$TEST_COUNT" "$slug"
}

run_program()
{
	local label=$1
	local limit=$2
	local timeout_file
	local process_id
	local watcher_id

	shift 2
	TEST_COUNT+=1
	RUN_TEST_DIR="$SUITE_LOG_DIR/$(test_directory_name "$label")"
	mkdir -p -- "$RUN_TEST_DIR" || exit 1
	RUN_OUTPUT="$RUN_TEST_DIR/stdout.log"
	RUN_ERROR="$RUN_TEST_DIR/stderr.log"
	RUN_VALGRIND_REPORT="$RUN_TEST_DIR/valgrind.log"
	timeout_file="$TEST_TMP_DIR/$TEST_COUNT.timeout"
	: >"$RUN_OUTPUT"
	: >"$RUN_ERROR"
	: >"$RUN_VALGRIND_REPORT"
	if (( LEAK_MODE != 0 ))
	then
		valgrind \
			--leak-check=full \
			--show-leak-kinds=all \
			--errors-for-leak-kinds=all \
			--fair-sched=yes \
			--error-exitcode=125 \
			--log-file="$RUN_VALGRIND_REPORT" \
			-- "$PHILO_BIN" "$@" >"$RUN_OUTPUT" 2>"$RUN_ERROR" &
	else
		"$PHILO_BIN" "$@" >"$RUN_OUTPUT" 2>"$RUN_ERROR" &
	fi
	process_id=$!
	(sleep "$limit"; if kill -0 "$process_id" 2>/dev/null; then
		print -r -- timeout >"$timeout_file"
		kill -TERM "$process_id" 2>/dev/null
	fi) &
	watcher_id=$!
	wait "$process_id"
	RUN_EXIT_CODE=$?
	kill -TERM "$watcher_id" 2>/dev/null
	wait "$watcher_id" 2>/dev/null
	if [[ -f "$timeout_file" ]]
	then
		RUN_EXIT_CODE=124
	fi
	RUN_HAS_VALGRIND_ERRORS=0
	if (( LEAK_MODE != 0 && RUN_EXIT_CODE != 124 )) \
		&& { (( RUN_EXIT_CODE == 125 )) || grep -Eq \
		'ERROR SUMMARY: [1-9][0-9]* errors' "$RUN_VALGRIND_REPORT"; }
	then
		RUN_HAS_VALGRIND_ERRORS=1
	fi
}

fail_on_valgrind_errors()
{
	local label=$1

	if (( RUN_HAS_VALGRIND_ERRORS == 0 ))
	then
		return 1
	fi
	fail_test "$label" "Valgrind detected an error (see $RUN_VALGRIND_REPORT)"
	return 0
}

format_error()
{
	local philo_count=$1

	awk -v count="$philo_count" '
	BEGIN { dead = 0; bad = 0 }
	$0 !~ /^[0-9]+ [1-9][0-9]* (has taken a fork|is eating|is sleeping|is thinking|died)$/ {
		print "invalid line " NR ": " $0; bad = 1
	}
	$2 < 1 || $2 > count {
		print "invalid philosopher on line " NR ": " $2; bad = 1
	}
	dead == 1 {
		print "message after death on line " NR; bad = 1
	}
	$3 == "died" { dead = 1 }
	END {
		if (NR == 0) { print "empty output"; bad = 1 }
		exit bad
	}' "$RUN_OUTPUT"
}

death_error()
{
	local time_to_die=$1

	awk -v ttl="$time_to_die" '
	$3 == "is" && $4 == "eating" { last_meal[$2] = $1 }
	$3 == "died" {
		deaths++
		meal = (($2 in last_meal) ? last_meal[$2] : 0)
		delay = $1 - meal - ttl
		if (delay < 0 || delay > 10) {
			print "death delay is " delay " ms for philosopher " $2
			bad = 1
		}
	}
	END {
		if (deaths != 1) { print "expected one death, got " deaths; bad = 1 }
		exit bad
	}' "$RUN_OUTPUT"
}

meal_error()
{
	local philo_count=$1
	local meal_count=$2

	awk -v count="$philo_count" -v expected="$meal_count" '
	$3 == "is" && $4 == "eating" { meals[$2]++ }
	$3 == "died" { death_line = NR }
	END {
		if (death_line != 0) {
			print "unexpected death on line " death_line
			exit 1
		}
		for (i = 1; i <= count; i++) {
			if (meals[i] < expected) {
				print "philosopher " i " ate " meals[i] "/" expected " times"
				bad = 1
			}
		}
		exit bad
	}' "$RUN_OUTPUT"
}

test_death()
{
	local label=$1
	local limit=$2
	local time_to_die=$3
	local philo_count=$4
	local details

	shift 4
	run_program "$label" "$limit" "$@"
	if fail_on_valgrind_errors "$label"
	then
		return
	fi
	if (( RUN_EXIT_CODE != 0 ))
	then
		fail_test "$label" "exit status $RUN_EXIT_CODE"
		return
	fi
	details=$(format_error "$philo_count")
	if [[ $? -ne 0 ]]
	then
		fail_test "$label" "$details"
		return
	fi
	details=$(death_error "$time_to_die")
	if [[ $? -ne 0 ]]
	then
		fail_test "$label" "$details"
		return
	fi
	pass_test "$label"
}

test_meals()
{
	local label=$1
	local -i limit=$2
	local philo_count=$3
	local meal_count=$4
	local details

	shift 4
	if (( LEAK_MODE != 0 ))
	then
		limit=$((limit * LEAK_TIMEOUT_FACTOR))
	fi
	run_program "$label" "$limit" "$@"
	if fail_on_valgrind_errors "$label"
	then
		return
	fi
	if (( RUN_EXIT_CODE != 0 ))
	then
		fail_test "$label" "exit status $RUN_EXIT_CODE"
		return
	fi
	details=$(format_error "$philo_count")
	if [[ $? -ne 0 ]]
	then
		fail_test "$label" "$details"
		return
	fi
	details=$(meal_error "$philo_count" "$meal_count")
	if [[ $? -ne 0 ]]
	then
		fail_test "$label" "$details"
		return
	fi
	pass_test "$label"
}

test_survival()
{
	local label=$1
	local duration=$2

	shift 2
	if (( LEAK_MODE != 0 ))
	then
		test_meals "$label" "$duration" "$1" "$LEAK_SURVIVAL_MEALS" \
			"$@" "$LEAK_SURVIVAL_MEALS"
		return
	fi
	run_program "$label" "$duration" "$@"
	if fail_on_valgrind_errors "$label"
	then
		return
	fi
	if (( RUN_EXIT_CODE == 124 ))
	then
		pass_test "$label"
	else
		fail_test "$label" "simulation stopped early with status $RUN_EXIT_CODE"
	fi
}

test_invalid()
{
	local label=$1

	shift
	run_program "$label" 2 "$@"
	if fail_on_valgrind_errors "$label"
	then
		return
	fi
	if (( RUN_EXIT_CODE == 124 ))
	then
		fail_test "$label" "program did not stop"
	elif (( RUN_EXIT_CODE == 0 ))
	then
		fail_test "$label" "invalid arguments were accepted"
	elif [[ ! -s "$RUN_OUTPUT" && ! -s "$RUN_ERROR" ]]
	then
		fail_test "$label" "no error message"
	else
		pass_test "$label"
	fi
}

test_zero_meals()
{
	local label="edge: zero required meals"

	run_program "$label" 2 200 800 200 200 0
	if fail_on_valgrind_errors "$label"
	then
		return
	fi
	if (( RUN_EXIT_CODE != 0 ))
	then
		fail_test "$label" "exit status $RUN_EXIT_CODE"
	elif [[ -s "$RUN_OUTPUT" ]]
	then
		fail_test "$label" "unexpected output"
	else
		pass_test "$label"
	fi
}

test_custom_logs()
{
	local label="custom logs smoke test"

	run_program "$label" 5 -l 2 800 200 200 1
	if fail_on_valgrind_errors "$label"
	then
		return
	fi
	if (( RUN_EXIT_CODE != 0 ))
	then
		fail_test "$label" "exit status $RUN_EXIT_CODE"
	elif ! grep -Fq $'\033[' "$RUN_OUTPUT"
	then
		fail_test "$label" "ANSI colors are missing"
	elif ! grep -Fq "=>" "$RUN_OUTPUT"
	then
		fail_test "$label" "meal summary is missing"
	else
		pass_test "$label"
	fi
}

if [[ ! -x "$PHILO_BIN" ]]
then
	print -ru2 -- "Executable not found: $PHILO_BIN"
	print -ru2 -- "Run 'make -C $PROJECT_DIR' or pass its path as the first argument."
	exit 1
fi

if (( LEAK_MODE != 0 )) && ! command -v valgrind >/dev/null 2>&1
then
	print -ru2 -- "Valgrind is required when --leak is used."
	exit 1
fi

mkdir -p -- "$LOGS_DIR" || exit 1
typeset -i suite_number=1
while ! mkdir -- "$LOGS_DIR/suite_$suite_number" 2>/dev/null
do
	suite_number+=1
done
readonly SUITE_LOG_DIR="$LOGS_DIR/suite_$suite_number"

print -r -- "Testing $PHILO_BIN"
print -r -- "Logs: $SUITE_LOG_DIR"
if (( LEAK_MODE != 0 ))
then
	print -r -- "Valgrind leak detection is enabled."
	print -r -- "Survival tests use $LEAK_SURVIVAL_MEALS meals for a clean exit."
fi
print -r -- "Timestamps are validated but are not required to be globally ordered."

test_death "grid: 1 800 200 200" 3 800 1 1 800 200 200
test_survival "grid: 5 800 200 200 survives" 3 5 800 200 200
test_meals "grid: 5 800 200 200 7" 10 5 7 5 800 200 200 7
test_survival "grid: 4 410 200 200 survives" 3 4 410 200 200
test_death "grid: 4 310 200 100" 3 310 4 4 310 200 100
test_death "grid: 2 philosophers, death at 190 ms" 3 190 2 2 190 100 100
test_death "grid: 2 philosophers, death at 310 ms" 3 310 2 2 310 200 100

test_zero_meals
test_death "edge: minimum evaluation timings" 2 60 1 1 60 60 60
test_meals "stress: 199 philosophers" 15 199 2 199 10000 60 60 2
test_meals "stress: 200 philosophers" 15 200 3 200 10000 60 60 3
test_meals "ultimate: 200 philosophers, 2*eat+1 ms, 500 meals" \
	120 200 500 200 121 60 60 500

test_invalid "invalid: missing arguments"
test_invalid "invalid: zero philosophers" 0 800 200 200
test_invalid "invalid: negative philosophers" -1 800 200 200
test_invalid "invalid: non-numeric value" 4 nope 200 200
test_invalid "invalid: millisecond overflow" 4 9223372036854775808 200 200
test_invalid "invalid: too many arguments" 4 800 200 200 7 extra

test_custom_logs

print
print -r -- "$PASS_COUNT passed, $FAIL_COUNT failed"
if (( FAIL_COUNT != 0 ))
then
	exit 1
fi
exit 0
