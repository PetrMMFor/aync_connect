#include "CHttpConnect.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/core/detail/type_traits.hpp>
#include <boost/asio/buffer.hpp>
#include <iostream>
#include <boost/regex.hpp>
#include <utility>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "CSession.h"

CHttpConnect::CHttpConnect() {
    _host = "data.fixer.io";
    _port = "80";
    _target = "/api/latest?access_key=******&format=1";
    _version = 11;
}
CHttpConnect::CHttpConnect(std::string url) {
    //Проверяем не пустой ли url адрес
    if(!url.empty()) {
        //Проверка на правильность url адреса с помощью регулярных выражений

        boost::regex url_pattern("^(?:((?:https?|s?ftp):)\\/\\/)?([^:\\/\\s]+)(?::(\\d*))?(?:\\/([^\\s?#]+)?([?][^?#]*)?(#.*)?)?");
        if(boost::regex_match(url.begin(),url.end(),url_pattern)) {
            //удаляем из строки начало url если таковое имеется "http://" или
            // "https://"
            if (url.find("http://") != std::string::npos) {
                url.erase(url.find("http://"), 7);
            } else if (url.find("https://") != std::string::npos) {
                url.erase(url.find("https://"), 8);
            }
            //Разделяем входной url адрес на состовляющие
            auto port_start_position = url.find(':');
            auto target_start_position = url.find('/');
            if (port_start_position != std::string::npos && target_start_position != std::string::npos) {
                _host = url.substr(0, port_start_position);
                _port = url.substr(port_start_position + 1, target_start_position - (port_start_position + 1));
                _target = url.substr(target_start_position);
            } else if (port_start_position == std::string::npos && target_start_position != std::string::npos) {
                _host = url.substr(0, target_start_position);
                _port = "80";
                _target = url.substr(target_start_position);
            } else if (port_start_position != std::string::npos) {
                _host = url.substr(0, port_start_position);
                _port = url.substr(port_start_position + 1);
                _target = "/";
            } else {
                _host = url.substr(0);
                _port = "80";
                _target = "/";
            }
        } else {
            std::cout << "Bad Url address" << std::endl;
        }
    } else  {
        std::cout << "Empty Url address" << std::endl;
    }
}
void CHttpConnect::http_connection() {
    using tcp = boost::asio::ip::tcp;
    namespace http = boost::beast::http;

    boost::asio::io_context ioc;
    tcp::resolver resolver{ioc};
    tcp::socket socket{ioc};

    auto const results = resolver.resolve(_host, _port);
    boost::asio::connect(socket, results.begin(), results.end());

    http::request<http::string_body> req{http::verb::get, _target, _version};
    req.set(http::field::host, _host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);
    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);

    socket.shutdown(tcp::socket::shutdown_both);
    _date = res.at("Date").to_string();
    _json = boost::beast::buffers_to_string(res.body().data());
}
std::string CHttpConnect::get_host() {
    return _host;
}
std::string CHttpConnect::get_port(){
    return _port;
}
std::string CHttpConnect::get_target(){
    return _target;
}
std::string CHttpConnect::get_url() {
    std::string url;
    url + _host + _port + _target;
    return url;
}
std::string CHttpConnect::get_date() {
    return _date;
}
std::string CHttpConnect::get_json() {
    return _json;
}
void CHttpConnect::asynchronous_http_connection()
{
    boost::asio::io_context ioc;
    std::make_shared<CSession>(CSession(ioc, *this))->run(_host.c_str(), _port.c_str(), _target.c_str(), _version);
    ioc.run();
}
void CHttpConnect::write_json(std::string str_json)
{
    _json = std::move(str_json);
}
void CHttpConnect::write_date(std::string str_date)
{
    _date = std::move(str_date);
}
void CHttpConnect::print_rates()
{
    rapidjson::Document document;
    document.Parse(_json.c_str());

    for (auto itr = document["rates"].MemberBegin();
         itr != document["rates"].MemberEnd(); ++itr)
    {
        std::cout << "\"" << itr->name.GetString() << "\" = " << itr->value.GetDouble() << std::endl;
    }
}
void CHttpConnect::print_rates(std::vector<std::string> currency_cfg)
{
    rapidjson::Document document;
    document.Parse(_json.c_str());

    auto begin = currency_cfg.begin();
    auto end = currency_cfg.end();
    while (begin != end) {
        if(document["rates"].HasMember(begin->c_str())) {
            std::cout << "\""<< *begin << "\" = " << document["rates"][begin++->c_str()].GetDouble() << std::endl;
        }else
        {
            std::cout << "Error \"" <<*begin++ << "\" is not valid currency" << std::endl;
        }
    }
}
