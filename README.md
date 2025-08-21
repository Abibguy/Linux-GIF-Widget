# Linux GIF Widget

A lightweight, draggable **GTK3 desktop widget** for Linux that plays animated GIFs directly on your screen.  
Designed for minimalists, coders, and desktop customizers who want dynamic, unobtrusive desktop decorations.

---

## Features

- **Titlebarless Design:** Seamlessly integrates into your desktop environment.  
- **Draggable Anywhere:** Move the widget freely to any position on your screen.  
- **Minimalist UI:** Clean and unobtrusive, ideal for ricing and productivity setups.  
- **Lightweight:** Minimal memory footprint for continuous desktop display.  
- **Animated GIF Support:** Smooth, looping GIF playback with automatic resizing.  
- **Multiple Widgets:** Launch multiple instances with different GIFs for a dynamic desktop.  

---
````

## Installation

You can either **use the precompiled executable** included in this repo, or **compile the source code yourself**.
**Add the executable binary to your startup to start on login.**  

### Dependencies (for compilation)

- **GTK3** (`gtk+-3.0`) – Main toolkit for UI and window handling  
- **GLib** (`glib-2.0`) – Utility library used by GTK  
- **GDK-Pixbuf** (`gdk-pixbuf-2.0`) – Handles image loading and GIF animation  
- **C++ Compiler** (e.g., `g++`)  
- **pkg-config** – Finds compiler flags and library paths  
````

**Debian/Ubuntu:**
```bash
sudo apt update
sudo apt install libgtk-3-dev libglib2.0-dev libgdk-pixbuf2.0-dev g++ pkg-config
````

**Fedora:**

```bash
sudo dnf install gtk3-devel glib2-devel gdk-pixbuf2-devel gcc-c++ pkgconf-pkg-config
```

**Arch Linux:**

```bash
sudo pacman -S gtk3 glib2 gdk-pixbuf2 base-devel
```

---

### Compilation (optional)

If you prefer compiling yourself, run:

```bash
g++ -std=c++17 GIF_Player.cpp -o GIF_Player `pkg-config --cflags --libs gtk+-3.0 gdk-pixbuf-2.0`
```

Make the binary executable:

```bash
chmod +x GIF_Player
```

---

## Demo

Run the widget with your chosen GIF. For example:

```bash
git clone https://github.com/Abibguy/Linux-GIF-Widget.git
cd Linux-GIF-Widget
chmod +x /home/$USER/Linux-GIF-Widget/GIF_Player
/home/$USER/Linux-GIF-Widget/GIF_Player /home/$USER/Linux-GIF-Widget/Kanagawa.gif
```

---

## Usage

To run with any GIF of your choice:

```bash
./GIF_Player /path/to/your/animated.gif
```

* The window will **automatically resize** to the GIF dimensions.
* Multiple instances can be launched with different GIFs.

---

## Known Limitations

* Auto-positioning may not work correctly under **Wayland**.
* May interfere with **GTK2 windows**.
* Very large GIFs can cause lag or scaling artifacts.
* Built to work best with **GNOME =>45**; tested only on GNOME as of present.
* Rounded corners require the **“Rounded Window Corners” GNOME extension**; built-in rounding is not yet implemented.
* Extremely small GIFs may appear blurry due to scaling.

---

## Contributing

Contributions are welcome! Please feel free to open an issue or submit a pull request.

This is a **student hobby project**, so feel free to copy, use, or modify — credit is appreciated ;3 but not required.
