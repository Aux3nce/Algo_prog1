#include "projet_final_base.h"

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
