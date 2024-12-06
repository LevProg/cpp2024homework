#pragma once
#include <string>

class FileWrapper {
public:
    explicit FileWrapper(const std::string& filePath);

    std::string read() const;
    void write(const std::string& content) const;
    const std::string& getFilePath() const;

private:
    std::string filePath_;
};

