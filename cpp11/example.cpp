#include <iostream>
#include <algorithm>

#include "AnuRandom.hpp"

using namespace std;


int main(void)
{
  //AnuRandom rnd{"www.onet.pl", "/index.html"};
  AnuRandom rnd;
  AnuRandom::Data data=rnd.read();
  cout<<"got "<<data.size()<<" bytes"<<endl;
  //for_each( data.begin(), data.end(), [](uint8_t c) { cout<<c; } );
  cout<<endl;
  return 0;
}
