/*
 * implemented by Bartek 'BaSz' Szurgot (http://www.baszerr.eu)
 *
 * note: parts of network IO have been taken from boost::asio examples.
 *
 */
#include <fstream>
#include <sstream>
#include <cassert>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Util/XMLConfiguration.h>

#include "AnuRandom.hpp"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


//
// helper classes
//
namespace
{

void readHtml(const string &host, const string &path, stringstream &ss)
{
  try
  {
    HTTPClientSession s{host};
    HTTPRequest       request{HTTPRequest::HTTP_GET, path};
    s.sendRequest(request);

    HTTPResponse response;
    istream&     is = s.receiveResponse(response);
    if( response.getStatus() != HTTPResponse::HTTP_OK )
    {
      stringstream err;
      err << "invalid HTTP response from '" << host << "', while requesting '"
          << path << "' - status " << response.getStatus() << ": " << response.getReason();
      throw AnuRandom::ExceptionIO{ err.str() };
    }
    Poco::StreamCopier::copyStream(is, ss);
  }
  catch(const AnuRandom::ExceptionIO &ex)
  {
    throw ex;
  }
  catch(const Poco::Exception &ex)
  {
    throw AnuRandom::ExceptionIO{string{"I/O error: "} + ex.displayText()};
  }
  catch(const std::exception &ex)
  {
    throw AnuRandom::ExceptionIO{string{"I/O error: "} + ex.what()};
  }
} // readHtml()


struct HtmlReader: public XMLConfiguration
{
  explicit HtmlReader(istream &is):
    XMLConfiguration{is}
  {
  }
};


void parseFromHtml(stringstream &html, AnuRandom::Data &out)
{
  const string location="body.div.table.tr.td";
  try
  {
    HtmlReader   xml{html};
    const string str=xml.getString(location);
    copy( str.begin(), str.end(), back_insert_iterator<AnuRandom::Data>{out} );
  }
  catch(const std::exception &ex)
  {
    throw AnuRandom::ExceptionParseError{"unable to parse '" + location + "' from input: " + ex.what()};
  }
} // parseFromHtml()

} // unnamed namespace


//
// API's PIMPL implementation
//

class AnuRandom::Impl
{
public:
  Impl(string&& host, string&& path):
    host_{ move(host) },
    path_{ move(path) }
  {
    if(path_.empty() || path_[0]!='/')
      throw ExceptionParameterError{ "invalid path '" + path_ + "' - must start with '/'" };
  }

  void read(Data &out)
  {
    stringstream html;
    readHtml(host_, path_, html);
{                                                               
string tmp=html.str();                                                  
copy( tmp.begin(), tmp.end(), back_insert_iterator<Data>{out} );            
}                                                                       
    //parseHtml( move(html), out );
    parseFromHtml(html, out);
  }

private:
  const string host_;
  const string path_;
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
