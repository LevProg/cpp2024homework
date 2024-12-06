#include "Subject.h"
#include <cctype>

std::string Subject::replace(const ArgsMap& args) {
    const std::string& text = args.at("text");
    const std::string& old_word = args.at("old_word");
    const std::string& new_word = args.at("new_word");

    std::string result = text;
    size_t pos = 0;
    while ((pos = result.find(old_word, pos)) != std::string::npos) {
        result.replace(pos, old_word.length(), new_word);
        pos += new_word.length();
    }
    return result;
}

std::string Subject::find(const ArgsMap& args) {
    const std::string& text = args.at("text");
    const std::string& phrase = args.at("phrase");

    return text.find(phrase) != std::string::npos ? "Found "+ phrase+ "\n"+text : "Not Found";
}

std::string Subject::capitalize_words(const ArgsMap& args) {
    const std::string& text = args.at("text");

    std::string result;
    bool capitalize = true;
    for (char c : text) {
        if (capitalize && std::isalpha(c)) {
            result += std::toupper(c);
            capitalize = false;
        }
        else {
            result += c;
        }
        if (std::isspace(c)) capitalize = true;
    }
    return result;
}

std::string Subject::number_sentences(const ArgsMap& args) {
    const std::string& text = args.at("text");

    std::string result;
    size_t sentence_count = 1;
    bool new_sentence = true;
    bool new_line = true;
    for (char c : text) {
        if (new_sentence && std::isalpha(c)) {
            if (!new_line) result += '\n';
            result += std::to_string(sentence_count++) + ". ";
            new_sentence = false;
            new_line = false;
        }
        result += c;
        if (c == '.' || c == '?' || c == '!') {
            new_sentence = true;
        }
        if (c == '\n') {
            new_line = true;
        }
    }
    return result;
}

std::string Subject::capitalize_first_letter_of_sentence(const ArgsMap& args) {
    const std::string& text = args.at("text");
    std::string result;
    bool new_sentence = true;

    for (char c : text) {
        if (new_sentence && std::isalpha(c)) {
            result += std::toupper(c);
            new_sentence = false;
        }
        else {
            result += c;
        }
        if (c == '.' || c == '?' || c == '!') {
            new_sentence = true;
        }
    }
    return result;
}