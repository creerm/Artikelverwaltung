#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char vbuf[128];

//Struktur für Artikel
typedef struct
{
  char name [25+1];
  int nr;
  int Lbst;
}tArt;

tArt readArtFromFile(FILE* f);
void deleteArtikel(tArt* Bestand, int nummer, int size);
int Artikelistneu(char* name, int nummer, tArt *Bestand, int size);