//Kevin Shannon
//State Node method definitions
#include "StateNode.h"

StateNode::StateNode(bool start, bool final) {
  this->start = start;
  this->final = final;
}

StateNode::~StateNode() {
  // :)
}

bool StateNode::getStart() {
  return start;
}

bool StateNode::getFinal() {
  return final;
}

void StateNode::setStart(bool start) {
  this->start = start;
}

void StateNode::setFinal(bool final) {
  this->final = final;
}

void StateNode::addTransition(char c, StateNode* dest) {
  if(Successor(c) == NULL) {
    for(std::vector<std::pair<std::vector<char>, StateNode*> >::iterator i1 = transitons.begin(); i1 != transitons.end(); ++i1) {
      if(i1->second == dest) {
        i1->first.push_back(c);
        return;
      }
    }
    std::vector<char> v;
    v.push_back(c);
    transitons.push_back(std::make_pair(v, dest));
  }
}

void StateNode::removeTransition(char c, StateNode* dest) {
  for(std::vector<std::pair<std::vector<char>, StateNode*> >::iterator i1 = transitons.begin(); i1 != transitons.end(); ++i1) {
    for(std::vector<char>::iterator i2 = i1->first.begin(); i2 != i1->first.end(); i2++) {
      if(c == *i2)
        i1->first.erase(i2);
      if(i1->first.empty())
        transitons.erase(i1);
    }
  }
}

void StateNode::removeAllTransitions() {
  transitons.clear();
}

StateNode* StateNode::Successor(char c) {
  for(std::vector<std::pair<std::vector<char>, StateNode*> >::iterator i1 = transitons.begin(); i1 != transitons.end(); ++i1) {
    for(std::vector<char>::iterator i2 = i1->first.begin(); i2 != i1->first.end(); i2++) {
      if(c == *i2)
        return i1->second;
    }
  }
  return NULL;
}
