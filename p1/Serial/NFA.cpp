//Kevin Shannon
//NFA method definitions

#include "NFA.h"

NFA::NFA() {
  this->states = std::vector<StateNode*> {};
  this->stateNames = std::set<int> {};
  this->startState = NULL;
  this->finalState = NULL;
}

NFA::NFA(std::string regex) {
  std::stack<NFA*> nfaStack;
  while(regex != "") {
    NFA* a = new NFA();
    NFA* b = new NFA();
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
  states = nfaStack.top()->states;
  stateNames = nfaStack.top()->stateNames;
  transitionTable = nfaStack.top()->transitionTable;
  startState = nfaStack.top()->startState;
  finalState = nfaStack.top()->finalState;
}

StateNode* NFA::getStartState() {
  return startState;
}

std::vector<StateNode*> NFA::epsClosure(StateNode* s) {
  std::queue<StateNode*> fringe;
  fringe.push(s);
  std::vector<StateNode*> visited;
  while(!fringe.empty()) {
    StateNode* state = fringe.front();
    fringe.pop();
    if(!isStateInVector(state, visited)) {
      visited.push_back(state);
      if(transitionTable.find(std::make_pair(state, '\0')) != transitionTable.end()) {
        for(std::vector<StateNode*>::iterator i = transitionTable[std::make_pair(state, '\0')].begin(); i != transitionTable[std::make_pair(state, '\0')].end(); ++i) {
          if(!isStateInVector(*i, visited))
            fringe.push(*i);
        }
      }
    }
  }
  return visited;
}

std::vector<StateNode*> NFA::Move(StateNode* s, char c) {
  std::vector<StateNode*> visited;
  if(transitionTable.find(std::make_pair(s, c)) != transitionTable.end()) {
    visited = Combine(visited, transitionTable[std::make_pair(s, c)]);
    for(std::vector<StateNode*>::iterator i = transitionTable[std::make_pair(s, c)].begin(); i != transitionTable[std::make_pair(s, c)].end(); ++i) {
      visited = Combine(visited, epsClosure(*i));
    }
  }
  std::vector<StateNode*> epsOfS = epsClosure(s);
  if(transitionTable.find(std::make_pair(s, '\0')) != transitionTable.end()) {
    for(std::vector<StateNode*>::iterator i = epsOfS.begin(); i != epsOfS.end(); ++i) {
      if(transitionTable.find(std::make_pair(*i, c)) != transitionTable.end()) {
        visited = Combine(visited, transitionTable[std::make_pair(*i, c)]);
        for(std::vector<StateNode*>::iterator j = transitionTable[std::make_pair(*i, c)].begin(); j != transitionTable[std::make_pair(*i, c)].end(); ++j)
          visited = Combine(visited, epsClosure(*j));
      }
    }
  }
  return visited;
}

void NFA::PrintTable() {
  for(std::vector<StateNode*>::iterator i = states.begin(); i != states.end(); ++i) {
    std::cout << "st: " << (*i)->getID() << ", ";
    for(std::set<char>::iterator j = alphabet.begin(); j != alphabet.end(); ++j) {
      std::map<std::pair<StateNode*, char>, std::vector<StateNode*> >::iterator k = transitionTable.find(std::make_pair(*i, *j));
      if(k != transitionTable.end()) {
        std::cout << "{";
        for(std::vector<StateNode*>::iterator l = transitionTable[std::make_pair(*i, *j)].begin(); l != transitionTable[std::make_pair(*i, *j)].end(); ++l) {
          std::cout << (*l)->getID() << " ";
        }
        std::cout << "}";
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}

bool NFA::isInAlphabet(char c) {
  return alphabet.count(c);
}

int NFA::newID(NFA* a) {
  if(!a->stateNames.empty())
    return *(a->stateNames.insert(*a->stateNames.rbegin() + 1).first);
  return *(a->stateNames.insert(0).first);
}

NFA* NFA::buildSingle(char c) {
  NFA* t = new NFA();
  StateNode* st0 = new StateNode(1, 0, newID(t));
  StateNode* st1 = new StateNode(0, 1, newID(t));
  t->states.push_back(st0);
  t->states.push_back(st1);
  t->transitionTable.insert(std::make_pair(std::make_pair(st0, c), std::vector<StateNode*> {st1}));
  t->startState = st0;
  t->finalState = st1;
  return t;
}

void NFA::Glob(NFA* src, NFA* tar) {
  for(std::vector<StateNode*>::iterator it = src->states.begin(); it != src->states.end(); it++) {
    (*it)->setID(newID(tar));
    tar->states.push_back(*it);
    tar->stateNames.insert((*it)->getID());
  }
  tar->transitionTable.insert(src->transitionTable.begin(), src->transitionTable.end());
}

NFA* NFA::Union(NFA* a, NFA* b) {
  NFA* t = new NFA();
  Glob(a, t);
  Glob(b, t);
  a->startState->setStart(false);
  b->startState->setStart(false);
  StateNode* s = new StateNode(1, 0, newID(t));
  t->transitionTable.insert(std::make_pair(std::make_pair(s, '\0'), std::vector<StateNode*> {a->startState, b->startState}));
  t->startState = s;
  t->states.push_back(s);
  a->finalState->setFinal(false);
  b->finalState->setFinal(false);
  StateNode* f = new StateNode(0, 1, newID(t));
  t->transitionTable.insert(std::make_pair(std::make_pair(a->finalState, '\0'), std::vector<StateNode*> {f}));
  t->transitionTable.insert(std::make_pair(std::make_pair(b->finalState, '\0'), std::vector<StateNode*> {f}));
  t->finalState = f;
  t->states.push_back(f);
  return t;
}

NFA* NFA::Cat(NFA* a, NFA* b) {
  NFA* t = new NFA();
  Glob(a, t);
  Glob(b, t);
  t->startState = a->startState;
  a->finalState->setFinal(false);
  t->finalState = b->finalState;
  b->startState->setStart(false);
  t->transitionTable.insert(std::make_pair(std::make_pair(a->finalState, '\0'), std::vector<StateNode*> {b->startState}));
  return t;
}

NFA* NFA::Star(NFA* a) {
  NFA* t = new NFA();
  Glob(a, t);
  a->startState->setStart(false);
  a->finalState->setFinal(false);
  StateNode* s = new StateNode(1, 0, newID(t));
  StateNode* f = new StateNode(0, 1, newID(t));
  t->transitionTable.insert(std::make_pair(std::make_pair(s, '\0'), std::vector<StateNode*> {f, a->startState}));
  t->transitionTable.insert(std::make_pair(std::make_pair(a->finalState, '\0'), std::vector<StateNode*> {f, a->startState}));
  t->startState = s;
  t->finalState = f;
  t->states.push_back(s);
  t->states.push_back(f);
  return t;
}

NFA::~NFA() {
  //haha
}
