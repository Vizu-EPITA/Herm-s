#ifndef USER_INTERFACE
#define USER_INTERFACE

#include "../searcher/searcher.h"
//#include "../../tools/hash_table.h"
//#include "./data_structures/graph.h"
//#include "./data_structures/shared_queue.h"
//#include "./data_structures/linked_list.h"
#include <gtk/gtk.h>

typedef struct {
    HashTable *table_docId;
    HashTable *table_wordId;
    HashTable *table_inverted;
    Graph *graph;
    //GtkWidget *entry;
} ButtonClickedData;

void button_clicked(GtkWidget *widget, gpointer data);

void GtkMain(HashTable *table_docId, HashTable *table_wordId, HashTable *table_inverted, Graph *graph);
#endif
