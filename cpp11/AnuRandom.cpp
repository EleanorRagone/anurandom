/*
 * implemented by Bartek 'BaSz' Szurgot (http://www.baszerr.eu)
 *
 * note: parts of network IO have been taken from boost::asio examples.
 *
 */
#include <deque>
#include <istream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <boost/asio.hpp>

#include "AnuRandom.hpp"

using namespace std;
using boost::asio::ip::tcp;


//
// helper classes
//
namespace
{

// wrapper for network and HTTP protocol
class HttpIo
{
public:
  HttpIo(string&& host, string&& path):
    host_{ move(host) },
    path_{ move(path) }
  {
  }

  std::string readContent(void)
  {
    tcp::socket sock=connectToService();    // connect
    queryForPage(sock);                     // http/get
    return readResponse(sock);              // return the page (headers are checked, but ripped out)
  }

private:
  tcp::socket connectToService(void)
  {
    stringstream log;
    log << "call history:" << endl;
    log << "resolving '" << host_ << "'..." << endl;

    try
    {
      tcp::socket             sock(io_);
      tcp::resolver           resolver(io_);
      tcp::resolver::query    query(host_, "http");
      // iterate over all possible resolutions
      for(auto it = resolver.resolve(query); it!=tcp::resolver::iterator(); ++it)
        if( tryConnecting(sock, *it, log) )
          return sock;
    }
    catch(const std::exception &ex)
    {
      log << "fatal error: " << ex.what() << endl;
      throw AnuRandom::ExceptionIO{ log.str() };
    }

    // no working resolution found
    log << "unable to find any working resolution for host '" << host_ << "' - aborting" << endl;
    throw AnuRandom::ExceptionIO{ log.str() };
  }


  bool tryConnecting(tcp::socket&                               sock,
                     const tcp::resolver::iterator::value_type& endpoint,
                     stringstream&                              log)
  {
    try
    {
      log << "trying to connect to '" << endpoint.host_name() << "'..." << endl;
      sock.connect(endpoint);
      return true;
    }
    catch(const std::exception &ex)
    {
      log << "error: " << ex.what() << endl;
      return false;
    }
    assert(!"this code is never reached");
    return false;
  }


  void queryForPage(tcp::socket& sock)
  {
    // prepare the request
    boost::asio::streambuf req;
    ostream                reqStrm(&req);
    reqStrm << "GET " << path_ << " HTTP/1.0\r\n";  // choose proper page
    reqStrm << "Host: " << host_ << "\r\n";         // specify host
    reqStrm << "Accept: */*\r\n";                   // get all :)
    reqStrm << "Connection: close\r\n\r\n";         // we'll read until EOF
    // send it
    boost::asio::write(sock, req);
  }


  string readResponse(tcp::socket& sock)
  {
    boost::asio::streambuf response;
    boost::asio::read_until(sock, response, "\r\n"); // read the status line

    // check if response is positive
    std::istream responseStrm{&response};
    std::string  httpVersion;
    responseStrm >> httpVersion;
    unsigned int statusCode;
    responseStrm >> statusCode;
    std::string statusMessage;
    std::getline(responseStrm, statusMessage);
    if( !responseStrm || httpVersion.substr(0, 5)!="HTTP/" )
      throw AnuRandom::ExceptionIO{"unexpected response (invalid protocol?)"};
    if(statusCode!=200)
    {
      stringstream ss;
      ss << "non-OK status code received: " << statusCode;
      throw AnuRandom::ExceptionIO{ ss.str() };
    }

    // read the headers
    boost::asio::read_until(sock, response, "\r\n\r\n");

    // skip headers
    {
      std::string header;
      while( getline(responseStrm, header) && header!="\r" )
      {}
    }

    // output string
    stringstream content;

    // whatever's left write to the output buffer
    if( response.size() > 0)
      content << &response;

    // Read until EOF, writing data to output as we go.
    boost::system::error_code error;
    while( boost::asio::read(sock, response, boost::asio::transfer_at_least(1), error) )
      content << &response;
    // convert errot to some readable 
    if( error != boost::asio::error::eof )
      throw boost::system::system_error(error);

    return content.str();
  }

  string                  host_;
  string                  path_;
  boost::asio::io_service io_;
}; // HttpIo

} // unnamed namespace


//
// API's PIMPL implementation
//

class AnuRandom::Impl
{
public:
  Impl(string&& host, string&& path):
    http_{ move(host), move(path) }
  {
  }

  void read(Data &out)
  {
    std::string html=http_.readContent();
    copy( html.begin(), html.end(), back_insert_iterator<Data>{out} );
    // TODO
  }

private:
  HttpIo http_;
};



//
// API implementation
//

AnuRandom::AnuRandom(std::string host, std::string path):
  pimpl_{ new Impl{ move(host), move(path) } }
{
}


AnuRandom::~AnuRandom(void)
{
  // NOTE: this ensures proper destruction of implementation object
}


void AnuRandom::read(Data &out)
{
  pimpl_->read(out);
}
