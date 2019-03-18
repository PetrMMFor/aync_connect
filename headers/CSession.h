#ifndef FINANS_CSESSION_H
#define FINANS_CSESSION_H

#include <boost/system/error_code.hpp>
#include <iostream>
#include <memory>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast/http.hpp>
#include "CHttpConnect.h"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

void fail(boost::system::error_code ec, char const* what);

// Performs an HTTP GET and prints the response
class CSession : public std::enable_shared_from_this<CSession>
{
    tcp::resolver _resolver;
    tcp::socket _socket;
    boost::beast::flat_buffer _buffer; // (Must persist between reads)
    http::request<http::empty_body> _req;
    http::response<http::string_body> _res;
    CHttpConnect& _http_json_ptr;

public:
    // Resolver and socket require an io_context
    explicit CSession(boost::asio::io_context& ioc, CHttpConnect& http_tmp);
    // Start the asynchronous operation
    void run( char const* host,  char const* port,  char const* target, int version);
    void on_resolve(boost::system::error_code ec, tcp::resolver::results_type results);
    void on_connect(boost::system::error_code ec);
    void on_write(boost::system::error_code ec, std::size_t bytes_transferred);
    void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
    std::string get_res();
    std::string get_date();
};

#endif //FINANS_CSESSION_H
