#include <stdio.h>
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
    printf("0. Quitter\n");
    printf("Choix : ");
}

int main(void) {
    Catalogue cat;

    if (!charger_catalogue(&cat, NOM_FICHIER)) {
        fprintf(stderr, "Erreur lors du chargement du catalogue.\n");
        return 1;
    }

    printf("%d film(s) charge(s) depuis %s.\n", cat.n, NOM_FICHIER);

    int choix = -1;
    do {
        afficher_menu();
        if (scanf("%d", &choix) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            choix = -1;
            continue;
        }

        switch (choix) {
            case 1:
                /* TODO : ajouter_film(&cat); puis sauvegarder_catalogue(&cat, NOM_FICHIER); */
                printf("[TODO] Ajouter un film\n");
                break;
            case 2:
                /* TODO : afficher_films(&cat); */
                printf("[TODO] Afficher tous les films\n");
                break;
            case 3:
                /* TODO : rechercher_par_id(&cat, id); */
                printf("[TODO] Rechercher par identifiant\n");
                break;
            case 4:
                /* TODO : rechercher_par_titre(&cat, motif); */
                printf("[TODO] Rechercher par titre\n");
                break;
            case 5:
                /* TODO : afficher_par_genre(&cat, genre); */
                printf("[TODO] Afficher par genre\n");
                break;
            case 6:
                /* TODO : modifier_film(&cat, id); puis sauvegarder_catalogue(&cat, NOM_FICHIER); */
                printf("[TODO] Modifier un film\n");
                break;
            case 7:
                /* TODO : supprimer_film(&cat, id); puis sauvegarder_catalogue(&cat, NOM_FICHIER); */
                printf("[TODO] Supprimer un film\n");
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);

    clear_catalogue(&cat);
    return 0;
}