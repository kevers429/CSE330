//Kevin Shannon
//State Node Data Structure

#ifndef STATENODE_H
#define STATENODE_H

#include <utility>
#include <vector>
#include <cstddef>

class StateNode {
  protected:
    bool start;
    bool final;
    std::vector<std::pair<std::vector<char>, StateNode*> > transitons;

  public:
    StateNode(bool start, bool final);
    ~StateNode();
    bool getStart();
    bool getFinal();
    void setStart(bool start);
    void setFinal(bool final);
    void addTransition(char c, StateNode* dest);
    void removeTransition(char c, StateNode* dest);
    void removeAllTransitions();
    StateNode* Successor(char c);
};

#endif
