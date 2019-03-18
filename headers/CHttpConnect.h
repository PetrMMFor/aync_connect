//
// Created by petr on 25.06.18.
//

#ifndef FINANS_CHTTPCONNECT_H
#define FINANS_CHTTPCONNECT_H

#include <string>
#include <utility>
#include <vector>

class CHttpConnect {
private:
    std::string _host;
    std::string _port;
    std::string _target;
    int _version = 10;
    std::string _date;
    std::string _json;
public:
    // Констуктор заполняет поля _host _port _target
    CHttpConnect();
    CHttpConnect(std::string);
    // Осуществляется коннект к адресу и результат записывается в _date и _json
    // Без Html мусора
    void http_connection();
    void http_connection(std::vector<std::string> currency_cfg);
    // Методы для асинхронного конекта
    void asynchronous_http_connection();
    void asynchronous_http_connection(std::vector<std::string> currency_cfg);
    //Возврат всех параметров для соединения
    std::string get_url();
    std::string get_host();
    std::string get_port();
    std::string get_target();
    //Возврат даты установления соединения и json файла
    std::string get_date();
    std::string get_json();
    // Вывод на экран всей валюты или в соответствии с конфигом
    void print_rates();
    void print_rates(std::vector<std::string> currency_cfg);
    //Методы необходимы что бы класс CSession мог достучатся до нужных приватных полей класса
    void write_json(std::string);
    void write_date(std::string);
};

#endif //FINANS_CHTTPCONNECT_H
