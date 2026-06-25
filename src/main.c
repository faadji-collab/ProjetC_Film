#include <stdio.h>
#include <stdlib.h>
#include "film.h"
 
static void afficher_menu(void) {
    printf("\n===== CATALOGUE DE FILMS =====\n");
    printf("1. Ajouter un film\n");
    printf("2. Afficher tous les films\n");
    printf("3. Rechercher un film par identifiant\n");
    printf("4. Rechercher un film par titre\n");
    printf("5. Afficher les films d'un genre donne\n");
    printf("6. Modifier un film\n");
    printf("7. Supprimer un film\n");
    printf("8. Trier par note decroissante\n");
    printf("9. Trier par ordre alphabetique\n");
    printf("10. Top 5 des films\n");
    printf("11. Statistiques\n");
    printf("0. Quitter\n");
    printf("Choix : ");
}
 
/* Vide le buffer d'entree jusqu'au prochain saut de ligne. */
static void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
 
int main(void) {
    Catalogue cat;
    init_catalogue(&cat);
 
    /* ---Film* + int -> Catalogue --- */
    int nbFilms;
    Film *films = chargerFilms(NOM_FICHIER, &nbFilms);
    cat.tab = films;
    cat.n = nbFilms;
    cat.alloc = nbFilms;
 
    printf("%d film(s) charge(s) depuis %s.\n", cat.n, NOM_FICHIER);
 
    int choix = -1;
    int id;
    char texte[100];
 
    do {
        afficher_menu();
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            choix = -1;
            continue;
        }
        vider_buffer();
 
        switch (choix) {
            case 1:
                ajouter_film(&cat);
                sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);
                break;
 
            case 2:
                afficher_films(&cat);
                break;
 
            case 3:
                printf("Identifiant a rechercher : ");
                scanf("%d", &id);
                vider_buffer();
                rechercher_par_id(&cat, id);
                break;
 
            case 4:
                printf("Titre (ou partie du titre) a rechercher : ");
                scanf(" %99[^\n]", texte);
                rechercher_par_titre(&cat, texte);
                break;
 
            case 5:
                printf("Genre a rechercher : ");
                scanf(" %99[^\n]", texte);
                afficher_par_genre(&cat, texte);
                break;
 
            case 6:
                printf("Identifiant du film a modifier : ");
                scanf("%d", &id);
                vider_buffer();
                modifier_film(&cat, id);
                sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);
                break;
 
            case 7:
                printf("Identifiant du film a supprimer : ");
                scanf("%d", &id);
                vider_buffer();
                supprimer_film(&cat, id);
                sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);
                break;
 
            case 8:
                trier_par_note(&cat);
                break;
 
            case 9:
                trier_par_titre(&cat);
                break;
 
            case 10:
                top5_films(&cat);
                break;
 
            case 11:
                afficher_statistiques(&cat);
                break;
 
            case 0:
                printf("Au revoir !\n");
                break;
 
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
 
    /* --- Sauvegarde finale de securite --- */
    sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);
 
    clear_catalogue(&cat);
    return 0;
}
 