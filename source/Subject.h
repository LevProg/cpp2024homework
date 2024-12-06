#pragma once
#include <string>
#include <unordered_map>

class Subject {
public:
    using ArgsMap = std::unordered_map<std::string, std::string>;

    std::string replace(const ArgsMap& args);
    std::string find(const ArgsMap& args);
    std::string capitalize_words(const ArgsMap& args);
    std::string number_sentences(const ArgsMap& args);
    std::string capitalize_first_letter_of_sentence(const ArgsMap& args);
};
