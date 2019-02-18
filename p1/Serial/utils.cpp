//Kevin Shannon
//to make life esaier :)

#include "utils.h"

bool isStateInVector(StateNode* s, std::vector<StateNode*> v) {
  for(std::vector<StateNode*>::iterator i = v.begin(); i != v.end(); ++i) {
    if((*i)->getID() == s->getID())
      return true;
  }
  return false;
}

std::vector<StateNode*> Combine(std::vector<StateNode*> a, std::vector<StateNode*> b) {
  std::vector<StateNode*> result = a;
  for(std::vector<StateNode*>::iterator i = b.begin(); i != b.end(); ++i) {
    if(!isStateInVector(*i, a))
      result.push_back(*i);
  }
  return result;
}

bool isVectorInVector(std::vector<StateNode*> s, std::vector<std::vector<StateNode*> > v) {
  for(std::vector<std::vector<StateNode*> >::iterator i = v.begin(); i != v.end(); ++i) {
    if(Combine(*i, s) == *i && Combine(s, *i) == s)
      return true;
  }
  return false;
}
