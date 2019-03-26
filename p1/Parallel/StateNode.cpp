//Kevin Shannon
//State Node method definitions
#include "StateNode.h"

StateNode::StateNode(bool start, bool final, int id) {
  this->start = start;
  this->final = final;
  this->id = id;
}

bool StateNode::getStart() {
  return start;
}

bool StateNode::getFinal() {
  return final;
}

int StateNode::getID() {
  return id;
}

void StateNode::setStart(bool start) {
  this->start = start;
}

void StateNode::setFinal(bool final) {
  this->final = final;
}

void StateNode::setID(int id) {
  this->id = id;
}
