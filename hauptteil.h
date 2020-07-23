#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Artikelverwaltung.h"
#include "Lieferungen.h"

tArt* holeBestand(char* csvBestand);
tLief* holeLieferungen(char* csvLieferungen);
int Anzahl;
int LAnzahl;
void getDatum(char* ret);
void speichern(tArt* Bestand, int ende, tLief* Lieferung, int endeL,
	       char* csvBestand, char* csvLieferungen);
void deleteBstElement(int nummer, tArt *liste, int size);
int zeilenanzahl(FILE *fp);