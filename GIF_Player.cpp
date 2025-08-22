#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <iostream>
#include <string>

// ---------------- CONFIG ----------------
// Screen resolution
const int SCREEN_WIDTH  = 1920;
const int SCREEN_HEIGHT = 1080;

// Widget position
const int TOP_MARGIN    = 60;
const int RIGHT_MARGIN  = 30;

const double DEFAULT_OPACITY = 0.9;
const double DEFAULT_SCALE   = 1.0;  // 1.0 = original size
// --------------------------------------

class GifPlayer {
public:
    GifPlayer(const char* gif_path, double opacity = DEFAULT_OPACITY, double scale = DEFAULT_SCALE)
        : scale_(scale) {
        gtk_init(nullptr, nullptr);

        // Load GIF animation
        GError *error = nullptr;
        animation_ = gdk_pixbuf_animation_new_from_file(gif_path, &error);
        if (!animation_) {
            std::cerr << "Failed to load GIF: " << (error ? error->message : "unknown error") << std::endl;
            if (error) g_error_free(error);
            return;
        }

        // Base dimensions and scaled target size
        int w = gdk_pixbuf_animation_get_width(animation_);
        int h = gdk_pixbuf_animation_get_height(animation_);
        dst_w_ = std::max(1, int(w * scale_));
        dst_h_ = std::max(1, int(h * scale_));

        // Window setup
        window_ = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_widget_add_events(window_, GDK_BUTTON_PRESS_MASK);
        gtk_window_set_default_size(GTK_WINDOW(window_), dst_w_, dst_h_);
        gtk_window_set_decorated(GTK_WINDOW(window_), FALSE);
        gtk_window_set_resizable(GTK_WINDOW(window_), FALSE);
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window_), TRUE);
        gtk_window_set_skip_pager_hint(GTK_WINDOW(window_), TRUE);
        gtk_widget_set_opacity(window_, opacity);

        // Position top-right like original
        int x = SCREEN_WIDTH - dst_w_ - RIGHT_MARGIN;
        int y = TOP_MARGIN;
        gtk_window_move(GTK_WINDOW(window_), x, y);

        // Image widget that we'll update per-frame
        image_ = gtk_image_new();
        gtk_container_add(GTK_CONTAINER(window_), image_);

        // Create iterator and show first frame
        iter_ = gdk_pixbuf_animation_get_iter(animation_, nullptr);
        present_frame_();
        schedule_next_();

        // Signals
        g_signal_connect(window_, "button-press-event", G_CALLBACK(on_button_press_), this);
        g_signal_connect(window_, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

        gtk_widget_show_all(window_);
        gtk_main();
    }

private:
    GtkWidget *window_ = nullptr;
    GtkWidget *image_  = nullptr;
    GdkPixbufAnimation *animation_ = nullptr;
    GdkPixbufAnimationIter *iter_  = nullptr;
    int dst_w_ = 0, dst_h_ = 0;
    double scale_ = 1.0;

    static gboolean on_button_press_(GtkWidget *widget, GdkEventButton *event, gpointer) {
        if (event->button == 1)
            gtk_window_begin_move_drag(GTK_WINDOW(widget), event->button, (int)event->x_root, (int)event->y_root, event->time);
        return TRUE;
    }

    static gboolean frame_cb_(gpointer data) {
        auto *self = static_cast<GifPlayer*>(data);
        if (!self || !self->iter_) return G_SOURCE_REMOVE;
        gdk_pixbuf_animation_iter_advance(self->iter_, nullptr);
        self->present_frame_();
        self->schedule_next_();
        return G_SOURCE_REMOVE; // we reschedule with the next frame's delay
    }

    void schedule_next_() {
        int delay = gdk_pixbuf_animation_iter_get_delay_time(iter_); // ms
        if (delay < 10) delay = 10; // clamp to avoid busy looping
        g_timeout_add((guint)delay, frame_cb_, this);
    }

    void present_frame_() {
        GdkPixbuf *frame = gdk_pixbuf_animation_iter_get_pixbuf(iter_);
        if (!frame) return;
        // Scale current frame
        GdkPixbuf *scaled = gdk_pixbuf_scale_simple(frame, dst_w_, dst_h_, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image_), scaled);
        g_object_unref(scaled);
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <gif_path> [-opacity val] [-scale val]\n";
        return 1;
    }

    double opacity = DEFAULT_OPACITY;
    double scale   = DEFAULT_SCALE;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-opacity" && i + 1 < argc) {
            opacity = std::stod(argv[++i]);
            if (opacity < 0.0) opacity = 0.0; if (opacity > 1.0) opacity = 1.0;
        } else if (arg == "-scale" && i + 1 < argc) {
            scale = std::stod(argv[++i]);
            if (scale <= 0.0) scale = 1.0;
        }
    }

    GifPlayer player(argv[1], opacity, scale);
    return 0;
}