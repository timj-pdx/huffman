
#include <iostream>
#include <fstream>

using namespace std;

ofstream osPtr;

int main()
{
  osPtr.open("data");
  for (unsigned char i = 1; i<128; i++)
    osPtr << i << endl;
  osPtr.close();
}
