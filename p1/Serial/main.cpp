//Kevin Shannon
//usage: ./search <search-string pattern> <file-name pattern>

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <tuple>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "NFA.h"
#include "DFA.h"

using namespace std;

string postFix(string regex);
string catExpand(string regex);

string regex;
string line;
int lineNum;

int main(int argc, char *argv[]) {
  if(argc < 3)
    return 0;
  regex = argv[1];
  regex = catExpand(regex);
  regex = postFix(regex);
  cout << regex << endl;
  NFA* nfa = new NFA(regex);
  DFA* dfa = new DFA(nfa);
  for(int i = 0; i < argc - 2; i++) {
    ifstream infile(argv[i + 2]);
    lineNum = 0;
    while(std::getline(infile, line)) {
      lineNum++;
      istringstream iss(line);
      if(!line.empty()) {
        vector<tuple<int, int, string> > matches = dfa->Compute(line);
        for(vector<tuple<int, int, string> >::iterator it = matches.begin(); it != matches.end(); ++it)
          cout << argv[i + 2] << ", " << lineNum << ", " << get<0>(*it) << ", " << get<1>(*it) << ", " << get<2>(*it) << endl;
      }
    }
  }
  return 0;
}

bool isInAlphabet(char c) {
  if((c >= 49 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 46)
    return true;
  return false;
}

bool isOperator(char c) {
  if(c == '*' || c == '-' || c == '+')
    return true;
  return false;
}

int Precedence(char c) {
  if(c == '+')
    return 1;
  else if(c == '-')
    return 2;
  else if(c == '*')
    return 3;
  return 0;
}

string catExpand(string regex) { //explictly represents concatenation with '-'
  for(int i = 1; i < regex.length(); i++) {
    if(((isInAlphabet(regex[i-1]) || regex[i-1] == '*') && (isInAlphabet(regex[i]) || regex[i] == '(')) || (regex[i-1] == ')' && (isInAlphabet(regex[i]) || regex[i] == '(')))
      regex.insert(i, 1, '-');
  }
  return regex;
}

string postFix(string regex) {
  regex = catExpand(regex);
  queue<char> outputQueue;
  stack<char> operatorStack;
  while(regex != "") {
    char token = regex[0];
    regex.erase(0,1);
    if(isInAlphabet(token) || token == '*')
      outputQueue.push(token);
    if(isOperator(token) && token != '*') {
      if(!operatorStack.empty()) {
        while(!operatorStack.empty() && Precedence(operatorStack.top()) > Precedence(token)) {
          if(isOperator(operatorStack.top())) {
            outputQueue.push(operatorStack.top());
            operatorStack.pop();
          }
        }
      }
      operatorStack.push(token);
    }
    if(token == '(')
      operatorStack.push(token);
    if(token == ')') {
      while(operatorStack.top() != '(') {
        outputQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      operatorStack.pop();
    }
  }
  while(!operatorStack.empty()) {
    if(isOperator(operatorStack.top())) {
      outputQueue.push(operatorStack.top());
      operatorStack.pop();
    }
  }
  while(!outputQueue.empty()) {
    regex += outputQueue.front();
    outputQueue.pop();
  }
  return regex;
}
