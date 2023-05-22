#ifndef USER_INTERFACE
#define USER_INTERFACE

#include "../searcher/searcher.h"
#include <gtk/gtk.h>

typedef struct {
    ForwardTable *forward;
    HashTable *table_wordId;
    InvertedTable *table_inverted;
    Graph *graph;
    GtkWidget *entry;
} ButtonClickedData;

void button_clicked(GtkWidget *widget, gpointer data);

void GtkMain(ForwardTable *forward, HashTable *table_wordId, InvertedTable *table_inverted, Graph *graph);
#endif
