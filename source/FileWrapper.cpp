#include "FileWrapper.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

FileWrapper::FileWrapper(const std::string& filePath) : filePath_(filePath) {}

std::string FileWrapper::read() const {
    std::ifstream file(filePath_);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading: " + filePath_);
    }
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

void FileWrapper::write(const std::string& content) const {
    std::ofstream file(filePath_, std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filePath_);
    }
    file << content;
}

const std::string& FileWrapper::getFilePath() const {
    return filePath_;
}
