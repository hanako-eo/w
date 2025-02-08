#ifndef LOCATION_H
#define LOCATION_H

#include <cctype>
#include <filesystem>
#include <memory>

namespace W {
    struct Location {
        std::shared_ptr<std::filesystem::path> file_path;
        std::size_t line;
        std::size_t col;
    };
}

#endif
