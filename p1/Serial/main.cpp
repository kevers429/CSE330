//Kevin Shannon
//usage: ./search <search-string pattern> <file-name pattern>

#include <iostream>
#include <string>
#include <iomanip>
#include "NFA.h"

using namespace std;

string regex;
string* files;

int main(int argc, char *argv[]) {
  if(argc < 3)
    return 0;
  regex = argv[1];
  files = new string[argc - 1];
  for(int i = 0; i < argc - 2; i++)
    files[i] = argv[i + 2];
  NFA* nfa = new NFA(regex);
  return 0;
}
