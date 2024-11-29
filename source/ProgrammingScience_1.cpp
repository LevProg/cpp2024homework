#include "Parser.h"

int main() {
    try {
        setlocale(LC_ALL, "");
        std::vector<Plugin> plugins = PluginManager::loadPlugins("./plugins");

        while (true) {
            std::string expression;
            std::cout << "Введите выражение: ";
            std::getline(std::cin, expression);
            if (expression.empty()) break;

            Lexer lexer(expression);
            Parser parser(lexer, plugins);
            double result = parser.parse();

            std::cout << "Результат: " << result << std::endl;
        }

        PluginManager::unloadPlugins(plugins);
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
