#include "Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& expr) : expression(expr), pos(0) {
    currentChar = expression.empty() ? '\0' : expression[pos];
}

void Lexer::advance() {
    pos++;
    currentChar = pos < expression.size() ? expression[pos] : '\0';
}

void Lexer::skipWhitespace() {
    while (currentChar == ' ') {
        advance();
    }
}

double Lexer::getNumber() {
    std::string result;
    while (std::isdigit(currentChar) || currentChar == '.') {
        result += currentChar;
        advance();
    }
    return std::stod(result);
}

std::string Lexer::getFunction() {
    std::string result;
    while (std::isalpha(currentChar)) {
        result += currentChar;
        advance();
    }
    return result;
}

Token Lexer::getNextToken() {
    while (currentChar != '\0') {
        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }
        if (std::isdigit(currentChar)) {
            return Token{ TokenType::NUMBER, getNumber() };
        }
        if (std::isalpha(currentChar)) {
            return Token{ TokenType::FUNCTION, 0, getFunction() };
        }
        char prevChar = currentChar;
        advance();
        for (const auto token : AllOperators) {
            if (static_cast<char>(token) == prevChar) return Token{ token };
        }
        throw std::runtime_error(std::string("Неизвестный символ: ") + prevChar);
    }
    return Token{ TokenType::END };
}
