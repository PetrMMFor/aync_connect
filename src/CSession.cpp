#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "CSession.h"
#include "CHttpConnect.h"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

// Report a failure
void fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}
CSession::CSession(boost::asio::io_context& ioc, CHttpConnect& http_tmp) : _resolver(ioc), _socket(ioc), _http_json_ptr(http_tmp) {}
void CSession::run( char const* host,  char const* port,  char const* target, int version)
{
    // Set up an HTTP GET request message
    _req.version(version);
    _req.method(http::verb::get);
    _req.target(target);
    _req.set(http::field::host, host);
    _req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    auto self = shared_from_this();
    _resolver.async_resolve( host, port, [this, self](boost::system::error_code ec, tcp::resolver::results_type results){
                on_resolve(ec,results);
            });
}

void CSession::on_resolve(boost::system::error_code ec, tcp::resolver::results_type results)
{
    if(ec)
        return fail(ec, "resolve");

    // Make the connection on the IP address we get from a lookup
    auto self = shared_from_this();
    boost::asio::async_connect(_socket, results.begin(), results.end(), [this, self](boost::system::error_code ec, const tcp::endpoint& endpoint){
        on_connect(ec);
    });
}

void CSession::on_connect(boost::system::error_code ec)
{
    if(ec)
        return fail(ec, "connect");

    // Send the HTTP request to the remote host
    auto self = shared_from_this();
    http::async_write(_socket, _req, [this, self](boost::system::error_code ec, std::size_t bytes_transferred){
        on_write(ec, bytes_transferred);
    });
}

void CSession::on_write(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    // Receive the HTTP response
    auto self = shared_from_this();
    http::async_read(_socket, _buffer, _res,[this,self](boost::system::error_code ec,std::size_t bytes_transferred){
        on_read(ec,bytes_transferred);
    });
}

void CSession::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec) {
        return fail(ec, "read");
    }

    // Write the message to standard out
    _http_json_ptr.write_json(_res.body().data());
    _http_json_ptr.write_date(_res.at("Date").to_string());
    //std::cout << _res.body() << std::endl;

    // Gracefully close the socket
    _socket.shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes so don't bother reporting it.
    if(ec && ec != boost::system::errc::not_connected)
        return fail(ec, "shutdown");

    // If we get here then the connection is closed gracefully
}
std::string CSession::get_res()
{
    return _res.body();
}
std::string CSession::get_date()
{
    return _res.at("Date").to_string();
}

