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
#include <time.h>

#include "hauptteil.h"

char vbuf[128];

FILE * fp;

//Bestand aus daten.csv Datei in Array einlesen
tArt* holeBestand(char* csvBestand)
{

  if((fp=fopen(csvBestand,"r+t"))==NULL){
      fprintf(stderr,"Fileerror %s\n",csvBestand); exit(1);}

//Streampointer fp zurück auf den Anfang setzen
  rewind(fp);
  tArt* Bestand;

//Anzahl der Elemente im Bestand herausfinden
  Anzahl = zeilenanzahl(fp);
  
//Speicher bereitstellen für Array Bestand
  Bestand = malloc(Anzahl * sizeof(tArt));

//Bestand aus Datei einlesen
  int k = 0;
  for(; k < Anzahl; k++)
  {
    Bestand[k]=readArtFromFile(fp);
  }
  
  fclose(fp);
  return Bestand;
}
//Lieferungen aus Lieferungen.csv Datei in Array einlesen
tLief* holeLieferungen(char* csvLieferungen)
{
  fp=fopen(csvLieferungen,"r+t");
  if(fp==NULL){fprintf(stderr,"Fileerror %s\n",csvLieferungen); exit(1);}

  rewind(fp);
  tLief* LiefList;

  LAnzahl = zeilenanzahl(fp);
  
	  
  LiefList = malloc(LAnzahl * sizeof(tLief));

  rewind(fp);
  int k = 0;
  for(; k < LAnzahl; k++)
  {
    LiefList[k]=readLiefFromFile(fp);
  }
  rewind(fp);
  
  fclose(fp);
  return LiefList;
}

//Funktion, die die Anzahl der Zeilen in einer Datei zurückgibt
int zeilenanzahl(FILE *fp)
{
  rewind(fp);
  char c;
  int zeilenanzahl=0;
  do
  {
    c = fgetc(fp);


    if (c=='\n') {zeilenanzahl++;}

  }while (c!=EOF);
  
  rewind(fp);
  return zeilenanzahl;
}
  
  
//Funktion die aktuelles Datum ermittelt
void getDatum(char *ret)
{
  time_t jetzt;
  struct tm jetztLokal;
  jetzt = time(NULL);
  
  jetztLokal = *localtime(&jetzt);
  
  char dat[11];

  sprintf(dat, "%02d.%02d.%4d", jetztLokal.tm_mday,
				jetztLokal.tm_mon + 1,
				jetztLokal.tm_year+1900);
  strcpy(ret,dat);
}


//(Veränderte) Arrays zurück in csv-Dateien schreiben
void speichern(tArt* Bestand, int ende, tLief* Lieferung, int endeL,
	       char* csvBestand, char* csvLieferungen)
{
    FILE *fp;
    fp = fopen(csvBestand, "w");
    
    int j = 0;
    for(; j < ende; j++)
    {
	fprintf(fp,"%s,%u,%d\n",
	      Bestand[j].name,
	      Bestand[j].nr,
	      Bestand[j].Lbst);
    }
    fclose(fp);
    
    fp = fopen(csvLieferungen, "w");
    
    for(j=0; j < endeL; j++)
    {
	fprintf(fp,"%s,%u,%s,%u,%s,%05u\n",
	      Lieferung[j].name,
	      Lieferung[j].nr,
	      Lieferung[j].typ,
	      Lieferung[j].anzahl,
	      Lieferung[j].datum,
	      Lieferung[j].liefernr
 	      );
    }
    fclose(fp);
}






