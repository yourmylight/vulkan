#pragma once

#include <vector>
#include <string>

namespace utils {
    class FileUtils {
    public:
        static std::vector<char> readFile(const std::string& filename);
    };
}