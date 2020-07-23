#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char vbuf[128];

//Struktur für Lieferungen
typedef struct
{
  char name [28];
  int nr;
  char typ [28];
  unsigned int anzahl;
  char datum [11];
  unsigned int liefernr;
}tLief;

tLief readLiefFromFile(FILE* f);
void deleteElement(int liefernummer, tLief *liste, int size);