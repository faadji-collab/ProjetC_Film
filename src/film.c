#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "film.h"
#include <ctype.h>

//==============COULEURS=====================================//
#define RESET   "\033[0m"
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define GRAS    "\033[1m"

static int id_max(const Catalogue *c);
static int contient_insensible(const char *texte, const char *motif);

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

    if (tableau == NULL && compteur > 0)
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
        if (token == NULL) continue;
        tableau[i].id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(tableau[i].titre, token, sizeof(tableau[i].titre) - 1);
        tableau[i].titre[sizeof(tableau[i].titre) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(tableau[i].genre, token, sizeof(tableau[i].genre) - 1);
        tableau[i].genre[sizeof(tableau[i].genre) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        tableau[i].annee = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        tableau[i].note = atof(token);

        i++;
    }

    fclose(fichier);   
    *nbFilms = i;

    return tableau;
}

//============SAUVEGARDER FILMS=========================//
int sauvegarderFilms(const Film *tableau, int nbFilms, const char *nomFichier) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'écrire dans %s\n", nomFichier);
        return 0;
    }
    for (int i = 0; i < nbFilms; i++) {
        fprintf(f, "%d;%s;%s;%d;%.1f\n",
                tableau[i].id, tableau[i].titre, tableau[i].genre,
                tableau[i].annee, tableau[i].note);
    }
    fclose(f);
    return 1;
}

//============CATALOGUE FILMS=========================//
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

//==================FONCTION AJOUTER FILM=======================//
void ajouter_film(Catalogue *c)
{
    Film nouveau;
    nouveau.id = id_max(c) + 1;

    printf(CYAN "\n===== AJOUT D'UN FILM =====\n" RESET);

    printf("Titre : ");
    fgets(nouveau.titre, sizeof(nouveau.titre), stdin);
    nouveau.titre[strcspn(nouveau.titre, "\n")] = '\0';

    printf("Genre : ");
    fgets(nouveau.genre, sizeof(nouveau.genre), stdin);
    nouveau.genre[strcspn(nouveau.genre, "\n")] = '\0';

    printf("Année : ");
    scanf("%d", &nouveau.annee);
    vider_buffer();

    printf("Note : ");
    scanf("%f", &nouveau.note);
    vider_buffer();

    Film *tmp = realloc(c->tab, (c->n + 1) * sizeof(Film));

    if(tmp == NULL)
    {
        printf(ROUGE "Erreur d'allocation mémoire.\n" RESET);
        return;
    }

    c->tab = tmp;
    c->tab[c->n] = nouveau;
    c->n++;
    c->alloc = c->n;

    sauvegarderFilms(c->tab, c->n, NOM_FICHIER);

    printf(VERT "\nFilm ajouté avec succès !\n" RESET);
}

//============AFFICHER FILM GENERALE=========================//
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

//============AFFICHER FILMS=========================//
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

//============RECHERCHER FILMS PAR ID=========================//
void rechercher_par_id(const Catalogue *c, int id)
{
    for (int i = 0; i < c->n; i++)
    {
        if (c->tab[i].id == id)
        {
            printf(VERT GRAS "\n✅ FILM TROUVÉ !\n\n" RESET);
            afficher_film_carte(&c->tab[i]);
            return;
        }
    }
    printf(ROUGE "\n❌ Aucun film avec l'ID %d.\n" RESET, id);
}

//============RECHERCHER FILMS PAR TITRE=========================//
void rechercher_par_titre(const Catalogue *c, const char *titre)
{
    int trouve = 0;
    for (int i = 0; i < c->n; i++)
    {
        if (contient_insensible(c->tab[i].titre, titre))
        {
            if (!trouve) printf(VERT GRAS "\n✅ FILM(S) TROUVÉ(S) !\n\n" RESET);
            afficher_film_carte(&c->tab[i]);
            printf("\n");
            trouve = 1;
        }
    }
    if (!trouve) printf(ROUGE "\n❌ Aucun film contenant le titre \"%s\".\n" RESET, titre);
}

//============TRIER FILMS PAR NOTE=========================//
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

//================INSESIBILITÉ À LA CASSE=======================//
int strcmp_insensible(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);

        if (c1 != c2)
            return c1 - c2;

        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1)
         - tolower((unsigned char)*s2);
}
//============TRIER FILMS PAR TITRE=========================//
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
        if (strcmp_insensible(copie[j].titre, copie[j + 1].titre) > 0)
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

//==================TOP5 FILMS=======================//
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

//==================AFFICHER STATISTIQUE=========================//
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
            strncpy(genres[nbGenres], c->tab[i].genre, 49);
            genres[nbGenres][49] = '\0';
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

static int contient_insensible(const char *texte, const char *motif) {
    char t[200], m[200];
    int i;

    for (i = 0; texte[i] && i < 199; i++)
        t[i] = (char)tolower((unsigned char)texte[i]);
    t[i] = '\0';

    for (i = 0; motif[i] && i < 199; i++)
        m[i] = (char)tolower((unsigned char)motif[i]);
    m[i] = '\0';

    return strstr(t, m) != NULL;
}

//===================ID MAX============================//
static int id_max(const Catalogue *c) {
    int max = 0;
    for (int i = 0; i < c->n; i++)
        if (c->tab[i].id > max) max = c->tab[i].id;
    return max;
}
//====================AFFICHER FILM PAR GENRE=====================//
void afficher_par_genre(const Catalogue *c, const char *genre) {
    int trouve = 0;
    printf("\nFilms de genre « %s » :\n", genre);
    for (int i = 0; i < c->n; i++) {
        if (trouve == 0)
        {
            printf("\n══════════════════════════════════════════════════════\n");
            printf("                 FILMS DU GENRE : %s\n", genre);
            printf("══════════════════════════════════════════════════════\n");
            printf(" ID   | TITRE                          | ANNEE | NOTE\n");
            printf("------------------------------------------------------\n");
        }

        printf(" %-4d | %-30s | %-5d | %-4.1f\n",
            c->tab[i].id,
            c->tab[i].titre,
            c->tab[i].annee,
            c->tab[i].note);

        trouve++;
    }
    if (!trouve) printf("  Aucun film trouvé pour ce genre.\n");
}

//=================MODIFIER FILM================================//
void modifier_film(Catalogue *c, int id) {
    for (int i = 0; i < c->n; i++) {
        if (c->tab[i].id == id) {
            printf("Film actuel : %s | %s | %d | %.1f\n",
                   c->tab[i].titre, c->tab[i].genre,
                   c->tab[i].annee, c->tab[i].note);
            char buf[100];
            int val_i;
            float val_f;

            printf("Nouveau titre (Entrée pour conserver) : ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0)
            {
                strncpy(c->tab[i].titre, buf, sizeof(c->tab[i].titre) - 1);
                c->tab[i].titre[sizeof(c->tab[i].titre) - 1] = '\0';
            }

            printf("Nouveau genre (Entrée pour conserver) : ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) 
            {
                strncpy(c->tab[i].genre, buf, sizeof(c->tab[i].genre) - 1);
                c->tab[i].genre[sizeof(c->tab[i].genre) - 1] = '\0';
            }
            printf("Nouvelle année (0 pour conserver) : ");
            if (scanf("%d", &val_i) == 1 && val_i != 0) c->tab[i].annee = val_i;
            vider_buffer();

            printf("Nouvelle note (-1 pour conserver) : ");
            if (scanf("%f", &val_f) == 1 && val_f >= 0 && val_f <= 10) c->tab[i].note = val_f;
            vider_buffer();

            printf("Film modifié avec succès.\n");
            return;
        }
    }
    printf("Aucun film trouvé avec l'ID %d.\n", id);
}

//=====================SUPPRIMER FILM============================//
void supprimer_film(Catalogue *c, int id) {
    for (int i = 0; i < c->n; i++) {
        if (c->tab[i].id == id) {
            printf("Film « %s » supprimé.\n", c->tab[i].titre);
            for (int j = i; j < c->n - 1; j++)
                c->tab[j] = c->tab[j + 1];
            c->n--;
            c->alloc = c->n;
            return;
        }
    }
    printf("Aucun film trouvé avec l'ID %d.\n", id);
}