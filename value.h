#pragma once

#include "common.h"

#include <string>

//Alias so that I can use a tagged union without changing stuff when I implement it. 
using Value = double;

std::string valueToString(Value value);