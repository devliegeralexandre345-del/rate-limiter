# Rate Limiter

Implémentation d'un limiteur de débit (Rate Limiter) en C++20 avec les algorithmes **Token Bucket** et **Sliding Window**.

## Fonctionnalités

- Support des deux algorithmes principaux : Token Bucket et Sliding Window
- Interface thread-safe
- Réinitialisation possible
- Dépendances uniquement sur STL C++20

## Algorithmes

### Token Bucket

L'algorithme Token Bucket permet de réguler le flux de requêtes en utilisant un "seau" de jetons. Chaque requête consomme un jeton, et les jetons sont remplis périodiquement.

### Sliding Window

L'algorithme Sliding Window limite le nombre de requêtes dans une fenêtre glissante de temps fixe.

## Utilisation

```cpp
#include "rate_limiter.hpp"

using namespace rate_limiter;

// Création d'un Rate Limiter avec Token Bucket (5 requêtes par seconde)
RateLimiter limiter(Algorithm::TOKEN_BUCKET, 5);

if (limiter.allow()) {
    // Traitement de la requête
} else {
    // Requête refusée
}
```

## Interface CLI

Exemple d'exécution :

```bash
$ ./main
Rate Limiter Demo
==================
Token Bucket (5 req/sec):
  Request 1 allowed
  Request 2 allowed
  Request 3 allowed
  Request 4 denied
  Request 5 denied
  Request 6 denied
  Request 7 denied
  Request 8 denied

Sliding Window (3 req/2sec):
  Request 1 allowed
  Request 2 allowed
  Request 3 denied
  Request 4 denied
  Request 5 denied
  Request 6 denied
```

## Tests Unitaires

Les tests unitaires vérifient :
- La fonctionnalité de base des deux algorithmes
- Le comportement de réinitialisation
- Le déterminisme des résultats
- Les différences entre les algorithmes

```bash
$ g++ -std=c++20 tests/test.cpp -o test && ./test
All tests passed!
```

## Architecture

Le projet est structuré en 4 fichiers :

1. `include/rate_limiter.hpp` : Header-only avec la logique principale
2. `src/main.cpp` : Interface CLI de démonstration
3. `tests/test.cpp` : Tests unitaires
4. `README.md` : Documentation