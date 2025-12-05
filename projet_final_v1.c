#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>  // Ajouter en haut

/* ==========Fichier Journal (LOG)========== */
FILE *log_file = NULL;

void ouvrir_log() {
    log_file = fopen("operations.log", "a");  // "a" pour append (ajouter)
    if (log_file) {
        time_t now;
        time(&now);
        fprintf(log_file, "\n=== Session du %s ===\n", ctime(&now));
    }
}

void logger_operation(const char *operation, const char *details) {
    if (log_file) {
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(log_file, "[%02d:%02d:%02d] %s - %s\n", 
                local->tm_hour, local->tm_min, local->tm_sec,
                operation, details);
        fflush(log_file);  // Écrire immédiatement
    }
}

void fermer_log() {
    if (log_file) {
        fprintf(log_file, "=== Fin de session ===\n\n");
        fclose(log_file);
    }
}

/* ==========Définition de notre structure========== */

#define TAILLE_MAX 100

typedef struct {
  float coef[TAILLE_MAX];
  int taille;
  } polynome;

/*==========Manipulation des polynômes==========*/

polynome initialiser_polynome(void) {
  int degre;
  printf("Indiquez le degré de votre polynome : ");
  scanf("%d", &degre);
  
  polynome A;
  A.taille = degre + 1;
  for (int i = degre; i >= 0; i--) {
    printf("Indiquez le coefficient de degré %d : ", i);
    scanf("%f", &A.coef[i]);
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

float application(polynome *A, float x) {
  float y = 0.0; // Image que l'on renvoie
  float puissance = 1.0; // On initialise à x^0
  
  for (int i = 0; i < A->taille; i++) {
    y = y + A->coef[i] * puissance;
    puissance = puissance * x; // On incrémente la puissance pour créer x^(i+1)
  }
  return y;
}
/*==========Enregistrement des polynômes==========*/
polynome lire_polynome_clavier() {
    return initialiser_polynome();
}

polynome lire_polynome_fichier(const char *filename) {
    polynome A;
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        A.taille = 1;
        A.coef[0] = 0;
        return A;
    }
    
    // Format attendu : "degre coef0 coef1 ... coefN"
    int degre;
    fscanf(file, "%d", &degre);
    A.taille = degre + 1;
    
    for (int i = 0; i < A.taille; i++) {
        fscanf(file, "%d", &A.coef[i]);
    }
    
    fclose(file);
    logger_operation("Lecture polynome fichier", filename);
    return A;
}

// Menu de choix d'entrée
polynome initialiser_polynome_avec_choix() {
    printf("\nMode d'entree du polynome :\n");
    printf("1. Saisie clavier\n");
    printf("2. Fichier texte\n");
    printf("Choix : ");
    
    int choix;
    scanf("%d", &choix);
    
    if (choix == 2) {
        char filename[100];
        printf("Nom du fichier : ");
        scanf("%s", filename);
        return lire_polynome_fichier(filename);
    } else {
        return lire_polynome_clavier();
    }
}

/* ==========Somme de 2 polynômes========== */

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

/* ==========Produits de 2 polynômes========== */

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

/* ==========Dérivation d'un polynôme========== */

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
/* ==========Recherche racine d'un polynôme========== */

float application(polynome *A, float x) {
  float y = 0.0; // Image que l'on renvoie
  float puissance = 1.0; // On initialise à x^0
  
  for (int i = 0; i < A->taille; i++) {
    y = y + A->coef[i] * puissance;
    puissance = puissance * x; // On incrémente la puissance pour créer x^(i+1)
  }
  return y;
}

float racine_x0(polynome *A, float x0, float eps, int Nmax) {
  polynome *B = derivee_polynome(A);
  if (!B) {
    printf("Erreur d'allocation mémoire\n");
    return x0;
  }
  
  float x = x0; // La potentielle racine
  float x_prev = x0; // Mémoire de x
  int convergence = 0; // Booléen pour tester la convergence
  
  for (int n = 0; n < Nmax;  n++) {
    float f_x = application(A,x);
    float f_deriv_x = application(B,x);
    
    // Tester la convergence
    if (fabs(f_x) < eps) {
      convergence = 1;
      break;
    }
    
    // Vérifier si la dérivée est nulle pour éviter la division par 0
    if ( f_deriv_x < 1e-10) {
      printf("Erreur : dérivée nulle");
    } else {
    
      // Mise à jour et sauvegarde de x
      x_prev = x;
      x = x - f_x/f_deriv_x;
      
    } if (fabsf(f_x/f_deriv_x) < eps || fabsf(x - x_prev) < eps) {
      convergence = 1;
      break;
    }
    
    if (fabsf(f_x/f_deriv_x) > 10.0) {
      printf("Pas trop grand, reduction\n");
      x = x_prev - f_x/f_deriv_x;
    }
  }
  free(B);
  
  if (!convergence) {
    printf("Nombre maximum d'itérations atteint (%d)\n", Nmax);
  }
  
  return x;
}

float racine_interv(polynome *A, float a, float b, float eps, int Nmax) {
    // Vérifier que la racine existe
    float fa = application(A, a);
    float fb = application(A, b);
    
    if (fa * fb > 0) {
        printf(" f(a) et f(b) ont meme signe, pas de garantie de racine dans [%f, %f]\n", a, b);
    }
    
    // Choisir le meilleur point initial
    float x0;
    if (fabsf(fa) < fabsf(fb)) {
        x0 = a;
    } else {
        x0 = b;
    }
    
    // Appliquer Newton
    polynome *deriv = derivee_polynome(A);
    float x = x0;
        
    for (int n = 0; n < Nmax; n++) {
        float fx = application(A, x);
        float fpx = application(deriv, x);
        
        // Condition d'arrêt
        if (fabsf(fx) < eps) {
            free(deriv);
            return x;
        }
        
        // Éviter division par zéro
        if (fabsf(fpx) < 1e-15) {
            free(deriv);
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
    free(deriv);
    return x;
}

/* ==========Affichage du menu========== */

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
        C = integrale_polynome(&A);
        afficher_polynome(C);
        break;
      case 5 :
        A = initialiser_polynome();
        float a;
        printf("Sélectionner le point en lequel vous souhaitez faire le DL :");
        scanf("%f", a);
        C = developpement_limite(&A, a);
        afficher_polynome(C);
        break;
      case 6 :
        A = initialiser_polynome();
        float a;
        float b;
        float eps;
        int Nmax;
        printf("Sélectionnez les bornes de l'intervalle, puis la tolérance et enfin le nombre d'itérations maximal\n");
        scanf("%f%f%f%d", &a, &b, &eps, &Nmax);
        
        float racine_result = racine_interv(&A, a, b, eps, Nmax);
        printf("Racine trouvée : %f\n", racine_result);
        printf("Valeur du polynome en ce point : %f\n", application(&A, racine_result));
        break;
      case 7 :
        printf("Polynômes sauvegardés. Au revoir !\n");
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
