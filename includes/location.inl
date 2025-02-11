#include <cassert>

namespace W {
    inline Location Location::merge(const Location& left, const Location& right) {
        assert(left.file_path == right.file_path);
        assert(left.start_line <= right.end_line);
        assert(left.start_line < right.end_line || left.start_col <= right.end_col);

        Location new_loc;
        new_loc.file_path = left.file_path;
        new_loc.start_line = left.start_line;
        new_loc.start_col = left.start_col;
        new_loc.end_line = right.end_line;
        new_loc.end_col = right.end_col;

        return new_loc;
    }

    inline void Location::extend_to_right(const Location& right_location) {
        assert(file_path == right_location.file_path);
        assert(right_location.end_line >= start_line);
        end_line = right_location.end_line;
        assert(right_location.end_line > start_line || right_location.end_col >= start_col);
        end_col = right_location.end_col;
    }
}
