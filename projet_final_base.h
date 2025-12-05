#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 100

typedef struct {
  int coef[TAILLE_MAX];
  int taille;
  } polynome;

/* ==========Fichier Journal (LOG)========== */
void ouvrir_log(void);
void logger_operation(const char *operation, const char *details);
void fermer_log(void);

/*==========Manipulation des polynômes==========*/
polynome initialiser_polynome(void);
void afficher_polynome (polynome *A);
float evaluation_polynome(polynome *A, float a);

/* ==========Création de fichiers polynômes========== */
void creer_fichier_polynome(const char *filename);
void sauvegarder_polynome(polynome *A, const char *filename);

/*==========Enregistrement des polynômes==========*/
polynome lire_polynome_clavier(void);
polynome lire_polynome_fichier(const char *filename);
polynome initialiser_polynome_avec_choix(void);

/* ==========Somme de 2 polynômes========== */
polynome *somme_polynomes(polynome *A, polynome *B);

/* ==========Produits de 2 polynômes========== */
polynome *produits_polynomes(polynome *A, polynome *B);

/* ==========Dérivation d'un polynôme========== */
polynome *derivee_polynome(polynome *A);
polynome *derivee_ordre_n(polynome *A, int n);

/* ==========Intégrale d'un polynôme sur un segment========== */
float integrale_polynome(polynome *A, float a, float b);

/* ==========Développement limité d'un polynôme========== */
int fact(int n);
void developpement_limite(polynome *A, int n, int a);

/* ==========Recherche racine d'un polynôme========== */
float racine_interv(polynome *A, float a, float b, float eps, int Nmax);

