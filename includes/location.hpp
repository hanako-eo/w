#ifndef LOCATION_H
#define LOCATION_H

#include <filesystem>
#include <cctype>

namespace W {
    struct Location {
        const std::filesystem::path& file_path;
        std::size_t line;
        std::size_t col;
    };
}

#endif
