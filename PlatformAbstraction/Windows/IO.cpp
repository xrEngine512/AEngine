//
// Created by islam on 12.02.17.
//

#include <IO.h>
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>

using std::experimental::string_view;

namespace IO {
    namespace Mouse {
        PointerPosition get_pointer_position() const {
            POINT point;
            GetCursorPos(&point);
            PointerPosition position;
            position.x = point.x;
            position.y = point.y;
            return position;
        }

        void set_pointer_position(const PointerPosition& position) const {
            SetCursorPos(position.x, position.y);
        }

        void set_cursor_visible(bool visible) {
            ShowCursor(visible);
        }

        void set_input_focus(WindowHandle handle) {
            SetFocus(handle);
        }
    }
}
