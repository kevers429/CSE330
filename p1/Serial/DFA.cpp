//Kevin Shannon
//DFA method definitions

#include "DFA.h"

NFA::DFA() {
  this->states = std::vector<StateNode*> {};
  this->startState = NULL;
  this->finalState = NULL;
}

NFA::DFA(NFA* nfa) {
  DFA* dfa = new DFA();
}
