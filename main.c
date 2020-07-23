/*=====================================================
# Modul:            	Programmierung I WS16/17
# Uhrheber:          	Jonathan Vincent Cremer
# MatrikelNr:       	41889
# Studiengruppennummer: 16 041 01
# Projekt:          	Belegaufgabe
# letzte Änderung:	26.02.2017
=====================================================*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "hauptteil.h"
#include <ctype.h>
#include <time.h>

//Struktur definieren zur einfacheren Parameterübergabe
typedef struct {
        GtkWidget *window;
        GtkWidget *neuHinzuBtn;
	GtkWidget *VerlaufsTabelle;
	GtkWidget *neueEingabe;
        GtkWidget *anzahl;
	GtkWidget *anzahlNeu;
	GtkWidget *ArtEingabe;
	GtkWidget *altArtBtn;
	GtkWidget *sucheEntry;
	GtkWidget *sucheEntryL;
	GtkWidget *neuArtEntry;
	GtkWidget *Tabelle;
	GtkWidget *nummerEntry;
	GtkWidget *artikelwahl_combobox;
	GtkWidget *statusbar;
	GtkWidget *deleteBtn;
	GtkWidget *deleteBtnBst;
	
        GtkListStore *liste;	
	GtkListStore *VerlaufsListe;
	
	char	datum[11];
        tLief	*Lieferungen;
        tArt	*Bestand;
} AppAttributes;

 
enum {
   COLUMN_STRING,
   COLUMN_NR,
   COLUMN_LBST,
   N_COLUMNS
 };

enum {
   L_COLUMN_STRING,
   L_COLUMN_NR,
   L_COLUMN_TYP,
   L_COLUMN_Z,
   L_COLUMN_DAT,
   L_COLUMN_LID,
   L_N_COLUMNS
 };
 
//Sichtbare Tabellen aktualisieren durch Neueinlesen der Arrays
void aktualisiereTabelle(AppAttributes *app)
{
  GtkTreeIter iter;
  gint i;
  gtk_list_store_clear(GTK_LIST_STORE(app->liste));

  for (i = 0; i < Anzahl; i++)
    {
      gtk_list_store_append (GTK_LIST_STORE(app->liste), &iter);
      gtk_list_store_set (GTK_LIST_STORE(app->liste), &iter,
                          COLUMN_STRING, app->Bestand[i].name,                          
                          COLUMN_NR, app->Bestand[i].nr,
                          COLUMN_LBST, app->Bestand[i].Lbst,
                          -1);
    }
    
    
  gtk_list_store_clear(GTK_LIST_STORE(app->VerlaufsListe));    
  for (i = 0; i < LAnzahl; i++)
    {
      gtk_list_store_append (GTK_LIST_STORE(app->VerlaufsListe), &iter);
      gtk_list_store_set (GTK_LIST_STORE(app->VerlaufsListe), &iter,
                          L_COLUMN_STRING, 	app->Lieferungen[i].name,                          
                          L_COLUMN_NR, 		app->Lieferungen[i].nr,
                          L_COLUMN_TYP, 	app->Lieferungen[i].typ,
			  L_COLUMN_Z, 		app->Lieferungen[i].anzahl,
			  L_COLUMN_DAT, 	app->Lieferungen[i].datum,
			  L_COLUMN_LID,		app->Lieferungen[i].liefernr,
                          -1);
    }

}

//Funktion zur überprüfung, ob es sich bei einer Zeichenkette um eine Zahl handelt
int istZahl(char* nummer, int l){
  int i = 0;
  
  if(nummer[0] == '-')
    i++;
  if(atoi(nummer)==0)
    return 0;
  
  for(; i<l; i++){
        if ( ! (isdigit(*(nummer+i))))
	     return 0;
  }
  return 1;
}
//Funktion zur überprüfung, ob es sich bei einer Zeichenkette um ein Wort handelt
int istWort(char* text, int l){
  int i;
  for(i = 0; i<l; i++){
        if ( ! (isalpha(*(text+i)))){
             i = 0;
	     return 0;
        } 
  }
  return 1;
}

//Funktion um die Beschriftung des labels "statusbar" zu verändern und Eingabefelder zu leeren
void statusmeldung(char* text, AppAttributes* app)
{
      gtk_label_set_text (GTK_LABEL((app)->statusbar),text);
      gtk_entry_set_text (GTK_ENTRY((app)->anzahl),"");
      gtk_entry_set_text (GTK_ENTRY((app)->anzahlNeu),"");  
      gtk_entry_set_text (GTK_ENTRY((app)->neuArtEntry),"");
      gtk_entry_set_text (GTK_ENTRY((app)->nummerEntry),"");
}

/*Funktion um neuen Artikel in den Bestand und in die Tabelle Lieferungen aufzunehmen
Die Werte werden aus den Texteintragsfeldern geladen.
*/
static void hinzu(GtkWidget *widget,
                gpointer app_data)
{
  AppAttributes* app = (AppAttributes*)app_data;
  int anz;
  

  tLief localLief;
  tArt localArt;
  char name[25], nummer[6],z[6];
  strcpy(name,gtk_entry_get_text (GTK_ENTRY((app)->neuArtEntry)));
  strcpy(nummer,gtk_entry_get_text (GTK_ENTRY((app)->nummerEntry)));
  strcpy(z,gtk_entry_get_text (GTK_ENTRY((app)->anzahlNeu)));


  
//überprüfung, ob Eingaben sinnwoll sind, wenn nicht: Abbruch
  if(!istWort(name, strlen(name)))
  {
      statusmeldung("Bitte gültigen Namen eingeben", app);
      return;
  }
  if(strlen(nummer)!=5 || !(istZahl(nummer, 5)))
  {
      statusmeldung("Bitte fünfstellige Artikelnummer eingeben", app);
      return;
  }
  if(!istZahl(z,strlen(z)) || z[0] == '-' || strlen(z) > 5)
  {
      statusmeldung("Bitte gültige Anzahl eingeben (höchstens fünfstellig)", app);
      return;
  }
  if(!Artikelistneu(name, atoi(nummer), app->Bestand, Anzahl))
  {
      statusmeldung("Artikel ist bereits im Bestand enthalten", app);
      return;
  }
////////////////////////////////////////////////////////////////   
  
  
  anz = atoi(z);
  strcpy(localLief.typ,"Zugang");
  /////////////////////////////////////////////////////////////

//Attributen Werte zuweisen
  localArt.nr = atoi(nummer);
  localLief.nr = atoi(nummer);
  localArt.Lbst = anz;
  localLief.anzahl = anz;
  localLief.liefernr = (app->Lieferungen[LAnzahl-1]).liefernr + 1;
  strcpy(localLief.datum, app->datum);
      
  strcpy(localLief.name,name);
  strcpy(localArt.name,name);
 
//Neues Element in Bestandsarray und Lieferungsarray einfügen 
  LAnzahl++;
  app->Lieferungen = realloc(app->Lieferungen, LAnzahl*sizeof(tLief));
  app->Lieferungen[LAnzahl-1] = localLief;
  
  Anzahl++;
  app->Bestand = realloc(app->Bestand, Anzahl*sizeof(tArt));
  app->Bestand[Anzahl-1] = localArt;
  
  statusmeldung("Hinzugefügt", app);
  aktualisiereTabelle(((AppAttributes*)app));
}
  
//Wareneingang und Ausgang von bereits vorhandenem Artikel
//Artikel wird aus Combobox ausgewält
//ähnliches Prozedere wie in Funktion "hinzu()"
static void liefern(GtkWidget *widget,
                gpointer app_data)
{
  AppAttributes* app = (AppAttributes*)app_data;
  tLief localLief;
  int aktiv = gtk_combo_box_get_active (GTK_COMBO_BOX(app->artikelwahl_combobox));
  if(aktiv==-1)
  {
      statusmeldung("Bitte einen bestehenden Artikel auswählen", app);
      return;
  }
  int anz;
  char z[6];
  strcpy(z,gtk_entry_get_text (GTK_ENTRY((app)->anzahl)));
    
  if(!istZahl(z,strlen(z)) || strlen(z) > 5)
  {
      statusmeldung("Bitte gültige Anzahl eingeben (höchstens fünf Zeichen)", app);
      return;
  }

  if (z[0]=='-'){
      anz = atoi(z)*-1;
      if(anz > app->Bestand[aktiv].Lbst)
      {
	statusmeldung("Nicht genügen Artikel vorhanden", app);
	return;	
      }
      strcpy(localLief.typ,"Abgang");
      app->Bestand[aktiv].Lbst -= anz;
  }
  else{
      strcpy(localLief.typ,"Zugang");
      anz = atoi(z);
      app->Bestand[aktiv].Lbst += anz;
  }
 
  strcpy(localLief.name, app->Bestand[aktiv].name);
  localLief.nr = app->Bestand[aktiv].nr;
  localLief.anzahl = anz;
  localLief.liefernr = (app->Lieferungen[LAnzahl-1]).liefernr + 1;
  strcpy(localLief.datum, app->datum);

 
  LAnzahl++;
  app->Lieferungen = realloc(app->Lieferungen, LAnzahl*sizeof(tLief));
  app->Lieferungen[LAnzahl-1] = localLief;
  
  
  gtk_entry_set_text (GTK_ENTRY((app)->anzahl),"");
  aktualisiereTabelle(((AppAttributes*)app));
 
  gtk_label_set_text (GTK_LABEL((app)->statusbar),"Hinzugefügt");
}

//Einzelnes Element aus dem Lieferungsarray löschen und nicht mehr anzeigen lassen
//Der Bestand wird davon nicht beeinflusst
static void delete(GtkWidget *widget,
                gpointer app_data)
{
  AppAttributes* app = (AppAttributes*)app_data;

//ausgewälte Zeile herausfinden  
  GtkTreeSelection *selection;
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app->VerlaufsTabelle));
 
  GtkTreeModel *model;
  model = gtk_tree_view_get_model (GTK_TREE_VIEW(app->VerlaufsTabelle));
  
  GtkTreeIter aktiv;
  if((gtk_tree_selection_get_selected (selection,
                                   &model,
                                   &aktiv)) == 0){
    gtk_label_set_text (GTK_LABEL((app)->statusbar),"Kein Datensatz ausgewählt");
    return;
  }
  
//Lieferungsnummer aus Zeile entnehmen
  int id;  
  gtk_tree_model_get(model, &aktiv, L_COLUMN_LID, &id, -1);

//gewähltes Elememnt aus Array entfernen
  LAnzahl--;
  deleteElement(id, app->Lieferungen, LAnzahl);

//Sichtbare Tabellen aktualisieren durch Neueinlesen der Arrays
  aktualisiereTabelle(app);
  gtk_label_set_text (GTK_LABEL((app)->statusbar),"Datensatz entfernt");
  
}

//Artikel aus dem Bestand entfernen
static void deleteBst(GtkWidget *widget,
                gpointer app_data)
{
  AppAttributes* app = (AppAttributes*)app_data;
  
  GtkTreeSelection *selection;
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app->Tabelle));
  
  GtkTreeModel *model;
  
  model = gtk_tree_view_get_model (GTK_TREE_VIEW(app->Tabelle));
  
  GtkTreeIter aktiv;
  if((gtk_tree_selection_get_selected (selection,
                                   &model,
                                   &aktiv)) == 0){
    return;
  }
  
  int nr;
  
  gtk_tree_model_get(model, &aktiv, L_COLUMN_NR, &nr, -1);

  Anzahl--;
  deleteArtikel(app->Bestand,nr, Anzahl);

//Sichtbare Tabellen aktualisieren durch Neueinlesen der Arrays
  aktualisiereTabelle(app);  
}

int main( int   argc,
          char *argv[] )
{
if (argc!=3){fprintf(stderr,"usage: %s <filename> <filename> \n","a.out"); exit(1);}


GtkBuilder *builder;

//Objekt vom Typ AppAttributes erstellen
AppAttributes *app = g_slice_new(AppAttributes);

//Bestand aus übergebener Datei in Array einlesen
app->Bestand = holeBestand(argv[1]);

//Lieferungen aus übergebener Datei in Array einlesen
app->Lieferungen = holeLieferungen(argv[2]);

gtk_init(&argc,&argv);

builder = gtk_builder_new();

//Glade File einbinden
gtk_builder_add_from_file(builder, "window.glade",NULL);

//Aktuelles Datum durch Funktion getDatum() herausfinden und in app->datum speichern
getDatum(app->datum);

//Makro definieren zum einbinden von Widgets, auf die man im Programmverlauf
//zugreifen möchte

#define appGET(xx) \
 app->xx=GTK_WIDGET(gtk_builder_get_object(builder,#xx))

//Widgets aus Gladefile holen
appGET(window);
appGET(neuHinzuBtn);
appGET(VerlaufsTabelle);
appGET(neueEingabe);
appGET(anzahl);
appGET(anzahlNeu);
appGET(ArtEingabe);
appGET(altArtBtn);
appGET(artikelwahl_combobox);
appGET(sucheEntry);
appGET(sucheEntryL);
appGET(neuArtEntry);
appGET(nummerEntry);
appGET(Tabelle);
appGET(statusbar);
appGET(deleteBtn);
appGET(deleteBtnBst);
////////////////////////////////////////////////////////////////////////////
#define appGETLIST(xx) \
 app->xx=GTK_LIST_STORE(gtk_builder_get_object(builder,#xx))
appGETLIST(liste);
appGETLIST(VerlaufsListe);

//Liste einlesen
  GtkTreeIter iter;
  gint i;
  
//   gtk_list_store_clear(GTK_LIST_STORE(app->liste));

  for (i = 0; i < Anzahl; i++)
    {
      gtk_list_store_append (GTK_LIST_STORE(app->liste), &iter);
      gtk_list_store_set (GTK_LIST_STORE(app->liste), &iter,
                          COLUMN_STRING,	app->Bestand[i].name,                          
                          COLUMN_NR,		app->Bestand[i].nr,
                          COLUMN_LBST,		app->Bestand[i].Lbst,
                          -1);
    }

//VerlaufsListe einlesen
  gtk_list_store_clear(GTK_LIST_STORE(app->VerlaufsListe));    
  for (i = 0; i < LAnzahl; i++)
    {
      gtk_list_store_append (GTK_LIST_STORE(app->VerlaufsListe), &iter);
      gtk_list_store_set (GTK_LIST_STORE(app->VerlaufsListe), &iter,
                          L_COLUMN_STRING, 	app->Lieferungen[i].name,                          
                          L_COLUMN_NR, 		app->Lieferungen[i].nr,
                          L_COLUMN_TYP, 	app->Lieferungen[i].typ,
			  L_COLUMN_Z, 		app->Lieferungen[i].anzahl,
			  L_COLUMN_DAT, 	app->Lieferungen[i].datum,
			  L_COLUMN_LID,		app->Lieferungen[i].liefernr,
                          -1);
    }


//Signale
gtk_builder_connect_signals(builder, app);
g_signal_connect (app->window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
g_signal_connect (app->altArtBtn, "clicked", G_CALLBACK (liefern), app);
g_signal_connect (app->neuHinzuBtn, "clicked", G_CALLBACK (hinzu), app);
g_signal_connect (app->deleteBtn, "clicked", G_CALLBACK (delete), app);
g_signal_connect (app->deleteBtnBst, "clicked", G_CALLBACK (deleteBst), app);

//Suche für Benutzer aktivieren
gtk_tree_view_set_search_entry (GTK_TREE_VIEW(app->Tabelle),
                                GTK_ENTRY(app->sucheEntry));
gtk_tree_view_set_search_entry (GTK_TREE_VIEW(app->VerlaufsTabelle),
                                GTK_ENTRY(app->sucheEntryL));


gtk_widget_show(GTK_WIDGET(app->window));
g_object_unref(G_OBJECT(builder));
gtk_main();


//(Veränderte) Arrays zurück in csv-Dateien schreiben
speichern(app->Bestand, Anzahl, app->Lieferungen, LAnzahl,argv[1],argv[2]);

//Speicher freigeben
free(app->Bestand);
free(app->Lieferungen);

g_slice_free(AppAttributes, app);


return 0;
}





