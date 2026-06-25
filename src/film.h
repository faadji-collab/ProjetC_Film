#ifndef FILM_H
#define FILM_H

#define NOM_FICHIER "../data/films.csv"

typedef struct {
    int id;
    char titre[100];
    char genre[50];
    int annee;
    float note;
} Film;

typedef struct {
    int alloc;
    int n;
    Film *tab;
} Catalogue;

Film *chargerFilms(char *nomFichier, int *nbFilms);
int sauvegarderFilms(const Film *tableau, int nbFilms, const char *nomFichier);

/*Gestion en mémoire (format Catalogue) - responsable : moi */
void init_catalogue(Catalogue *c);
void clear_catalogue(Catalogue *c);

void ajouter_film(Catalogue *c);
void afficher_films(const Catalogue *c);
void rechercher_par_id(const Catalogue *c, int id);
void rechercher_par_titre(const Catalogue *c, const char *motif);
void afficher_par_genre(const Catalogue *c, const char *genre);
void modifier_film(Catalogue *c, int id);
void supprimer_film(Catalogue *c, int id);
 
/* Fonctionnalités bonus */
void trier_par_note(Catalogue *c);
void trier_par_titre(Catalogue *c);
void top5_films(const Catalogue *c);
void afficher_statistiques(const Catalogue *c);
 
#endif