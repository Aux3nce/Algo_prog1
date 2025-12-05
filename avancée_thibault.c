#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TAILLE_MAX 100

typedef struct {
int coef[TAILLE_MAX];
int taille;
} polynome;

polynome initialiser_polynome(void) {
  int degre;
  printf("Indiquez le degré de votre polynome : ");
  scanf("%d", &degre);
 
  polynome A;
  A.taille = degre + 1;
  for (int i = degre; i >= 0; i--) {
    printf("Indiquez le coefficient de degré %d : ", i);
    scanf("%d", &A.coef[i]);
  }
  return A;
}

void afficher_polynome (polynome *A) {
  if (A == NULL) return;
 
  int affiche_qqchose = 0;
 
  for (int i = A->taille - 1; i>=0; i--) {
    int coef = A->coef[i];
   
    if (coef != 0) {
      if (affiche_qqchose) {
        printf(" %c ", (coef > 0) ? '+' : '-');
        if (coef < 0) coef = -coef;
    }
    else if (coef < 0) {
      printf("-");
      coef = -coef;
    }
   
    if (coef != 1 || i == 0) {
      printf("%d", coef);
    }
   
    if (i > 0) {
      printf("X");
      if (i > 1) printf("^%d", i);
    }
    affiche_qqchose = 1;
    }
  }
  if (!affiche_qqchose) {
    printf("0");
  }
  printf("\n");
}

float evaluation_polynome(float a, polynome *A) {
int i;
float S=0;
float puiss_a=1;
for (i = 0; i<=A->taille-1;i--) {
S+=A->coef[A->taille-1-i]*puiss_a;
puiss_a=puiss_a*a;
}
return S;
}

polynome *somme_polynomes(polynome *A, polynome *B) {
  polynome *C = malloc(sizeof(polynome)); //Allocation mémoire
  if (!C) return NULL; //Vérification d'allocation
 
 
  if (A->taille < B->taille) {
    C->taille = B->taille;
    for (int i = A->taille - 1; i >= 0; i--) {
      C->coef[i] = A->coef[i] + B->coef[i];
    }
    for (int i = A->taille; i < B->taille; i++) {
      C->coef[i] = B->coef[i];
    }
  } else {
  if (A->taille > B->taille) {
    C->taille = A->taille;
    for (int i = B->taille - 1; i >= 0; i--) {
      C->coef[i] = A->coef[i] + B->coef[i];
    }
    for (int i = B->taille; i < A->taille; i++) {
      C->coef[i] = A->coef[i];
    }
  } else {
    int degre = A->taille;
    while (A->coef[degre] + B->coef[degre] == 0) {
      degre--;
    }
    if (degre == 0) {
      printf("0\n");
      C->taille = 1; // Polynôme nul
    } else {
      for (int i = degre - 1; i >= 0; i--) {
        C->coef[i] = A->coef[i] + B->coef[i];
      }
    }
  }}
  return C;
}





polynome *produits_polynomes(polynome *A, polynome *B) {
  polynome *C = malloc(sizeof(polynome)); // Allocation mémoire
  if (!C) return NULL;
 
  C->taille = A->taille + B->taille -1;
  for (int i = C->taille; i>0; i--) {
    C->coef[i] = 0;
  }
  for (int i = A->taille - 1; i>=0; i--) {
    for (int j = B->taille - 1; j>=0; j--) {
      C->coef[i+j] = C->coef[i+j] + A->coef[i]*B->coef[j];
    }
  }
  return C;
}

polynome *derivee_polynome(polynome *A) {
  polynome *C = malloc(sizeof(polynome)); //Allocation mémoire
  if (!C) return NULL;
  C->taille = A->taille - 1;
    for (int i = 1; i<= A->taille - 1; i++) {
      C->coef[i-1] = i*(A->coef[i]);
    }
  return C;
}

polynome *derivee_ordre_n(polynome *A, int n) {
polynome *C = malloc(sizeof(polynome)); //Allocation mémoire
  if (!C) return NULL;
  if (n == 0) {
  return A;
  }
 
  else {
  polynome *temporaire = A;
  int k;
  for (k = 0; k<n; k++) {
  C = derivee_polynome(temporaire);
  free (temporaire);
  temporaire = C;
  }
  }
  return C;
}

float integrale_polynome(polynome *A, int a, int b) {
int i;
float S=0;
int puiss_a=a;
int puiss_b=b;
for (i = 0; i<=A->taille-1;i++) {
S+=(A->coef[A->taille-1-i]/(i+1))*(puiss_b - puiss_a);
puiss_a=puiss_a*a;
puiss_b=puiss_b*b;
}
return S;  
}
int fact(int n) {
if (n == 0 || n == 1) {
return 1;
}
else {
int P = 1;
int k;
for (k=1; k<=n; k++){
P = P*k;
}
return P;
}
}

polynome *developpement_limite(polynome *A, int a) {
polynome *C = malloc(sizeof(polynome)); //Allocation mémoire
if (!C) return NULL;
printf("A quel ordre voulez-vous faire votre DL : ");
int n;
scanf("%d",&n);
C->taille = n+1;
int i;
for(i = 0; i<=n; i++) {
polynome *derivee = derivee_ordre_n(A,i);
int val = evaluation_polynome(a, derivee);
C->coef[i] = val/fact(i);
free(derivee);
}
return C;
}

void afficher_DL_polynome(polynome *A, int a) {
  if (A == NULL) return;
 
  int affiche_qqchose = 0;
 
  for (int i = A->taille - 1; i>=0; i--) {
    int coef = A->coef[i];
   
    if (coef != 0) {
      if (affiche_qqchose) {
        printf(" %c ", (coef > 0) ? '+' : '-');
        if (coef < 0) coef = -coef;
    }
    else if (coef < 0) {
      printf("-");
      coef = -coef;
    }
   
    if (coef != 1 || i == 0) {
      printf("%d", coef);
    }
   
    if (i > 0) {
      printf("(X-%d)", a);
      if (i > 1) printf("^%d", i);
    }
    affiche_qqchose = 1;
    }
  }
  if (!affiche_qqchose) {
    printf("0");
  }
  printf("\n");
}

int main(void) {

  int choix;
  polynome A;
  polynome B;
  polynome *C = NULL;
 
  do {
  printf("\n\nQue souhaitez-vous faire ?\n");
  printf("\n==========MENU==========\n\n");
  printf("1. Somme de deux polynômes\n");
  printf("2. Produit de deux polynômes\n");
  printf("3. Dérivée d'un polynôme\n");
  printf("4. Intégrale d'un polynôme sur un intervalle\n");
  printf("5. Développement limité d'un polynôme autour d'un point\n");
  printf("6. Racine d'un polynôme sur un intervalle\n");
  printf("7. Quitter\n\n");
  printf("=========================\n\n");
  printf("Votre choix : ");
  scanf("%d", &choix);

 
    switch(choix) {
      case 1 :
        A = initialiser_polynome();
        B = initialiser_polynome();
        C = somme_polynomes(&A, &B);
        afficher_polynome(C);
        break;
      case 2:
        A = initialiser_polynome();
        B = initialiser_polynome();
        C = produits_polynomes(&A, &B);
        afficher_polynome(C);
        break;
      case 3 :
        A = initialiser_polynome();
        C = derivee_polynome(&A);
        afficher_polynome(C);
        break;
      case 4 :
      A = initialiser_polynome();
printf("lintégrale du polynome sur cet intervalle vaut : %f", integrale_polynome(&A));
        break;
      case 5 :
      printf("En quel point voulez-vous faire votre DL : ");
float a;
scanf("%f ",&a);
A = initialiser_polynome();
C = developpement_limite(&A,a);
afficher_DL_polynome(C,a);
        break;
      case 6 :

        break;
      case 7 :
     
        break;
      default :
        printf("Choix invalide, veuillez recommencer.\n");
        break;
    }
  }
  while (choix != 7);
 
  if (C != NULL) {
    free(C);
  }
 
  return 0;
}
