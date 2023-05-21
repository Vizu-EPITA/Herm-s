#include <gtk/gtk.h>

// Global variable to store the query text
char *QUERY = NULL;

// Callback function for the button click event
void button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = (GtkWidget *)data;
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));

    // Save the query text
    if (QUERY)
        g_free(QUERY);
    QUERY = g_strdup(text);

    // Display the query in the text display field
    GtkWidget *text_view = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "text_view");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, text, -1);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Text Entry Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Text field
    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Horizontal box for the button and display field
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // Button
    GtkWidget *button = gtk_button_new_with_label("Save");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    // Text display field
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);
    g_object_set_data(G_OBJECT(button), "text_view", text_view);

    gtk_widget_show_all(window);
    gtk_main();

    // Cleanup
    if (QUERY)
        g_free(QUERY);

    return 0;
}
