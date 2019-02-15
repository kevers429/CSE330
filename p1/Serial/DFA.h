//Kevin Shannon
//DFA Data Structure

#ifndef DFA_H
#define DFA_H

#include "StateNode.h"
#include <string>
#include <map>
#include <set>

class DFA {
  protected:
    std::vector<std::vector<StateNode*> > states;
    std::set<char> alphabet = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                               'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                               '1','2','3','4','5','6','7','8','9','.'};
    std::map<std::pair<std::vector<StateNode*>, char>, std::vector<StateNode*> > transitionTable;
    StateNode* startState;
    std::vector<StateNode*> finalStates;
    bool isInAlphabet(char c);

  public:
    DFA();
    DFA(NFA* nfa);
    ~DFA();
};


#endif
