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
    int id;

  public:
    StateNode(bool start, bool final, int id);
    bool getStart();
    bool getFinal();
    int getID();
    void setStart(bool start);
    void setFinal(bool final);
    void setID(int id);
};

#endif
