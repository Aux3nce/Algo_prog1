#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    } else if (coef < 0) {
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
    polynome *C = malloc(sizeof(polynome));
    if (!C) return NULL;
    if (A->taille <= 1) {
        C->taille = 1;
        C->coef[0] = 0;
    } else {
        C->taille = A->taille - 1;
        for (int i = 0; i < C->taille; i++) {
            C->coef[i] = A->coef[i+1] * (i+1);
        }
    }
    return C;
}

/* ==========Correction de la fonction derivee_ordre_n ========== */

polynome *derivee_ordre_n(polynome *A, int n) {
    if (!A || n < 0) return NULL;
    
    // Si n=0, retourner une copie du polynôme original
    if (n == 0) {
        polynome *C = malloc(sizeof(polynome));
        if (!C) return NULL;
        C->taille = A->taille;
        for (int i = 0; i < A->taille; i++) {
            C->coef[i] = A->coef[i];
        }
        return C;
    }
    
    polynome *resultat = malloc(sizeof(polynome));
    if (!resultat) return NULL;
    
    // Copier A dans resultat
    resultat->taille = A->taille;
    for (int i = 0; i < A->taille; i++) {
        resultat->coef[i] = A->coef[i];
    }
    
    // Appliquer la dérivation n fois
    for (int ordre = 0; ordre < n; ordre++) {
        polynome *temp = resultat;
        resultat = derivee_polynome_auxence(temp); // Utiliser la version correcte
        
        if (ordre > 0) { // Ne pas free le premier (qui est A)
            free(temp);
        }
        
        if (!resultat) {
            if (ordre > 0) free(temp);
            return NULL;
        }
    }
    
    return resultat;
}

/* ==========Correction de evaluation_polynome ========== */

float evaluation_polynome(float a, polynome *A) {
    if (!A) return 0;
    
    float S = 0;
    float puiss_a = 1; // a^0
    
    // Parcourir des coefficients de degré 0 à degré max
    for (int i = 0; i < A->taille; i++) {
        S += A->coef[i] * puiss_a;
        puiss_a = puiss_a * a;
    }
    return S;
}

/* ==========Correction de developpement_limite ========== */

polynome *developpement_limite(polynome *A, float a) {
    if (!A) return NULL;
    
    printf("A quel ordre voulez-vous faire votre DL : ");
    int n;
    scanf("%d", &n); // Retirer l'espace après %d
    
    if (n < 0) {
        printf("L'ordre doit être positif\n");
        return NULL;
    }
    
    polynome *C = malloc(sizeof(polynome));
    if (!C) return NULL;
    
    // Initialiser C avec la bonne taille et coefficients à 0
    C->taille = n + 1; // Degré n signifie taille n+1
    for (int i = 0; i < C->taille; i++) {
        C->coef[i] = 0;
    }
    
    // Calculer les coefficients du DL
    for (int i = 0; i <= n; i++) {
        polynome *derivee = derivee_ordre_n(A, i);
        if (!derivee) {
            free(C);
            return NULL;
        }
        
        // Coefficient du DL : f^{(i)}(a) / i!
        float valeur_derivee = evaluation_polynome(a, derivee);
        float coeff = valeur_derivee / fact(i);
        
        // Stocker dans le coefficient de degré i
        C->coef[i] = coeff;
        
        free(derivee);
    }
    
    return C;
}

/* ==========Fonction factorielle (version améliorée) ========== */

int fact(int n) {
    if (n < 0) return 1; // Pas de factorielle négative
    int resultat = 1;
    for (int k = 1; k <= n; k++) {
        // Vérification de dépassement (optionnel)
        if (resultat > INT_MAX / k) {
            printf("Attention : factorielle trop grande pour n=%d\n", n);
            return INT_MAX;
        }
        resultat *= k;
    }
    return resultat;
}

polynome *integrale_polynome(polynome *A) {
printf("Sélectionnez un intervalle d'intégration : ");
float a;
float b;
scanf("[%f,%f]\n",&a,&b);
int i;
float S=0;
float puiss_a=a;
float puiss_b=b;
for (i = 0, i<=A->taille-1,i++) {
S+=(A->coef[A->taille-1-i]/(i+1))*(puiss_b - puiss_a)
puiss_a=puiss_a*a;
puiss_b=puiss_b*b;
}
return S;  
}
int fact(n) {
int P = 1;
for (k=1, k<=n, k++){
P = P*k;
}
return P;
}

polynome *developpement_limite(polynome *A) {
polynome *C = malloc(sizeof(polynome)); //Allocation mémoire
if (!C) return NULL;
printf("En quel point voulez-vous faire votre DL : ");
float a;
scanf("%f ",&a);
printf("A quel ordre voulez-vous faire votre DL : ");
int n;
scanf("%f ",&n);
int i;
for(i = 0; i<=n, i++) {
C->coef[i] = evaluation_polynome(a,derivee_polynome(A))
}

}

float racine_intervalle(polynome *A, float a, float b, float eps, int Nmax) {
    // Vérifier les conditions de Bolzano
    float fa = application(A, a);
    float fb = application(A, b);
    
    if (fa * fb > 0) {
        printf("Attention : f(a) et f(b) ont meme signe, pas de garantie de racine dans [%f, %f]\n", a, b);
        logger_operation("Newton", "Intervalle sans changement de signe");
    }
    
    // Choisir le meilleur point initial
    float x0;
    if (fabsf(fa) < fabsf(fb)) {
        x0 = a;
    } else {
        x0 = b;
    }
    
    // Appliquer Newton
    polynome *derivee = derivee_polynome(A);
    float x = x0;
    
    char log_msg[200];
    snprintf(log_msg, sizeof(log_msg), "Intervalle [%f,%f], x0=%f, eps=%e, Nmax=%d", 
             a, b, x0, eps, Nmax);
    logger_operation("Newton demarre", log_msg);
    
    for (int n = 0; n < Nmax; n++) {
        float fx = application(A, x);
        float fpx = application(B, x);
        
        // Condition d'arrêt
        if (fabsf(fx) < eps) {
            snprintf(log_msg, sizeof(log_msg), "Convergence en %d iterations, x=%f, f(x)=%e", 
                     n+1, x, fx);
            logger_operation("Newton reussi", log_msg);
            free(derivee);
            return x;
        }
        
        // Éviter division par zéro
        if (fabsf(fpx) < 1e-15) {
            logger_operation("Newton erreur", "Derivee nulle");
            free(derivee);
            return x;
        }
        
        // Itération de Newton
        float x_new = x - fx / fpx;
        
        // Vérifier si on reste dans l'intervalle
        if (x_new < a || x_new > b) {
            printf("Newton sort de l'intervalle, utilisation de la bissection\n");
            x_new = (a + b) / 2.0;
        }
        
        x = x_new;
    }
    
    logger_operation("Newton", "Maximum d'iterations atteint");
    free(derivee);
    return x;
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

        break;
      case 5 :

        break;
      case 6 :

        break;
      case 7 :
      
        break;
      default :
        printf("Choix invalide, veuillez recommencer.\n");
        break;
    }
  } while (choix != 7);
  
  if (C != NULL) {
    free(C);
  }
  
  return 0;
}
