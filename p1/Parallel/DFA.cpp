//Kevin Shannon
//DFA method definitions

#include "DFA.h"
#include "utils.h"

DFA::DFA() {
  this->states = std::vector<std::vector<StateNode*> > {{}};
  this->startState = std::vector<StateNode*> {};
  this->finalStates = std::vector<std::vector<StateNode*> > {{}};
}

//constructs equivalent DFA from NFA
DFA::DFA(NFA* nfa) {
  DFA* dfa = new DFA();
  startState = nfa->epsClosure(nfa->getStartState());
  states.push_back(startState);
  std::stack<std::vector<StateNode*> > newStates;
  newStates.push(startState);
  while(!newStates.empty()) { //while there are states to be explored
    std::vector<StateNode*> s = newStates.top();
    newStates.pop();
    for(std::set<char>::iterator c = alphabet.begin(); c != alphabet.end(); ++c) { //for each character in the alphabet
      std::vector<StateNode*> temp = {};
      for(std::vector<StateNode*>::iterator i = s.begin(); i != s.end(); ++i) //for each state in the powerset
        temp = Combine(temp, nfa->Move(*i, *c)); //find the states we can move to
      if(!isVectorInVector(temp, states)) {
        newStates.push(temp);
        states.push_back(temp);
        for(std::vector<StateNode*>::iterator j = temp.begin(); j != temp.end(); ++j) {
          if((*j)->getFinal()) { //check if any state is a final states
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

//walk through DFA and return all matches
std::vector<std::tuple<int, int, std::string> > DFA::Compute(std::string str) {
  std::vector<std::tuple<int, int, std::string> > matches;
  std::vector<StateNode*> current;
  #pragma omp parallel for num_threads(8) private(current)
  for(std::string::size_type j = 0; j < str.size() - 1; ++j) { //start from each character in string
    current = startState;
    #pragma omp critical
    {
      for(std::string::size_type i = j; i < str.size() && isInAlphabet(str[i]) && !(transitionTable[std::make_pair(current, str[i])].empty()); ++i) {
        current = transitionTable[std::make_pair(current, str[i])];
        //if the next transition is still in a final state keep going
        if(isVectorInVector(current, finalStates) && !isVectorInVector(transitionTable[std::make_pair(current, str[i+1])], finalStates))
          matches.push_back(std::make_tuple(j + 1, i + 1, str.substr(j, (i+1) - (j))));
      }
    }
  }
  return matches;
}
