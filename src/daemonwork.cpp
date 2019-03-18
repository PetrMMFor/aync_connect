#include "Functions.h"
#include "CHttpConnect.h"
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

void daemon_work(std::pair<std::vector<std::string>, std::vector<std::string>>  config)
{
    // Создается массив из классов сколько url адресов в конфиге
    // all_daemon_work строит задание для демона на основе конфига
    // в который входит адрес для коннекта и список валют
    // список валют может быть пусты находятся config.first
    // список url пустым быть не может находятся в config.second
    // Если список валют в config.first пуст то рассматривается весь список валют
    // если есть то только те которые там описаны
    std::vector<CHttpConnect> all_daemon_work;

    for(auto i = 0; i < config.second.size();++i) {
        all_daemon_work.emplace_back(config.second[i]);
        all_daemon_work[i].get_json_from_http();
        //config.first.empty() ? all_daemon_work[i].get_json_from_http() : all_daemon_work[i].get_json_from_http(config.first);
        std::cout << all_daemon_work[i].get_date() << std::endl;
        sleep(10);
        //std::cout << all_daemon_work[i].get_json() << std::endl;
    }
}



