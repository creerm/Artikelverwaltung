/*=====================================================
# Modul:            	Programmierung I WS16/17
# Uhrheber:          	Jonathan Vincent Cremer
# MatrikelNr:       	41889
# Studiengruppennummer: 16 041 01
# Projekt:          	Belegaufgabe
# letzte Änderung:	26.02.2017
=====================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Artikelverwaltung.h"

char vbuf[128];

//Funktion, die einen Artikel aus Datei durch zeilenweises
//auslesen zurückgibt
tArt readArtFromFile(FILE* f)
{
  tArt a={};
  char* p;
//Zeile in vbuf einlesen
  if(fgets(vbuf,128,f)!=NULL)
  {
//Zeichenkette mit strtok zerlegen
    p=strtok(vbuf,",\n");
    strcpy(a.name,p);
    a.nr =atoi(strtok(NULL,","));
    a.Lbst =atoi(strtok(NULL,",\n"));
  }
  return a;
}

//Übergebenen Artikel aus Array entfernen
void deleteArtikel(tArt* Bestand, int nummer, int size)
{
//Zu entfernenden Artikel im Array "Bestand" suchen (determiniert durch Artikelnummer)
  int i;
  for(i=0;;i++)
  {
     if(Bestand[i].nr == nummer)
	break;
  }
  
//Überschreiben des übergebenen Artikels durch 
//Verschieben der nachfolgenden Artikel um Eins nach vorne
  int k;
  for(k=i; k<size ; k++)
  {
      Bestand[k] = Bestand[k+1];
  }
  Bestand = realloc(Bestand, sizeof(Bestand) - sizeof(tArt));
}

int Artikelistneu(char* name, int nummer, tArt *Bestand, int size)
{
  int i;
  for(i=0; i<size ; i++)
  {
    if(strcmp(Bestand[i].name, name)==0)
      return 0;
    if(Bestand[i].nr == nummer)
      return 0;
  }
 return 1; 
}
  
  
  

