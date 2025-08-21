#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <iostream>

// ---------------- CONFIG ----------------
// Screen resolution
const int SCREEN_WIDTH  = 1920;
const int SCREEN_HEIGHT = 1080;

// Widget position
const int TOP_MARGIN    = 60;
const int RIGHT_MARGIN  = 30;
// --------------------------------------

class GifPlayer {
public:
    GifPlayer(const char* gif_path, int top_margin = TOP_MARGIN, int right_margin = RIGHT_MARGIN) {
        gtk_init(nullptr, nullptr);

        // Load animated GIF
        GError *error = nullptr;
        animation = gdk_pixbuf_animation_new_from_file(gif_path, &error);
        if (!animation) {
            std::cerr << "Failed to load GIF: " << error->message << std::endl;
            g_error_free(error);
            return;
        }

        int gif_width  = gdk_pixbuf_animation_get_width(animation);
        int gif_height = gdk_pixbuf_animation_get_height(animation);

        // Create top-level window
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(GTK_WINDOW(window), gif_width, gif_height);
        gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // no titlebar
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

        // Hide from dash/overview
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
        gtk_window_set_skip_pager_hint(GTK_WINDOW(window), TRUE);

        // Position at top-right
        int x = SCREEN_WIDTH - gif_width - right_margin;
        int y = top_margin;
        gtk_window_move(GTK_WINDOW(window), x, y);

        image = gtk_image_new_from_animation(animation);
        gtk_container_add(GTK_CONTAINER(window), image);

        // Make draggable anywhere
        g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press), this);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

        gtk_widget_show_all(window);
        gtk_main();
    }

private:
    GtkWidget *window;
    GtkWidget *image;
    GdkPixbufAnimation *animation;

    static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
        if (event->button == 1) { // left click
            gtk_window_begin_move_drag(GTK_WINDOW(widget),
                                       event->button,
                                       static_cast<int>(event->x_root),
                                       static_cast<int>(event->y_root),
                                       event->time);
        }
        return TRUE;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <gif_path>" << std::endl;
        return 1;
    }

    GifPlayer player(argv[1], TOP_MARGIN, RIGHT_MARGIN);
    return 0;
}

