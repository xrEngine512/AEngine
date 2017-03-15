//
// Created by islam on 12.02.17.
//

#include <IO.h>
#include <X11/Xlib.h>


Display* get_display() {
    static Display *display(XOpenDisplay(NULL));
    return display;
}

Window get_window() {
    static Window window(XDefaultRootWindow(get_display()));
    return window;
}

namespace IO {
    namespace Mouse {
        PointerPosition get_pointer_position() {
            PointerPosition position;
            Window ret_root;
            Window ret_child;
            int root_x;
            int root_y;
            int win_x;
            int win_y;
            unsigned int mask;

            auto display = get_display();
            auto window = get_window();

            if(XQueryPointer(display, window, &ret_root, &ret_child, &root_x, &root_y, &win_x, &win_y, &mask))
            {
                position.x = root_x;
                position.y = root_y;
            }
            else
            {
                position.x = -1;
                position.y = -1;
            }
            XFlush(display);

            return position;
        }

        void set_pointer_position(const PointerPosition& position) {
            auto display = get_display();
            auto window = get_window();

            XSelectInput(display, window, KeyReleaseMask);

            XWarpPointer(display, None, window, 0, 0, 0, 0, position.x, position.y);

            XFlush(display);
        }

        void set_cursor_visible(bool visible) {
            Display *dpy;
            Window root_window;

            dpy = XOpenDisplay(0);
            root_window = XRootWindow(dpy, 0);

            XDefineCursor(dpy, root_window, visible ? 1 : None);
        }
    }

    namespace GUI {
        void set_input_focus(WindowHandle handle) {
            XSetInputFocus(get_display(), handle, 0, None);
        }
    }
}
