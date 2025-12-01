#include "projet_final_base.h"

void initialiser_polynome(void) {
  int degre;
  printf("Indiquez le degré de votre polynome : ");
  scanf("%d", degre);
  
  polynome A;
  A->taille = degre;
  for (int i = degre - 1; i >= 0; i--) {
    printf("Indiquez le coefficient de degré %d : ", i);
    scanf("%d", A->coef[i]);
  }
}

void afficher_polynome (polynome *A) {
  for (int i = A->taille - 1; i > 0; i--) {
    printf("%dX**%d +", A->coef[i], i);
  }
  printf("%d\n", A->coef[0]);
}

void somme_polynomes(polynome *A, polynome *B) {
  polynome C;
  if (A->taille < B->taille) {
    C->taille = B->taille
    for (int i = A->taille - 1; i >= 0; i--) {
      C->coef[i] = A->coef[i] + B->coef[i];
    } 
    for (int i = A->taille; i < B->taille; i++) {
      C->coef[i] = B->coef[i];
    }
  } else {
  if (A->taille > B->taille) {
    C->taille = A->taille
    for (int i = B->taille - 1; i >= 0; i--) {
      C->coef[i] = A->coef[i] + B->coef[i];
    } 
    for (int i = B->taille; i < A->taille; i++) {
      C->coef[i] = A->coef[i];
    }
  } else {
    int degre = A->taille;
    while (A->coef[degre] + B->coef[degre] == 0) do {
      degre--;
    }
    if (degre == 0) {
      printf("0\n");
    } else {
      for (int i = degre - 1; i >= 0; i--) {
        C->coef = A->coef[i] + B->coef[i];
      }
    }
  }}
}

void produits_polynomes(polynome *A, polynome *B) {
  polynome C;
  C->taille = A->taille + B->taille;
  for (int i = C->taille; i>0; i--) {
    C->coef[i] = 0;
  }
  for (int i = A->taille - 1; i>=0; i--) {
    for (int j = B->taille - 1; j>=0; j--) {
      C->coef[i+j] = C->coef[i+j] + A->coef[i]*B->coef[j];
    }
  }
}

void derivee_polynome(polynome *A) {
  polynome C;
  for (int i = A->taille - 2; i>=0; i--) {
    C->coef[i] = (i+1)*(A->coef[i+1]);
  }
}
