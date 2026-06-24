#ifndef FILM_H
#define FILM_H

#define NOM_FICHIER "films.csv"

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

// --- Constructeur / destructeur ---
void init_catalogue(Catalogue *c);
void clear_catalogue(Catalogue *c);

// --- Sauvegarde / chargement ---
int compter_lignes(const char *nomFichier);
int charger_catalogue(Catalogue *c, const char *nomFichier);
int sauvegarder_catalogue(const Catalogue *c, const char *nomFichier);

#endif
