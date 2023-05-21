#include <gtk/gtk.h>

// Global variables
char *query = NULL;

// Callback function for the button click event
void save_text(GtkButton *button, gpointer data)
{
    GtkWidget *text_entry = (GtkWidget *)data;
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(text_entry));
    query = strdup(entry_text);
	//SEARCH

    //g_print("Text saved: %s\n", query);
}

int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Fantastic Search Engine");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a text entry field
    GtkWidget *text_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Enter text here");
    gtk_box_pack_start(GTK_BOX(vbox), text_entry, FALSE, FALSE, 0);

    // Create a button
    GtkWidget *button = gtk_button_new_with_label("Search");
    g_signal_connect(button, "clicked", G_CALLBACK(save_text), text_entry);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Create a label to display the saved text
    GtkWidget *label = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // Update the label with the saved text
    gtk_label_set_text(GTK_LABEL(label), text ? text : "");

    // Show all widgets
