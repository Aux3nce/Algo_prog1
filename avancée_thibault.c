#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
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

float evaluation_polynome(int a, polynome *A) {
    float S = 0;
    for (int i = A->taille - 1; i >= 0; i--) {  // Du plus haut au plus bas
        S = S * a + A->coef[i];
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
    polynome *C = malloc(sizeof(polynome)); // Allocation mémoire pour C
    if (!C) return NULL; // Vérification d'allocation mémoire
    polynome *temporaire = A;

    for (int k = 0; k < n; k++) {
        C = derivee_polynome(temporaire); // Calcul de la dérivée
        // temporaire ne doit pas être libéré ici, car nous devons le garder pour les futures dérivées.
        temporaire = C; // Met temporaire à jour avec la nouvelle dérivée
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


/*polynome *developpement_limite(polynome *A, int a) {
    polynome *C = malloc(sizeof(polynome)); // Allocation mémoire pour C
    if (!C) return NULL; // Vérification d'allocation mémoire

    printf("A quel ordre voulez-vous faire votre DL : ");
    int n;
    scanf("%d", &n);

    // Initialisation des coefficients de C à 0
    for (int i = 0; i <= n; i++) {
        C->coef[i] = 0;
    }

    for (int i = 0; i <= n; i++) {
        // Calcul de la dérivée d'ordre i
        polynome *derivee = derivee_ordre_n(A, i);

        // Evaluation de la dérivée en a
        int val = evaluation_polynome(a, derivee);

        // Calcul du terme du développement limité : val / i!
        C->coef[i] = val / fact(i);

        free(derivee); // Libération de la mémoire allouée pour cette dérivée
    }

    return C; // Retourner le développement limité
}*/



void developpement_limite(polynome *A, int n, float a) {
    float f;
    f = evaluation_polynome(a, A); // On calcule A(a)
    printf("Le développement de Taylor de p est :\n");
    printf("%.0f", f); // On affiche A(a)

    // Pour chaque dérivée jusqu'à l'ordre n
    polynome *temporaire = malloc(sizeof(polynome));  // Allouer mémoire pour le polynôme temporaire
    if (!temporaire) {
        printf("Erreur d'allocation mémoire\n");
        return;
    }
    *temporaire = *A;  // Copier A dans temporaire

    for (int i = 1; i <= n; i++) {
        // Calcul de la dérivée
        polynome *deriv = derivee_polynome(temporaire);
        float t = evaluation_polynome(a, deriv); // Évaluation de la dérivée en a
        int m = fact(i); // Calcul de i!
float x =t/m;
        // Affichage du terme du développement limité
        if (a < 0) {
    printf(" + %.0f*((X + %.0f)^%d)", x, fabs(a), i);
}
else if (a == 0) {
    printf(" + %.0f*(X^%d)", x, i);
}
else {
    printf(" + %.0f*((X - %.0f)^%d)", x, a, i);  // X - a, pas X + a
}
        *temporaire = *deriv;  // D'abord copier
free(deriv);  
    }

    free(temporaire); // Libérer la mémoire du polynôme temporaire
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
      printf("Sélectionnez la borne inférieure : ");
int a;
scanf("%d", &a);
printf("Sélectionnez la borne supérieure : ");
int b;
scanf("%d", &b);
      A = initialiser_polynome();
printf("intégrale du polynome sur cet intervalle vaut : %f", integrale_polynome(&A, a, b));
        break;
      case 5 :
      int u;
      printf("En quel point voulez-vous faire votre DL : ");
scanf("%d",&u);
int n;
      printf("A quel ordre voulez-vous faire votre DL : ");
scanf("%d",&n);
A = initialiser_polynome();
developpement_limite(&A,n,u);
//afficher_DL_polynome(C,u);
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
