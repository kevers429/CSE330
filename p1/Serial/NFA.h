//Kevin Shannon
//NFA Data Structure

#ifndef NFA_H
#define NFA_H

#include "StateNode.h"
#include <string>

class NFA {
  protected:
    StateNode* states;
  public:
    NFA(std::string regex);
    ~NFA();
};


#endif
