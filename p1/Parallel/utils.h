//Kevin Shannon
//to make life esaier :)

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "StateNode.h"
#include <omp.h>

bool isStateInVector(StateNode* s, std::vector<StateNode*> v);
std::vector<StateNode*> Combine(std::vector<StateNode*> a, std::vector<StateNode*> b);
bool isVectorInVector(std::vector<StateNode*> s, std::vector<std::vector<StateNode*> > v);

#endif
