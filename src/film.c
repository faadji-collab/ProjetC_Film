#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "film.h"
#include <ctype.h>

Film *chargerFilms(char *nomFichier, int *nbFilms) {
      *nbFilms = 0;

    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        /* Le fichier n'existe pas encore : on le crée vide */
        f = fopen(nomFichier, "w");
        if (f != NULL) fclose(f);
        return NULL;
    }

    /* 1re passe : compter les lignes pour allouer exactement */
    int nb = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) nb++;
    rewind(f);

    if (nb == 0) { fclose(f); return NULL; }

    Film *tab = malloc(nb * sizeof(Film));
    if (tab == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        fclose(f);
        return NULL;
    }

    /* 2e passe : lire chaque ligne au format id;titre;genre;annee;note */
    int i = 0;
    while (i < nb && fgets(ligne, sizeof(ligne), f)) {
        /* Supprimer le '\n' final si présent */
        ligne[strcspn(ligne, "\r\n")] = '\0';
        if (sscanf(ligne, "%d;%99[^;];%49[^;];%d;%f",
                   &tab[i].id,
                   tab[i].titre,
                   tab[i].genre,
                   &tab[i].annee,
                   &tab[i].note) == 5) {
            i++;
        }
    }
    fclose(f);

    *nbFilms = i;
    return tab;
}

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

static int contient_insensible(const char *chaine, const char *motif) {
    int lc = (int)strlen(chaine);
    int lm = (int)strlen(motif);
    for (int i = 0; i <= lc - lm; i++) {
        int ok = 1;
        for (int j = 0; j < lm; j++) {
            if (tolower((unsigned char)chaine[i + j]) !=
                tolower((unsigned char)motif[j])) {
                ok = 0;
                break;
            }
        }
        if (ok) return 1;
    }
    return 0;
}

static int assurer_capacite(Catalogue *c) {
    if (c->n < c->alloc) return 1;
    int nouvelle_alloc = (c->alloc == 0) ? 4 : c->alloc * 2;
    Film *tmp = realloc(c->tab, nouvelle_alloc * sizeof(Film));
    if (tmp == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        return 0;
    }
    c->tab   = tmp;
    c->alloc = nouvelle_alloc;
    return 1;
}

static int id_max(const Catalogue *c) {
    int max = 0;
    for (int i = 0; i < c->n; i++)
        if (c->tab[i].id > max) max = c->tab[i].id;
    return max;
}

void ajouter_film(Catalogue *c) {
    if (!assurer_capacite(c)) return;
    Film f;
    f.id = id_max(c) + 1;
    printf("Titre : ");
    scanf(" %99[^\n]", f.titre);
    printf("Genre : ");
    scanf(" %49[^\n]", f.genre);
    printf("Année de sortie : ");
    scanf("%d", &f.annee);
    do {
        printf("Note (0 à 10) : ");
        scanf("%f", &f.note);
        if (f.note < 0 || f.note > 10)
            printf("  → Note invalide, réessayez.\n");
    } while (f.note < 0 || f.note > 10);
    c->tab[c->n] = f;
    c->n++;
    printf("Film « %s » ajouté avec l'ID %d.\n", f.titre, f.id);
}

void afficher_films(const Catalogue *c) {
    if (c->n == 0) { printf("Le catalogue est vide.\n"); return; }
    printf("\n%-5s %-30s %-20s %-6s %s\n", "ID", "Titre", "Genre", "Année", "Note");
    printf("%-5s %-30s %-20s %-6s %s\n", "---", "------------------------------", "--------------------", "------", "----");
    for (int i = 0; i < c->n; i++)
        printf("%-5d %-30s %-20s %-6d %.1f\n",
               c->tab[i].id, c->tab[i].titre,
               c->tab[i].genre, c->tab[i].annee, c->tab[i].note);
}

void rechercher_par_id(const Catalogue *c, int id) {
    for (int i = 0; i < c->n; i++) {
        if (c->tab[i].id == id) {
            printf("\nID    : %d\n",    c->tab[i].id);
            printf("Titre : %s\n",      c->tab[i].titre);
            printf("Genre : %s\n",      c->tab[i].genre);
            printf("Année : %d\n",      c->tab[i].annee);
            printf("Note  : %.1f/10\n", c->tab[i].note);
            return;
        }
    }
    printf("Aucun film trouvé avec l'ID %d.\n", id);
}

void rechercher_par_titre(const Catalogue *c, const char *motif) {
    int trouve = 0;
    for (int i = 0; i < c->n; i++) {
        if (contient_insensible(c->tab[i].titre, motif)) {
            printf("  [%d] %s (%d) – %.1f/10\n",
                   c->tab[i].id, c->tab[i].titre,
                   c->tab[i].annee, c->tab[i].note);
            trouve++;
        }
    }
    if (!trouve) printf("Aucun film ne correspond à « %s ».\n", motif);
}

void afficher_par_genre(const Catalogue *c, const char *genre) {
    int trouve = 0;
    printf("\nFilms de genre « %s » :\n", genre);
    for (int i = 0; i < c->n; i++) {
        if (contient_insensible(c->tab[i].genre, genre)) {
            printf("  [%d] %-30s %d  %.1f/10\n",
                   c->tab[i].id, c->tab[i].titre,
                   c->tab[i].annee, c->tab[i].note);
            trouve++;
        }
    }
    if (!trouve) printf("  Aucun film trouvé pour ce genre.\n");
}

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
            scanf(" %99[^\n]", buf);
            if (buf[0] != '\0') strncpy(c->tab[i].titre, buf, 99);
            printf("Nouveau genre (Entrée pour conserver) : ");
            scanf(" %49[^\n]", buf);
            if (buf[0] != '\0') strncpy(c->tab[i].genre, buf, 49);
            printf("Nouvelle année (0 pour conserver) : ");
            scanf("%d", &val_i);
            if (val_i != 0) c->tab[i].annee = val_i;
            printf("Nouvelle note (-1 pour conserver) : ");
            scanf("%f", &val_f);
            if (val_f >= 0 && val_f <= 10) c->tab[i].note = val_f;
            printf("Film modifié avec succès.\n");
            return;
        }
    }
    printf("Aucun film trouvé avec l'ID %d.\n", id);
}

void supprimer_film(Catalogue *c, int id) {
    for (int i = 0; i < c->n; i++) {
        if (c->tab[i].id == id) {
            printf("Film « %s » supprimé.\n", c->tab[i].titre);
            for (int j = i; j < c->n - 1; j++)
                c->tab[j] = c->tab[j + 1];
            c->n--;
            return;
        }
    }
    printf("Aucun film trouvé avec l'ID %d.\n", id);
}

void trier_par_note(Catalogue *c) {
    for (int i = 0; i < c->n - 1; i++)
        for (int j = 0; j < c->n - 1 - i; j++)
            if (c->tab[j].note < c->tab[j + 1].note) {
                Film tmp      = c->tab[j];
                c->tab[j]     = c->tab[j + 1];
                c->tab[j + 1] = tmp;
            }
    printf("Catalogue trié par note décroissante.\n");
    afficher_films(c);
}

void trier_par_titre(Catalogue *c) {
    for (int i = 0; i < c->n - 1; i++)
        for (int j = 0; j < c->n - 1 - i; j++)
            if (strcmp(c->tab[j].titre, c->tab[j + 1].titre) > 0) {
                Film tmp      = c->tab[j];
                c->tab[j]     = c->tab[j + 1];
                c->tab[j + 1] = tmp;
            }
    printf("Catalogue trié par ordre alphabétique.\n");
    afficher_films(c);
}

void top5_films(const Catalogue *c) {
    if (c->n == 0) { printf("Le catalogue est vide.\n"); return; }
    Film *copie = malloc(c->n * sizeof(Film));
    if (copie == NULL) { fprintf(stderr, "Erreur mémoire.\n"); return; }
    memcpy(copie, c->tab, c->n * sizeof(Film));
    for (int i = 0; i < c->n - 1; i++)
        for (int j = 0; j < c->n - 1 - i; j++)
            if (copie[j].note < copie[j + 1].note) {
                Film tmp   = copie[j];
                copie[j]   = copie[j + 1];
                copie[j+1] = tmp;
            }
    int limite = c->n < 5 ? c->n : 5;
    printf("\n=== TOP %d FILMS ===\n", limite);
    for (int i = 0; i < limite; i++)
        printf("%d. [%.1f] %s (%d)\n", i + 1, copie[i].note, copie[i].titre, copie[i].annee);
    free(copie);
}

void afficher_statistiques(const Catalogue *c) {
    if (c->n == 0) { printf("Le catalogue est vide.\n"); return; }
    float somme = 0;
    int idx_max = 0, idx_min = 0;
    for (int i = 0; i < c->n; i++) {
        somme += c->tab[i].note;
        if (c->tab[i].note > c->tab[idx_max].note) idx_max = i;
        if (c->tab[i].note < c->tab[idx_min].note) idx_min = i;
    }
    printf("\n=== STATISTIQUES ===\n");
    printf("Nombre de films   : %d\n", c->n);
    printf("Note moyenne      : %.2f / 10\n", somme / c->n);
    printf("Meilleur film     : %s (%.1f)\n", c->tab[idx_max].titre, c->tab[idx_max].note);
    printf("Film le moins bon : %s (%.1f)\n", c->tab[idx_min].titre, c->tab[idx_min].note);
    printf("\nFilms par genre :\n");
    char genres_vus[50][50];
    int  compteurs[50];
    int  nb_genres = 0;
    for (int i = 0; i < c->n; i++) {
        int j;
        for (j = 0; j < nb_genres; j++) {
            if (contient_insensible(genres_vus[j], c->tab[i].genre) &&
                contient_insensible(c->tab[i].genre, genres_vus[j])) {
                compteurs[j]++;
                break;
            }
        }
        if (j == nb_genres && nb_genres < 50) {
            strncpy(genres_vus[nb_genres], c->tab[i].genre, 49);
            compteurs[nb_genres] = 1;
            nb_genres++;
        }
    }
    for (int j = 0; j < nb_genres; j++)
        printf("  %-20s : %d film(s)\n", genres_vus[j], compteurs[j]);
}