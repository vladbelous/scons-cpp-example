#include <iostream>

#include <hello_world/HelloHelper.h>
#include <util/Util.h>

using namespace std;

int main() {
  cout << "Hello world\n";
  printLine("Hello using HelloHelper");
  printLine(getGreeting1()); // from Util.h
  printLine(getGreeting2()); // from Util.h
}
