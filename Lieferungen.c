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

#include "Lieferungen.h"

//Funktion, die eine Lieferung aus Datei durch zeilenweises
//auslesen zurückgibt
tLief readLiefFromFile(FILE* f)
{
  tLief l={};
  char *p, *t, *d;
//Zeile in vbuf einlesen
  if(fgets(vbuf,128,f)!=NULL)
  {
//Zeichenkette mit strtok zerlegen
     p=strtok(vbuf,",");
     strcpy(l.name,p);
     l.nr =atoi(strtok(NULL,","));
     
     t=strtok(NULL,",");
     strcpy(l.typ,t);
     l.anzahl =atoi(strtok(NULL,","));
     d=strtok(NULL,",");
     strcpy(l.datum,d);
     l.liefernr = atoi(strtok(NULL,"\n"));
  }
  return l;
}

//Übergebenen Artikel aus Array entfernen
void deleteElement(int liefernummer, tLief *liste, int size)
{
//Zu entfernenden Artikel im Array "Bestand" suchen (determiniert durch Lieferungsnummer)   
  int i;
  for(i=0;;i++)
  {
     if(liste[i].liefernr == liefernummer)
	break;
  }
  
  
//Überschreiben der übergebenen Lieferung durch 
//Verschieben der nachfolgenden Lieferungen um Eins nach vorne
  int k;
  for(k=i; k<size ; k++)
  {
      liste[k] = liste[k+1];
  }
  liste = realloc(liste, sizeof(liste) - sizeof(tLief));
}