//Kevin Shannon
//DFA method definitions

#include "DFA.h"
#include "utils.h"

DFA::DFA() {
  this->states = std::vector<std::vector<StateNode*> > {{}};
  this->startState = std::vector<StateNode*> {};
  this->finalStates = std::vector<std::vector<StateNode*> > {{}};
}

DFA::DFA(NFA* nfa) {
  DFA* dfa = new DFA();
  startState = nfa->epsClosure(nfa->getStartState());
  states.push_back(startState);
  std::stack<std::vector<StateNode*> > newStates;
  newStates.push(startState);
  while(!newStates.empty()) {
    std::vector<StateNode*> s = newStates.top();
    newStates.pop();
    for(std::set<char>::iterator c = alphabet.begin(); c != alphabet.end(); ++c) {
      std::vector<StateNode*> temp = {};
      for(std::vector<StateNode*>::iterator i = s.begin(); i != s.end(); ++i) {
        temp = Combine(temp, nfa->Move(*i, *c));
      }
      if(!isVectorInVector(temp, states)) {
        newStates.push(temp);
        states.push_back(temp);
        for(std::vector<StateNode*>::iterator j = temp.begin(); j != temp.end(); ++j) {
          if((*j)->getFinal()) {
            finalStates.push_back(temp);
            break;
          }
        }
      }
      transitionTable.insert(std::make_pair(std::make_pair(s, *c), temp));
    }
  }
}

bool DFA::isInAlphabet(char c) {
  return alphabet.count(c);
}

std::vector<std::tuple<int, int, std::string> > DFA::Compute(std::string str) {
  std::vector<std::tuple<int, int, std::string> > matches;
  for(std::string::size_type j = 0; j < str.size() - 1; ++j) {
    std::vector<StateNode*> current = startState;
    for(std::string::size_type i = j; i < str.size(); ++i) {
      if(isInAlphabet(str[i]) && !(transitionTable[std::make_pair(current, str[i])].empty()))
        current = transitionTable[std::make_pair(current, str[i])];
      else
        break;
      if(isVectorInVector(current, finalStates) && !isVectorInVector(transitionTable[std::make_pair(current, str[i+1])], finalStates)) {
        matches.push_back(std::make_tuple(j + 1, i + 1, str.substr(j, (i+1) - (j))));
        break;
      }
    }
  }
  return matches;
}
