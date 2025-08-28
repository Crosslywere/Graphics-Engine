#include <utils.h>

const std::string asAbsolutePath(const std::string& path) {
    if (
#ifdef _WIN32
    path.at(1) == ':'
#else
    path.at(0) == '/' || path.at(0) == '~'
#endif
    ) {
        return path;
    } else {
        return std::string(ROOT_DIR + path);
    }
}