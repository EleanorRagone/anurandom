#include <iostream>
#include <algorithm>

#include "AnuRandom.hpp"

using namespace std;


int main(void)
{
  AnuRandom rnd("www.onet.pl", "/index.html");
  //AnuRandom rnd("213.180.141.140", "/index.html");
  //AnuRandom rnd("127.0.0.1");
  AnuRandom::Data data=rnd.read();
  cout<<"got "<<data.size()<<" bytes"<<endl;
  for_each( data.begin(), data.end(), [](uint8_t c) { cout<<c; } );
  cout<<endl;
  return 0;
}
