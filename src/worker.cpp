#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "worker.hpp"

using boost::asio::ip::tcp;

std::string make_string(boost::asio::streambuf& streambuf)
{
    return {boost::asio::buffers_begin(streambuf.data()),
        boost::asio::buffers_end(streambuf.data())};
}

std::string Worker::work(std::string search_query) {
       
    const char* env_p = std::getenv("SEARCH_HOST");
    boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), env_p, "5000");
    tcp::resolver::iterator iterator = resolver.resolve(query);
    
    boost::system::error_code ec;
    tcp::socket s(io_service);
    s.connect(*iterator);
    
    boost::asio::write(s, boost::asio::buffer(search_query, search_query.length()));
    
    boost::asio::streambuf b;
    b.prepare(1024);
    boost::asio::read(s,b,ec);
    
    if (!ec || ec == boost::asio::error::eof) {
        // woot, no problem
    }
    return "<html><body>"+make_string(b)+"</body></html>";
}
