#include "Functions.h"
#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <iostream>
#include <sstream>

std::pair<std::vector<std::string>, std::vector<std::string>> get_config(std::string config_name)
{
    std::pair<std::vector<std::string>, std::vector<std::string>> json_cfg;

    // Открывается файл на чтение и копируется в строку
    // и парсится как json файл
    // Если файла нет то кидается исключение

    //Нужно кинуть исключение если файл пуст

    std::ifstream in_cfg(config_name);
    rapidjson::Document document;
    if(in_cfg) {
        std::stringstream tmp;
        std::string tmp1;
        tmp << in_cfg.rdbuf();
        tmp1 = tmp.str();
        document.Parse(tmp1.c_str());
        in_cfg.close();
    } else{
        std::cout << "No match config *.json" << std::endl;
    }


    const rapidjson::Value& rates = document["rates"];
    assert(rates.IsArray());
    if(document.HasMember("rates")) {
        for (rapidjson::SizeType i = 0; i < rates.Size(); ++i) {
            if(rates[i].IsString()) {
                json_cfg.first.emplace_back(rates[i].GetString());
            }
        }
    }

    // Если нет url адресов нужно кидать исключение и сразу завершать работу демона
    const rapidjson::Value& url = document["url"];
    assert(url.IsArray());
    if(document.HasMember("url")) {
        for (rapidjson::SizeType i = 0; i < url.Size(); ++i) {
            if(url[i].IsString()) {
                json_cfg.second.emplace_back(url[i].GetString());
            }
        }
    }

    // Проверка на наличие url адресов
    // Если их нет то работа демона прикращается с ошибкой

    if(json_cfg.second.empty())
    {
        std::cout << "Error no url address in cfg" << std::endl;
        std::cout << "Work can not start" << std::endl;
        exit(1);
    }

    return json_cfg;
}

