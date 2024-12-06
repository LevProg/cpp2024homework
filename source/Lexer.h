#include <string>
#include <iostream>
#include <algorithm>
 
enum class TokenType { NUMBER, FUNCTION, END, PLUS='+', MINUS='-', MULTIPLY='*', DIVIDE='/', POWER='^', LPAREN='(', RPAREN=')' };
using enum TokenType;

static const TokenType AllOperators[] = { PLUS,MINUS,MULTIPLY,DIVIDE,POWER,LPAREN,RPAREN };

struct Token {
    enum TokenType type;
    double value = 0; // Для чисел
    std::string funcName; // Для функций
} ;

class Lexer {
    std::string expression;
    size_t pos = 0;
    char currentChar;

    void advance();
    void skipWhitespace();
    double getNumber();
    std::string getFunction();

public:
    explicit Lexer(const std::string& expr);
    Token getNextToken();
};
