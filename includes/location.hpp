#ifndef LOCATION_H
#define LOCATION_H

#include <cctype>
#include <filesystem>
#include <memory>

namespace W {
    struct Location {
        static inline Location merge(const Location& left, const Location& right);

        inline void extend_to_right(const Location& other);

        std::shared_ptr<std::filesystem::path> file_path;

        std::size_t start_line;
        std::size_t start_col;
        std::size_t end_line;
        std::size_t end_col;
    };
}

#include <location.inl>

#endif
