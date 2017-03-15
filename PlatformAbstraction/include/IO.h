//
// Created by islam on 12.02.17.
//

#pragma once

namespace IO {
    namespace Mouse {
        struct PointerPosition {
            PointerPosition() = default;
            PointerPosition(int x, int y): x(x), y(y) {}
            int x, y;
        };
        PointerPosition get_pointer_position();
        void set_pointer_position(const PointerPosition&);
        void set_cursor_visible(bool visible);
    }

    namespace GUI {
        void set_input_focus(WindowHandle);
    }
}
