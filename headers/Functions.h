#ifndef FINANS_FUNCTIONS_H
#define FINANS_FUNCTIONS_H

#include <boost/beast/core/string.hpp>
#include <vector>

// Функция смотрит описанные(необходимые) валюты и url адреса в файле конфига "*.json"
// парсит их
std::pair<std::vector<std::string>, std::vector<std::string>>get_config(std::string);
// Функция самого демона
void daemon(const std::vector<std::string>);
// Функция описания инструкции для демона
void daemon_work(const std::pair<std::vector<std::string>, std::vector<std::string>>);


#endif //FINANS_FUNCTIONS_H
