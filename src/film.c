#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "film.h"
//==============COULEURS=====================================//
#define RESET   "\033[0m"
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define GRAS    "\033[1m"
#define NOM_FICHIER "films.csv" 
//============FONCTION CHARGER FILMS=========================//
Film *chargerFilms(char *nomFichier, int *nbFilms)
{

    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL)
    {
        *nbFilms = 0;
        return NULL;
    }

    int compteur = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        compteur++;
    }

    Film *tableau = malloc(compteur * sizeof(Film));

    if (tableau == NULL)
    {
        fclose(fichier);
        *nbFilms = 0;
        return NULL;
    }

    rewind(fichier);

    int i = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        if (ligne[0] == '\n' || ligne[0] == '\0')
        continue;

        char *token;

        token = strtok(ligne, ";");
        tableau[i].id = atoi(token);

        token = strtok(NULL, ";");
        strcpy(tableau[i].titre, token);

        token = strtok(NULL, ";");
        strcpy(tableau[i].genre, token);

        token = strtok(NULL, ";");
        tableau[i].annee = atoi(token);

        token = strtok(NULL, ";");
        tableau[i].note = atof(token);

        i++;
    }

    fclose(fichier);   

    *nbFilms = i;

    return tableau;
}

//==================FONCTION AJOUTER FILM=======================//
void ajouter_film(Catalogue *c)
{
    Film nouveau;
    int maxId = 0;

    for(int i = 0; i < c->n; i++)
    {
        if(c->tab[i].id > maxId)
        {
            maxId = c->tab[i].id;
        }
    }

    nouveau.id = maxId + 1;

    getchar();

    printf(CYAN "\n===== AJOUT D'UN FILM =====\n" RESET);

    printf("Titre : ");
    fgets(nouveau.titre, sizeof(nouveau.titre), stdin);
    nouveau.titre[strcspn(nouveau.titre, "\n")] = '\0';

    printf("Genre : ");
    fgets(nouveau.genre, sizeof(nouveau.genre), stdin);
    nouveau.genre[strcspn(nouveau.genre, "\n")] = '\0';

    printf("Année : ");
    scanf("%d", &nouveau.annee);

    printf("Note : ");
    scanf("%f", &nouveau.note);

    Film *tmp = realloc(c->tab, (c->n + 1) * sizeof(Film));

    if(tmp == NULL)
    {
        printf(ROUGE "Erreur d'allocation mémoire.\n" RESET);
        return;
    }

    c->tab = tmp;
    c->tab[c->n] = nouveau;
    c->n++;

    sauvegarderFilms(c->tab, c->n, NOM_FICHIER);

    printf(VERT "\nFilm ajouté avec succès !\n" RESET);
}

// ===============================AFFICHER UN FILM ===========================================
void afficher_film_carte(const Film *f)
{
    printf(CYAN "┌────────────────────────────────────────────┐\n" RESET);
    printf(CYAN "│ " RESET GRAS JAUNE "🎬 %-40s" RESET CYAN "│\n" RESET, f->titre);
    printf(CYAN "├────────────────────────────────────────────┤\n" RESET);
    printf(CYAN "│ " RESET "ID     : %-34d" CYAN "│\n" RESET, f->id);
    printf(CYAN "│ " RESET "Genre  : %-34s" CYAN "│\n" RESET, f->genre);
    printf(CYAN "│ " RESET "Année  : %-34d" CYAN "│\n" RESET, f->annee);

    if (f->note >= 7.0)
        printf(CYAN "│ " RESET "Note   : " VERT "%.1f/10%-28s" RESET CYAN "│\n" RESET, f->note, "");
    else if (f->note >= 4.0)
        printf(CYAN "│ " RESET "Note   : " JAUNE "%.1f/10%-28s" RESET CYAN "│\n" RESET, f->note, "");
    else
        printf(CYAN "│ " RESET "Note   : " ROUGE "%.1f/10%-28s" RESET CYAN "│\n" RESET, f->note, "");

    printf(CYAN "└────────────────────────────────────────────┘\n" RESET);
}
//====================FONCTION AJOUTER FILM===============================//
void afficher_films(const Catalogue *c)
{
    if (c->n == 0)
    {
        printf(ROUGE "\n❌ Catalogue vide.\n" RESET);
        return;
    }

    printf(BLEU GRAS "\n═══════════════════════════════════════════════\n" RESET);
    printf(BLEU GRAS "          🎞️  CATALOGUE DES FILMS  🎞️\n" RESET);
    printf(BLEU GRAS "═══════════════════════════════════════════════\n\n" RESET);

    for (int i = 0; i < c->n; i++)
    {
        afficher_film_carte(&c->tab[i]);
        printf("\n");
    }

    printf(JAUNE "Total : %d film(s)\n\n" RESET, c->n);
}

//=============RECHERCHER FILM PAR ID====================//
void rechercher_par_titre(const Catalogue *c, const char *titre)
{
    for (int i = 0; i < c->n; i++)
    {
        if (strcmp(c->tab[i].titre, titre) == 0)
        {
            printf(VERT GRAS "\nV FILM TROUVÉ !\n\n" RESET);
            afficher_film_carte(&c->tab[i]);
            return;
        }
    }

    printf(ROUGE "\nX Aucun film avec le titre \"%s\".\n" RESET, titre);
}

// =======================TRI PAR NOTE DÉCROISSANTE===========================================//
void trier_par_note(const Catalogue *c)
{
    if (c->n == 0)
    {
        printf(ROUGE "\n❌ Catalogue vide.\n" RESET);
        return;
    }

    Film *copie = malloc(c->n * sizeof(Film));
    if (copie == NULL)
    {
        printf(ROUGE "Erreur d'allocation mémoire.\n" RESET);
        return;
    }
    for (int i = 0; i < c->n; i++)
    {
        copie[i] = c->tab[i];
    }

    for (int i = 0; i < c->n - 1; i++)
    {
        for (int j = 0; j < c->n - 1 - i; j++)
        {
            if (copie[j].note < copie[j + 1].note)
            {
                Film tmp = copie[j];
                copie[j] = copie[j + 1];
                copie[j + 1] = tmp;
            }
        }
    }

    printf(BLEU GRAS "\n═══════════════════════════════════════════════\n" RESET);
    printf(BLEU GRAS "       🏆 FILMS TRIÉS PAR NOTE DÉCROISSANTE 🏆\n" RESET);
    printf(BLEU GRAS "═══════════════════════════════════════════════\n\n" RESET);

    for (int i = 0; i < c->n; i++)
    {
        afficher_film_carte(&copie[i]);
        printf("\n");
    }

    free(copie);
}


// =============================TRI ALPHABÉTIQUE PAR TITRE========================================//
void trier_par_titre(const Catalogue *c)
{
    if (c->n == 0)
    {
        printf(ROUGE "\n❌ Catalogue vide.\n" RESET);
        return;
    }

    Film *copie = malloc(c->n * sizeof(Film));
    if (copie == NULL)
    {
        printf(ROUGE "Erreur d'allocation mémoire.\n" RESET);
        return;
    }
    for (int i = 0; i < c->n; i++)
    {
        copie[i] = c->tab[i];
    }

    for (int i = 0; i < c->n - 1; i++)
    {
        for (int j = 0; j < c->n - 1 - i; j++)
        {
            if (strcmp(copie[j].titre, copie[j + 1].titre) > 0)
            {
                Film tmp = copie[j];
                copie[j] = copie[j + 1];
                copie[j + 1] = tmp;
            }
        }
    }

    printf(BLEU GRAS "\n═══════════════════════════════════════════════\n" RESET);
    printf(BLEU GRAS "         🔤 FILMS TRIÉS PAR ORDRE ALPHABÉTIQUE 🔤\n" RESET);
    printf(BLEU GRAS "═══════════════════════════════════════════════\n\n" RESET);

    for (int i = 0; i < c->n; i++)
    {
        afficher_film_carte(&copie[i]);
        printf("\n");
    }

    free(copie);
}


// ===============================TOP 5 DES FILMS===================================//
void top5_films(const Catalogue *c)
{
    if (c->n == 0)
    {
        printf(ROUGE "\n❌ Catalogue vide.\n" RESET);
        return;
    }

    Film *copie = malloc(c->n * sizeof(Film));
    if (copie == NULL)
    {
        printf(ROUGE "Erreur d'allocation mémoire.\n" RESET);
        return;
    }
    for (int i = 0; i < c->n; i++)
    {
        copie[i] = c->tab[i];
    }

    for (int i = 0; i < c->n - 1; i++)
    {
        for (int j = 0; j < c->n - 1 - i; j++)
        {
            if (copie[j].note < copie[j + 1].note)
            {
                Film tmp = copie[j];
                copie[j] = copie[j + 1];
                copie[j + 1] = tmp;
            }
        }
    }

    int limite = (c->n < 5) ? c->n : 5;

    printf(JAUNE GRAS "\n═══════════════════════════════════════════════\n" RESET);
    printf(JAUNE GRAS "             ⭐ TOP %d DES FILMS ⭐\n" RESET, limite);
    printf(JAUNE GRAS "═══════════════════════════════════════════════\n\n" RESET);

    for (int i = 0; i < limite; i++)
    {
        printf(MAGENTA GRAS "  Rang %d\n" RESET, i + 1);
        afficher_film_carte(&copie[i]);
        printf("\n");
    }

    free(copie);
}


// ============================= STATISTIQUES DU CATALOGUE==========================================//
void afficher_statistiques(const Catalogue *c)
{
    if (c->n == 0)
    {
        printf(ROUGE "\n❌ Catalogue vide.\n" RESET);
        return;
    }

    float somme = 0.0;
    int indexMeilleur = 0;
    int indexMoinsBon = 0;

    for (int i = 0; i < c->n; i++)
    {
        somme += c->tab[i].note;

        if (c->tab[i].note > c->tab[indexMeilleur].note)
        {
            indexMeilleur = i;
        }
        if (c->tab[i].note < c->tab[indexMoinsBon].note)
        {
            indexMoinsBon = i;
        }
    }

    float moyenne = somme / c->n;

    char genres[50][50];
    int compteurs[50];
    int nbGenres = 0;

    for (int i = 0; i < c->n; i++)
    {
        int trouve = 0;

        for (int j = 0; j < nbGenres; j++)
        {
            if (strcmp(genres[j], c->tab[i].genre) == 0)
            {
                compteurs[j]++;
                trouve = 1;
                break;
            }
        }

        if (!trouve)
        {
            strcpy(genres[nbGenres], c->tab[i].genre);
            compteurs[nbGenres] = 1;
            nbGenres++;
        }
    }

    printf(BLEU GRAS "\n═══════════════════════════════════════════════\n" RESET);
    printf(BLEU GRAS "           📊 STATISTIQUES DU CATALOGUE 📊\n" RESET);
    printf(BLEU GRAS "═══════════════════════════════════════════════\n\n" RESET);

    printf(CYAN "┌────────────────────────────────────────────┐\n" RESET);
    printf(CYAN "│ " RESET "Nombre total de films : %-19d" CYAN "│\n" RESET, c->n);
    printf(CYAN "│ " RESET "Note moyenne           : " VERT "%-19.2f" RESET CYAN "│\n" RESET, moyenne);
    printf(CYAN "└────────────────────────────────────────────┘\n\n" RESET);

    printf(VERT GRAS "🏆 Film le mieux noté :\n" RESET);
    afficher_film_carte(&c->tab[indexMeilleur]);

    printf(ROUGE GRAS "\n💔 Film le moins bien noté :\n" RESET);
    afficher_film_carte(&c->tab[indexMoinsBon]);

    printf(JAUNE GRAS "\n🎭 Répartition par genre :\n" RESET);
    printf(CYAN "┌────────────────────────────────────────────┐\n" RESET);
    for (int j = 0; j < nbGenres; j++)
    {
        printf(CYAN "│ " RESET "%-25s : %-15d" CYAN "│\n" RESET, genres[j], compteurs[j]);
    }
    printf(CYAN "└────────────────────────────────────────────┘\n\n" RESET);
}















