#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "film.h"

/*
 * Constructeur / destructeur */

void init_catalogue(Catalogue *c) {
    c->alloc = 0;
    c->n = 0;
    c->tab = NULL;
}

void clear_catalogue(Catalogue *c) {
    free(c->tab);   
    c->tab = NULL;
    c->alloc = 0;
    c->n = 0;
}

/** Sauvegarde du catalogue (réécriture complète du fichier)*/

int sauvegarder_catalogue(const Catalogue *c, const char *nomFichier) {
    FILE *f = fopen(nomFichier, "w"); 
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'écrire dans %s\n", nomFichier);
        return 0;
    }

    for (int i = 0; i < c->n; i++) {
        fprintf(f, "%d;%s;%s;%d;%.1f\n",
                c->tab[i].id,
                c->tab[i].titre,
                c->tab[i].genre,
                c->tab[i].annee,
                c->tab[i].note);
    }

    fclose(f);
    return 1;
}



/* void ajouter_film(Catalogue *c) { ... } */
/* void afficher_films(const Catalogue *c) { ... } */
/* void rechercher_par_id(const Catalogue *c, int id) { ... } */
/* void rechercher_par_titre(const Catalogue *c, const char *motif) { ... } */
/* void afficher_par_genre(const Catalogue *c, const char *genre) { ... } */
/* void modifier_film(Catalogue *c, int id) { ... } */
/* void supprimer_film(Catalogue *c, int id) { ... } */