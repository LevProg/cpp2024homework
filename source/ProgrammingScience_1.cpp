#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <stdexcept>
#include <cctype>

using PluginFunction = std::pair<bool, double>(*)(const char*, double);

struct Plugin {
    HMODULE library;
    PluginFunction function;
};

std::vector<Plugin> loadPlugins(const std::string& directory) {
    std::vector<Plugin> plugins;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".dll") {
            HMODULE lib = LoadLibraryA(entry.path().string().c_str());
            if (lib) {
                PluginFunction func = (PluginFunction)GetProcAddress(lib, "pluginFunc");
                if (func) {
                    plugins.push_back({ lib, func });
                }
                else {
                    std::cerr << "Ошибка: функция pluginFunc не найдена в " << entry.path().string() << std::endl;
                    FreeLibrary(lib);
                }
            }
            else {
                std::cerr << "Ошибка загрузки DLL: " << entry.path().string() << std::endl;
            }
        }
    }
    return plugins;
}

void unloadPlugins(std::vector<Plugin>& plugins) {
    for (auto& plugin : plugins) {
        FreeLibrary(plugin.library);
    }
}

enum TokenType { NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, POWER, LPAREN, RPAREN, FUNCTION, END };

struct Token {
    enum TokenType type;
    double value;  // Для чисел
    std::string funcName;  // Для функций
};
class Lexer {
    std::string expression;
    size_t pos = 0;
    char currentChar;
public:
    Lexer(const std::string& expr) : expression(expr), pos(0) {
        currentChar = expression[pos];
    }

    // Функция для продвижения к следующему символу
    void advance() {
        pos++;
        if (pos < expression.length()) {
            currentChar = expression[pos];
        }
        else {
            currentChar = '\0';
        }
    }
    void skipWhitespace() {
        while (currentChar == ' ') {
            advance();
        }
    }
    double getNumber() {
        std::string result;
        while (std::isdigit(currentChar) || currentChar == '.') {
            result += currentChar;
            advance();
        }
        return std::stod(result);
    }
    std::string getFunction() {
        std::string result;
        while (std::isalpha(currentChar)) {
            result += currentChar;
            advance();
        }
        return result;
    }
    Token getNextToken() {
        while (currentChar != '\0') {
            if (std::isspace(currentChar)) {
                skipWhitespace();
                continue;
            }
            if (std::isdigit(currentChar)) {
                return Token{ NUMBER, getNumber() };
            }
            if (std::isalpha(currentChar)) {
                return Token{ FUNCTION, 0, getFunction() };
            }
            auto prevChar = currentChar;
            advance();
            if (prevChar == '+') return Token{ PLUS };
            if (prevChar == '-') return Token{ MINUS };
            if (prevChar == '*') return Token{ MULTIPLY };
            if (prevChar == '/') return Token{ DIVIDE };
            if (prevChar == '^') return Token{ POWER };
            if (prevChar == '(') return Token{ LPAREN };
            if (prevChar == ')') return Token{ RPAREN };
            std::cerr << "Неизвестный символ " << prevChar << std::endl;
            exit(-1);
        }

        return Token{ END };
    }
};

class Parser {
    Lexer lexer;
    Token currentToken;
    std::vector<Plugin> plugins;

    void error() {
        std::cerr << "Ошибка синтаксиса" << std::endl;
        exit(-1);
    }
    // Продвижение к следующему токену
    void eat(enum TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        }
        else {
            error();
        }
    }

    double factor() {
        Token token = currentToken;

        if (token.type == NUMBER) {
            eat(NUMBER);
            return token.value;
        }
        else if (token.type == FUNCTION) {
            std::string funcName = token.funcName;
            eat(FUNCTION);
            eat(LPAREN);
            double result = expr();  // Рекурсивно вычисляем аргумент функции
            eat(RPAREN);


            for (const auto& plugin : plugins) {
                auto funcResult = plugin.function(funcName.c_str(), result);
                if (funcResult.first) {
                    double res = funcResult.second;
                    if (res != res) {// если функция вернула nan (к примеру ln(-100) или acos(5))
                        std::cerr << "Аргумент функции: " << funcName << " находится в недопустимых значениях" << std::endl;
                        exit(1);
                    }
                    return res;
                }
            }
        }
        else if (token.type == LPAREN) {
            eat(LPAREN);
            double result = expr();
            eat(RPAREN);
            return result;
        }
        error();
        return 0;
    }
    double power() {
        double result = factor();
        while (currentToken.type == POWER) {
            eat(POWER);
            result = std::pow(result, factor());
        }
        return result;
    }

    double term() {
        double result = power();

        while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
            Token token = currentToken;
            if (token.type == MULTIPLY) {
                eat(MULTIPLY);
                result *= power();
            }
            else if (token.type == DIVIDE) {
                eat(DIVIDE);
                result /= power();
            }
        }

        return result;
    }

    double expr() {
        double result = term();

        while (currentToken.type == PLUS || currentToken.type == MINUS) {
            Token token = currentToken;
            if (token.type == PLUS) {
                eat(PLUS);
                result += term();
            }
            else if (token.type == MINUS) {
                eat(MINUS);
                result -= term();
            }
        }
        return result;
    }

public:
    Parser(Lexer lexer, const std::vector<Plugin>& plugins) : lexer(lexer), plugins(plugins) {
        currentToken = this->lexer.getNextToken();
    }

    double parse() {
        return expr();
    }
};

int main() {
    setlocale(LC_ALL, "");

    std::vector<Plugin> plugins = loadPlugins("./plugins");

    while (true) {
        std::string expression;
        std::cout << "Введите выражнение: ";
        std::getline(std::cin, expression);
        Lexer lexer(expression);
        Parser parser(lexer, plugins);

        double result = parser.parse();
        std::cout << "Результат: " << result << std::endl;

    }
    unloadPlugins(plugins);

    return 0;
}
