#include "Lexer.h"
#include "PluginManager.h"
#include <cmath>

class Parser {
    Lexer lexer;
    Token currentToken;
    std::vector<Plugin> plugins;

    void consume(enum class TokenType type);
    double parseUnit();
    double parsePower();
    double parseTerm();
    double parseExpr();
    double evalFunc();

public:
    Parser(Lexer lexer, const std::vector<Plugin>& plugins);
    double parse();
};
