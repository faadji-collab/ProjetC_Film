#include <stdio.h>
#include <stdlib.h>
#include "film.h"

int main(void) {
    Catalogue cat;
    init_catalogue(&cat);

    /* --- Entrée : Film* -> Catalogue --- */
    int nbFilms;
    Film *films = chargerFilms(NOM_FICHIER, &nbFilms);
    cat.tab = films;
    cat.n = nbFilms;
    cat.alloc = nbFilms;

    printf("%d film(s) chargé(s).\n", cat.n);

    /* ... menu : ajouter_film(&cat), supprimer_film(&cat, id), etc.
       toutes ces fonctions travaillent directement sur cat.tab / cat.n ... */

    /* --- Sortie : Catalogue -> sauvegarde (à chaque modification) --- */
    sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);

    clear_catalogue(&cat);
    return 0;
}