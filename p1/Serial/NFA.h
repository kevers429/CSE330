//Kevin Shannon
//NFA Data Structure

#ifndef NFA_H
#define NFA_H

#include "StateNode.h"
#include <string>

class NFA {
  protected:
    StateNode* initState;
    StateNode* finalState;
    bool isInAlphabet(char c);
    NFA* buildSingle(char c);
    NFA* Union(NFA* a, NFA* b);
    NFA* Cat(NFA* a, NFA* b);
    NFA* Star(NFA* a);
  public:
    NFA();
    NFA(std::string regex);
    ~NFA();
};


#endif
