//Kevin Shannon
//NFA method definitions

#include "NFA.h"
#include <stack>

NFA::NFA() {
  this->initState = NULL;
  this->finalState = NULL;
}

NFA::NFA(std::string regex) {
  std::stack<NFA*> nfaStack;
  NFA* a = new NFA();
  NFA* b = new NFA();
  while(regex != "") {
    char token = regex[0];
    regex.erase(0,1);
    if(isInAlphabet(token))
      nfaStack.push(buildSingle(token));
    else if(token == '+') {
      a = nfaStack.top();
      nfaStack.pop();
      b = nfaStack.top();
      nfaStack.pop();
      nfaStack.push(Union(a, b));
    }
    else if(token == '-') {
      a = nfaStack.top();
      nfaStack.pop();
      b = nfaStack.top();
      nfaStack.pop();
      nfaStack.push(Cat(b, a));
    }
    else if(token == '*') {
      a = nfaStack.top();
      nfaStack.pop();
      nfaStack.push(Star(a));
    }
  }
  initState = nfaStack.top()->initState;
  finalState = nfaStack.top()->finalState;
}

bool NFA::isInAlphabet(char c) {
  if((c >= 49 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 46)
    return true;
  return false;
}

NFA* NFA::buildSingle(char c) {
  StateNode* st0 = new StateNode(0, 1);
  StateNode* st1 = new StateNode(1, 0);
  st0->addTransition(c, st1);
  initState = st0;
  finalState = st1;
  return this;
}

NFA* NFA::Union(NFA* a, NFA* b) {
  a->initState->setStart(false);
  b->initState->setStart(false);
  StateNode* s = new StateNode(1,0);
  s->addTransition('\0',a->initState);
  s->addTransition('\0',b->initState);
  StateNode* f = new StateNode(0,1);
  a->finalState->setFinal(false);
  a->finalState->addTransition('\0', f);
  b->finalState->setFinal(false);
  b->finalState->addTransition('\0', f);
  initState = s;
  finalState = f;
  return this;
}

NFA* NFA::Cat(NFA* a, NFA* b) {
  initState = a->initState;
  b->initState->setStart(false);
  finalState = b->finalState;
  a->finalState = b->initState;
  return this;
}

NFA* NFA::Star(NFA* a) {
  a->initState->setStart(false);
  StateNode* s = new StateNode(1,0);
  StateNode* f = new StateNode(0,1);
  s->addTransition('\0',f);
  s->addTransition('\0',a->initState);
  a->finalState->addTransition('\0', a->initState);
  a->finalState->addTransition('\0', f);
  initState = s;
  finalState = f;
  return this;
}

NFA::~NFA() {
  //haha
}
