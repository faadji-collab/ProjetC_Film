#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "film.h"

#define RESET   "\033[0m"
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define GRAS    "\033[1m"
 
/* SUPPRESSION DU MOT CLÉ STATIC POUR LE PARTAGE AVEC FILM.C */
void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
 
static void afficher_menu(void) {
    printf(BLEU GRAS "\n╔═══════════════════════════════════════════════╗\n" RESET);
    printf(BLEU GRAS "║        🎬  CATALOGUE PERSONNEL DE FILMS  🎬    ║\n" RESET);
    printf(BLEU GRAS "╠═══════════════════════════════════════════════╣\n" RESET);
    printf(BLEU "║  " RESET CYAN "1." RESET "  Ajouter un film                          " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET CYAN "2." RESET "  Afficher tous les films                  " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET CYAN "3." RESET "  Rechercher par identifiant               " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET CYAN "4." RESET "  Rechercher par titre                     " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET CYAN "5." RESET "  Afficher les films d'un genre            " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET CYAN "6." RESET "  Modifier un film                         " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET CYAN "7." RESET "  Supprimer un film                        " BLEU "║\n" RESET);
    printf(BLEU GRAS "╠═══════════════════════════════════════════════╣\n" RESET);
    printf(BLEU "║  " RESET JAUNE "8." RESET "  Trier par note décroissante              " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET JAUNE "9." RESET "  Trier par ordre alphabétique            " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET JAUNE "10." RESET " Top 5 des films                          " BLEU "║\n" RESET);
    printf(BLEU "║  " RESET JAUNE "11." RESET " Statistiques                             " BLEU "║\n" RESET);
    printf(BLEU GRAS "╠═══════════════════════════════════════════════╣\n" RESET);
    printf(BLEU "║  " RESET ROUGE "0." RESET "  Quitter                                  " BLEU "║\n" RESET);
    printf(BLEU GRAS "╚═══════════════════════════════════════════════╝\n" RESET);
    printf(CYAN "  Votre choix : " RESET);
}

int main(void) {
    Catalogue cat;
    init_catalogue(&cat);
 
    int nbFilms = 0;
    Film *films = chargerFilms(NOM_FICHIER, &nbFilms);
    if (films != NULL) {
        cat.tab   = films;
        cat.n     = nbFilms;
        cat.alloc = nbFilms;
    }
 
    printf(VERT GRAS "\n✅ %d film(s) chargé(s) depuis %s.\n" RESET, cat.n, NOM_FICHIER);
 
    int choix = -1;
    int id;
    char texte[100];
 
    do {
        afficher_menu();
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            choix = -1;
            printf(ROUGE "  ⚠️  Entrée invalide, veuillez réessayer.\n" RESET);
            continue;
        }
        vider_buffer();
 
        switch (choix) {
            case 1:
                ajouter_film(&cat);
                break;
 
            case 2:
                afficher_films(&cat);
                break;
 
            case 3:
                printf(CYAN "  Identifiant à rechercher : " RESET);
                scanf("%d", &id);
                vider_buffer();
                rechercher_par_id(&cat, id);
                break;
 
            case 4:
                printf(CYAN "  Titre (ou partie) à rechercher : " RESET);
                fgets(texte, sizeof(texte), stdin);
                texte[strcspn(texte, "\n")] = '\0';
                rechercher_par_titre(&cat, texte);
                break;
 
            case 5:
                printf(CYAN "  Genre à rechercher : " RESET);
                fgets(texte, sizeof(texte), stdin);
                texte[strcspn(texte, "\n")] = '\0';
                afficher_par_genre(&cat, texte);
                break;
 
            case 6:
                printf(CYAN "  Identifiant du film à modifier : " RESET);
                scanf("%d", &id);
                vider_buffer();
                modifier_film(&cat, id);
                sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);
                break;
 
            case 7:
                printf(CYAN "  Identifiant du film à supprimer : " RESET);
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
                printf(VERT GRAS "\n  👋 Au revoir !\n\n" RESET);
                break;
 
            default:
                printf(ROUGE "  ⚠️  Choix invalide, veuillez réessayer.\n" RESET);
        }
 
    } while (choix != 0);
   
    sauvegarderFilms(cat.tab, cat.n, NOM_FICHIER);
    clear_catalogue(&cat);
    return 0;
}