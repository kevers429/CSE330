//Kevin Shannon
//NFA Data Structure

#ifndef NFA_H
#define NFA_H

#include "StateNode.h"
#include <string>
#include <map>
#include <set>

class NFA {
  protected:
    std::vector<StateNode*> states;
    std::set<int> stateNames;
    std::set<char> alphabet = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                               'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                               '1','2','3','4','5','6','7','8','9','.','\0'};
    std::map<std::pair<StateNode*, char>, std::vector<StateNode*> > transitionTable;
    StateNode* startState;
    StateNode* finalState;
    int newID(NFA* a);
    bool isInAlphabet(char c);
    NFA* buildSingle(char c);
    void Glob(NFA* src, NFA* tar);
    NFA* Union(NFA* a, NFA* b);
    NFA* Cat(NFA* a, NFA* b);
    NFA* Star(NFA* a);
  public:
    NFA();
    void PrintTable();
    NFA(std::string regex);
    ~NFA();
};


#endif
