#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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