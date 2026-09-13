# Contention et dérive temporelle avec 200 philosophes

## Problème

Le test extrême suivant échoue presque immédiatement :

```sh
./philo 200 121 60 60 500
```

`121 ms` correspond à la durée idéale d'un cycle pair (`2 * time_to_eat + 1`).
L'objectif est de tenir environ une minute sans décès, pas de masquer le problème en
augmentant largement `time_to_die`.

## Observations

Le démarrage pair/impair semble maintenant correct :

- les 100 philosophes pairs commencent leur premier repas entre 0 et 1 ms ;
- les 100 philosophes impairs commencent entre 60 et 61 ms ;
- au cycle suivant, 24 pairs remangent à 120 ms et 72 à 121 ms ;
- les 4 autres n'obtiennent pas leurs fourchettes avant le passage du moniteur.

Exemple observé : le philosophe 58 mange à 0 ms, ses voisins 57 et 59 mangent à
60 ms, puis l'un d'eux ne libère ses fourchettes qu'à 121 ms. Le moniteur obtient
le mutex avant le philosophe 58 et le déclare mort à 121 ms.

Il ne s'agit donc plus principalement d'un problème de barrière de départ, ni
d'une dérive arithmétique de l'horloge. C'est une latence de réveil et de
synchronisation sur une fenêtre d'une milliseconde.

## Cause principale envisagée

Un seul mutex protège actuellement les fourchettes, `clock.stop`, `last_meal` et
une partie du travail du moniteur. Il est sollicité en permanence :

- `sleep_until()` vérifie `stop` toutes les 200 us pour chaque philosophe ;
- les philosophes sans fourchettes réessaient également toutes les 200 us ;
- le moniteur utilise le même mutex pour détecter les morts.

Avec 200 threads, cela peut représenter plus d'un million de prises de mutex par
seconde, en plus des réveils groupés. Le mutex global n'est pas nécessairement
lent en lui-même : c'est le couple **mutex multifonction + polling intensif** qui
crée la contention. Les logs synchrones peuvent aussi retarder le retour vers la
prise de fourchettes.

## Solutions à évaluer

1. **Séparer les responsabilités (solution privilégiée si autorisée)**
   Utiliser un mutex par fourchette, une protection dédiée par `last_meal`, et un
   état `stop` atomique. Les philosophes éloignés pourraient alors progresser en
   parallèle et la lecture de `stop` ne bloquerait plus les fourchettes.

2. **Remplacer le polling par des attentes bloquantes**
   Employer des condition variables ou un ordonnanceur central qui réveille les
   philosophes lorsqu'une ressource change. Un mutex global peut rester efficace
   si les threads dorment au lieu de le solliciter continuellement.

3. **Solution compatible avec une API très restreinte**
   Si atomiques et conditions sont interdites, séparer au minimum le mutex de
   `stop` des fourchettes, réduire la fréquence des vérifications et utiliser un
   backoff calculé selon les échéances des voisins. Toute fourchette acquise doit
   être libérée proprement lors de l'arrêt.

4. **Réduire le travail sur le chemin critique**
   Regrouper les écritures de logs, utiliser des échéances absolues et une horloge
   monotone si disponible. Ces optimisations sont complémentaires, mais ne
   remplacent pas la correction de la contention.

## Points à décider

- Quelles primitives sont autorisées par le sujet (`stdatomic`, conditions,
  `clock_gettime`, `pthread_mutex_trylock`) ?
- Faut-il conserver la réservation atomique des deux fourchettes ou revenir à de
  vrais mutex de fourchettes avec un ordre de prise empêchant le deadlock ?
- Comment garantir qu'aucun log ne soit produit après un décès sans remettre
  tous les threads derrière le même verrou chaud ?
- Le test à `121 ms` doit rester un test de performance extrême : augmenter sa
  fenêtre corrigerait le symptôme, pas la contention observée.
