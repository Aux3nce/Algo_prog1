#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ==========Définition de notre structure========== */

#define TAILLE_MAX 100

typedef struct {
  float coef[TAILLE_MAX];
  int taille;
} polynome;

/* ==========Fichier Journal (LOG)========== */
FILE *log_file = NULL;

void ouvrir_log(void) {
    log_file = fopen("operations.log", "a");
    if (log_file) {
        time_t now;
        time(&now);
        fprintf(log_file, "\n=== Session du %s ===\n", ctime(&now));
    } else {
        printf("Attention : impossible d'ouvrir le fichier log\n");
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

void fermer_log(void) {
    if (log_file) {
        fprintf(log_file, "=== Fin de session ===\n\n");
        fclose(log_file);
    }
}

/*==========Manipulation des polynômes==========*/

polynome initialiser_polynome(void) {
  int degre;
  printf("Indiquez le degre de votre polynome : ");
  scanf("%d", &degre);
  
  polynome A;
  A.taille = degre + 1;
  for (int i = degre; i >= 0; i--) {
    printf("Indiquez le coefficient de degre %d : ", i);
    scanf("%f", &A.coef[i]);
  }
  
  // Logguer la création du polynôme
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Polynome degre %d initialisé", degre);
  logger_operation("Initialisation", log_msg);
  
  return A;
}

void afficher_polynome (polynome *A) {
  if (A == NULL) return;
  
  int affiche_qqchose = 0;
  
  for (int i = A->taille - 1; i>=0; i--) {
    float coef = A->coef[i];  // Changé de int à float
    
    if (coef != 0) {
      if (affiche_qqchose) {
        printf(" %c ", (coef > 0) ? '+' : '-');
        if (coef < 0) coef = -coef;
      } else if (coef < 0) {
        printf("-");
        coef = -coef;
      }
    
      if (fabs(coef) != 1.0 || i == 0) {
        printf("%.2f", coef);  // Affichage avec 2 décimales
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

float evaluation_polynome(polynome *A, float a) {
  float S = 0.0;
  float puiss_a = 1.0;
  
  for (int i = 0; i < A->taille; i++) {
    S += A->coef[i] * puiss_a;
    puiss_a *= a;
  }
  
  // Logguer l'évaluation
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Evaluation en %.2f = %.6f", a, S);
  logger_operation("Evaluation", log_msg);
  
  return S;
}
/* ==========Création de fichiers polynômes========== */

void creer_fichier_polynome(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erreur : impossible de creer le fichier %s\n", filename);
        return;
    }
    
    int degre;
    printf("Degre du polynome a sauvegarder : ");
    scanf("%d", &degre);
    
    // Écrire le degré
    fprintf(file, "%d\n", degre);
    
    // Demander et écrire les coefficients
    for (int i = degre-1; i <= 0; i--) {
        float coef;
        printf("Coefficient de degre %d : ", i);
        scanf("%f", &coef);
        fprintf(file, "%.6f\n", coef);
    }
    
    fclose(file);
    
    char log_msg[200];
    snprintf(log_msg, sizeof(log_msg), "Fichier %s cree - degre %d", filename, degre);
    logger_operation("Creation fichier", log_msg);
    
    printf("Polynome sauvegarde dans %s\n", filename);
}

void sauvegarder_polynome(polynome *A, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erreur : impossible d'ecrire dans %s\n", filename);
        return;
    }
    
    // Écrire le degré (taille-1)
    fprintf(file, "%d\n", A->taille - 1);
    
    // Écrire les coefficients du degré 0 au degré N
    for (int i = 0; i < A->taille; i++) {
        fprintf(file, "%.3f\n", A->coef[i]);
    }
    
    fclose(file);
    
    char log_msg[200];
    snprintf(log_msg, sizeof(log_msg), "Polynome sauvegarde dans %s", filename);
    logger_operation("Sauvegarde", log_msg);
    
    printf("Polynome sauvegarde avec succes dans %s\n", filename);
}
/*==========Enregistrement des polynômes==========*/
polynome lire_polynome_clavier(void) {
    polynome A = initialiser_polynome();
    logger_operation("Lecture", "Polynome saisi au clavier");
    return A;
}

polynome lire_polynome_fichier(const char *filename) {
    polynome A;
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        A.taille = 1;
        A.coef[0] = 0;
        
        char log_msg[200];
        snprintf(log_msg, sizeof(log_msg), "Echec ouverture fichier %s", filename);
        logger_operation("ERREUR", log_msg);
        
        return A;
    }
    
    // Format attendu : "degre coef0 coef1 ... coefN"
    int degre;
    fscanf(file, "%d", &degre);
    A.taille = degre + 1;
    
    for (int i = 0; i < A.taille; i++) {
        fscanf(file, "%f", &A.coef[i]);  // Changé de %d à %f
    }
    
    fclose(file);
    
    char log_msg[200];
    snprintf(log_msg, sizeof(log_msg), "Fichier %s lu - degre %d", filename, degre);
    logger_operation("Lecture fichier", log_msg);
    
    return A;
}

// Menu de choix d'entrée des polynômes
polynome initialiser_polynome_avec_choix(void) {
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
  polynome *C = malloc(sizeof(polynome));
  if (!C) {
    logger_operation("ERREUR", "Echec allocation memoire dans somme_polynomes");
    return NULL;
  }
  
  int taille_max = (A->taille > B->taille) ? A->taille : B->taille;
  C->taille = taille_max;
  
  // Initialiser tous les coefficients à 0
  for (int i = 0; i < C->taille; i++) {
    C->coef[i] = 0.0;
  }
  
  // Additionner les coefficients
  for (int i = 0; i < A->taille; i++) {
    C->coef[i] += A->coef[i];
  }
  
  for (int i = 0; i < B->taille; i++) {
    C->coef[i] += B->coef[i];
  }
  
  // Réduire la taille si les coefficients les plus élevés sont nuls
  while (C->taille > 1 && fabs(C->coef[C->taille-1]) < 1e-10) {
    C->taille--;
  }
  
  // Logguer l'opération
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Somme effectuee - resultat degre %d", C->taille-1);
  logger_operation("Somme", log_msg);
  
  return C;
}

/* ==========Produits de 2 polynômes========== */

polynome *produits_polynomes(polynome *A, polynome *B) {
  polynome *C = malloc(sizeof(polynome));
  if (!C) {
    logger_operation("ERREUR", "Echec allocation memoire dans produits_polynomes");
    return NULL;
  }
  
  C->taille = A->taille + B->taille - 1;
  
  // Initialiser tous les coefficients à 0
  for (int i = 0; i < C->taille; i++) {
    C->coef[i] = 0.0;
  }
  
  // Calculer le produit
  for (int i = 0; i < A->taille; i++) {
    for (int j = 0; j < B->taille; j++) {
      C->coef[i+j] += A->coef[i] * B->coef[j];
    }
  }
  
  // Logguer l'opération
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Produit effectue - resultat degre %d", C->taille-1);
  logger_operation("Produit", log_msg);
  
  return C;
}

/* ==========Dérivation d'un polynôme========== */

polynome *derivee_polynome(polynome *A) {
  polynome *C = malloc(sizeof(polynome));
  if (!C) {
    logger_operation("ERREUR", "Echec allocation memoire dans derivee_polynome");
    return NULL;
  }
  
  if (A->taille <= 1) {
    C->taille = 1;
    C->coef[0] = 0.0;
  } else {
    C->taille = A->taille - 1;
    for (int i = 0; i < C->taille; i++) {
      C->coef[i] = A->coef[i+1] * (i+1);
    }
  }
  
  // Logguer l'opération
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Derivee calculee - degre %d -> %d", 
           A->taille-1, C->taille-1);
  logger_operation("Derivation", log_msg);
  
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

  // Logguer l'opération
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Derivee d'ordre %d calculee", n);
  logger_operation("Derivation multiple", log_msg);
  
  return C;
}

/* ==========Intégrale d'un polynôme sur un segment========== */

float integrale_polynome(polynome *A, float a, float b) {
  if (a > b) {
    float temp = a;
    a = b;
    b = temp;
  }
  
  float S = 0.0;
  float puissance_a = a;
  float puissance_b = b;
  
  // Intégrale de ∑ a_i x^i = ∑ a_i/(i+1) x^(i+1)
  for (int i = 0; i < A->taille; i++) {
    float terme = A->coef[i] / (float)(i + 1);
    S += terme * (puissance_b - puissance_a);
    puissance_a *= a;
    puissance_b *= b;
  }
  
  // Logguer l'opération
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Integrale de %.2f a %.2f = %.6f", a, b, S);
  logger_operation("Integration", log_msg);
  
  return S;
}

/* ==========Développement limité d'un polynôme========== */

int fact(int n) {
  if (n == 0 || n == 1) {
    return 1;
  } else {
    int P = 1;
    for (int k = 1; k <= n; k++) {
      P = P * k;
    }
    return P;
  }
}

void developpement_limite(polynome *A, int n, int a) {
    float f;
    f = evaluation_polynome(A, a); // On calcule A(a)
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
        float t = evaluation_polynome(deriv, a); // Évaluation de la dérivée en a
        int m = fact(i); // Calcul de i!
        float x =t/m;
        if (t == 0) {
            printf(" ");
        }
        else {
            // Affichage du terme du développement limité
            if (a < 0) {
            printf(" + %.0f*((X + %.0f)^%d)", x, fabs(a), i);
            }
            else if (a == 0) {
                 printf(" + %.0f*(X^%d)", x, i);
            }
            else {
                 printf(" + %.0f*((X - %d)^%d)", x, a, i);  // X - a, pas X + a
            }
        }
        *temporaire = *deriv;  // D'abord copier
        free(deriv);  
    }

    free(temporaire); // Libérer la mémoire du polynôme temporaire
    printf("\n");

    
    // Logguer l'opération
    char log_msg[200];
    snprintf(log_msg, sizeof(log_msg), "Développement limité en %d d'un polynome de degre : %d", a, A->taille - 1);
    logger_operation("Développement limité", log_msg);
}

/* ==========Recherche racine d'un polynôme========== */

float racine_interv(polynome *A, float a, float b, float eps, int Nmax) {
  // Logguer le début de la recherche
  char log_msg[200];
  snprintf(log_msg, sizeof(log_msg), "Recherche racine dans [%.2f, %.2f], eps=%.1e, Nmax=%d", 
           a, b, eps, Nmax);
  logger_operation("Newton debut", log_msg);
  
  // Vérifier les signes aux bornes
  float fa = evaluation_polynome(A, a);
  float fb = evaluation_polynome(A, b);
  
  if (fa * fb > 0) {
    snprintf(log_msg, sizeof(log_msg), 
             "f(%.2f)=%.2e et f(%.2f)=%.2e de meme signe", a, fa, b, fb);
    logger_operation("Newton attention", log_msg);
    printf("Attention : f(a) et f(b) ont meme signe, pas de garantie de racine\n");
  }
  
  // Choisir le meilleur point initial
  float x0 = (fabs(fa) < fabs(fb)) ? a : b;
  polynome *deriv = derivee_polynome(A);
  
  if (!deriv) {
    logger_operation("ERREUR", "Echec calcul derivee dans Newton");
    return (a + b) / 2.0;
  }
  
  float x = x0;
  int converged = 0;
  
  for (int n = 0; n < Nmax; n++) {
    float fx = evaluation_polynome(A, x);
    float fpx = evaluation_polynome(deriv, x);
    
    // Condition de convergence
    if (fabs(fx) < eps) {
      snprintf(log_msg, sizeof(log_msg), 
               "Convergence apres %d iterations : x=%.6f, f(x)=%.2e", n+1, x, fx);
      logger_operation("Newton succes", log_msg);
      converged = 1;
      break;
    }
    
    // Éviter division par zéro
    if (fabs(fpx) < 1e-15) {
      snprintf(log_msg, sizeof(log_msg), "Derivee nulle en x=%.6f", x);
      logger_operation("Newton erreur", log_msg);
      break;
    }
    
    // Itération de Newton
    float x_new = x - fx / fpx;
    
    // Vérifier si on reste dans l'intervalle
    if (x_new < a || x_new > b) {
      x_new = (a + b) / 2.0;
    }
    
    x = x_new;
  }
  
  free(deriv);
  
  if (!converged) {
    snprintf(log_msg, sizeof(log_msg), "Non convergence apres %d iterations", Nmax);
    logger_operation("Newton echec", log_msg);
  }
  
  return x;
}

/* ==========Affichage du menu========== */

int main(void) {
  // Ouvrir le fichier log au début
  ouvrir_log();
  logger_operation("Programme", "Demarrage");
  
  int choix;
  polynome A, B;
  polynome *C = NULL;
  
  do {
    printf("\n\nQue souhaitez-vous faire ?\n");
    printf("\n==========MENU==========\n\n");
    printf("1. Somme de deux polynomes\n");
    printf("2. Produit de deux polynomes\n");
    printf("3. Derivee d'un polynome\n");
    printf("4. Integrale d'un polynome sur un intervalle\n");
    printf("5. Developpement limite d'un polynome autour d'un point\n");
    printf("6. Racine d'un polynome sur un intervalle\n");
    printf("7. Enregistrer un nouveau polynome\n");
    printf("8. Quitter\n\n");
    printf("=========================\n\n");
    printf("Votre choix : ");
    scanf("%d", &choix);
    
    // Logguer le choix de l'utilisateur
    char log_msg[50];
    snprintf(log_msg, sizeof(log_msg), "Choix menu : %d", choix);
    logger_operation("Menu", log_msg);
    
    switch(choix) {
      case 1: {
        printf("\n--- Premier polynome ---\n");
        A = initialiser_polynome_avec_choix();
        printf("\n--- Deuxieme polynome ---\n");
        B = initialiser_polynome_avec_choix();
        
        C = somme_polynomes(&A, &B);
        if (C) {
          printf("\nResultat de la somme : ");
          afficher_polynome(C);
          free(C);
        }
        break;
      }
      
      case 2: {
        printf("\n--- Premier polynome ---\n");
        A = initialiser_polynome_avec_choix();
        printf("\n--- Deuxieme polynome ---\n");
        B = initialiser_polynome_avec_choix();
        
        C = produits_polynomes(&A, &B);
        if (C) {
          printf("\nResultat du produit : ");
          afficher_polynome(C);
          free(C);
        }
        break;
      }
      
      case 3: {
        A = initialiser_polynome_avec_choix();
        C = derivee_polynome(&A);
        if (C) {
          printf("\nDerivee du polynome : ");
          afficher_polynome(C);
          free(C);
        }
        break;
      }
      
      case 4: {
        A = initialiser_polynome_avec_choix();
        float a, b;
        printf("Intervalle d'integration [a, b] : ");
        scanf("%f %f", &a, &b);
        
        float integrale = integrale_polynome(&A, a, b);
        printf("\nIntegrale de ");
        afficher_polynome(&A);
        printf("sur [%.2f, %.2f] = %.6f\n", a, b, integrale);
        break;
      }
      
      case 5: {
        int u;
        printf("En quel point voulez-vous faire votre DL : ");
        scanf("%d",&u);
        int n;
        printf("A quel ordre voulez-vous faire votre DL : ");
        scanf("%d",&n);
        A = initialiser_polynome();
        developpement_limite(&A,n,u);
        break;
      }
      
      case 6: {
        A = initialiser_polynome_avec_choix();
        float a, b, eps;
        int Nmax;
        printf("Intervalle de recherche [a, b] : ");
        scanf("%f %f", &a, &b);
        printf("Tolerance (ex: 1e-6) : ");
        scanf("%f", &eps);
        printf("Nombre maximum d'iterations : ");
        scanf("%d", &Nmax);
        
        float racine = racine_interv(&A, a, b, eps, Nmax);
        float valeur = evaluation_polynome(&A, racine);
        
        printf("\nRacine approximee : %.6f\n", racine);
        printf("f(%.6f) = %.2e\n", racine, valeur);
        
        if (fabs(valeur) < eps) {
          printf("Convergence validee (|f(x)| < %.1e)\n", eps);
        } else {
          printf("Attention : convergence limitee (|f(x)| = %.1e)\n", fabs(valeur));
        }
        break;
      }
      case 7:
        char filename[100];
        printf("Nom du fichier pour sauvegarder (ex: mon_polynome.txt) : ");
        scanf("%s", filename);
        creer_fichier_polynome(filename);
        A = initialiser_polynome();
        sauvegarder_polynome(&A, filename);
        break;
      case 8:
        printf("\nProgramme termine. Operations enregistrees dans operations.log\n");
        logger_operation("Programme", "Fin normale");
        break;
        
      default:
        printf("\nChoix invalide, veuillez recommencer.\n");
        logger_operation("ERREUR", "Choix menu invalide");
        break;
    }
    
  } while (choix != 8);
  
  // Fermer le fichier log
  fermer_log();
  
  return 0;
}
