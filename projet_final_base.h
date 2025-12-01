#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 100

typedef struct {
  int coef[TAILLE_MAX];
  int taille;
  } polynome;

void initialiser_polynome(void);
void afficher_polynome (polynome *A);
void somme_polynomes(polynome *A, polynome *B);
void produits_polynomes(polynome *A, polynome *B);
