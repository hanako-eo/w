#include <location.hpp>

auto fmt::formatter<W::Location>::format(W::Location location, format_context& ctx) const -> format_context::iterator {
    return formatter<std::string>::format(fmt::format("{}:{}:{}", location.file_path->c_str(), location.start_line, location.start_col), ctx);
}
