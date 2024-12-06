#include "Parser.h"

Parser::Parser(Lexer lexer, const std::vector<Plugin>& plugins)
    : lexer(std::move(lexer)), plugins(plugins) {
    currentToken = this->lexer.getNextToken();
}

void Parser::consume(enum class TokenType expectedType) {
    if (currentToken.type != expectedType) {
        throw std::runtime_error("Ожидался токен типа " + std::to_string(static_cast<int>(expectedType)));
    }
    currentToken = lexer.getNextToken();
}

double Parser::evalFunc() {
    std::string funcName = currentToken.funcName;
    consume(TokenType::FUNCTION);
    consume(TokenType::LPAREN);
    double arg = parseExpr();
    consume(TokenType::RPAREN);

    for (const auto& plugin : plugins) {
        auto result = plugin.function(funcName.c_str(), arg);
        if (result.first) {
            return result.second;
        }
    }

    throw std::runtime_error("Функция " + funcName + " не найдена в плагинах");
}

double Parser::parseUnit() {
    if (currentToken.type == TokenType::NUMBER) {
        double value = currentToken.value;
        consume(TokenType::NUMBER);
        return value;
    }

    if (currentToken.type == TokenType::FUNCTION) {
        return evalFunc();
    }

    if (currentToken.type == TokenType::LPAREN) {
        consume(TokenType::LPAREN);
        double value = parseExpr();
        consume(TokenType::RPAREN);
        return value;
    }

    throw std::runtime_error("Недопустимый токен в parseUnit");
}

double Parser::parsePower() {
    double base = parseUnit();
    while (currentToken.type == TokenType::POWER) {
        consume(TokenType::POWER);
        base = std::pow(base, parseUnit());
    }
    return base;
}

double Parser::parseTerm() {
    double result = parsePower();
    while (currentToken.type == TokenType::MULTIPLY || currentToken.type == TokenType::DIVIDE) {
        auto tokenType = currentToken.type;
        consume(tokenType);
        if (tokenType == TokenType::MULTIPLY) {
            result *= parsePower();
        }
        else{
            double divisor = parsePower();
            if (divisor == 0) {
                throw std::runtime_error("Деление на ноль");
            }
            result /= divisor;
        }
    }
    return result;
}

double Parser::parseExpr() {
    double result = parseTerm();
    while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
        auto tokenType = currentToken.type;
        consume(tokenType);
        if (tokenType == TokenType::PLUS) {
            result += parseTerm();
        }
        else{
            result -= parseTerm();
        }
    }
    return result;
}

double Parser::parse() {
    return parseExpr();
}
