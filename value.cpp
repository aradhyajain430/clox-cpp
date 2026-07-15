#include <iostream>
#include <format>

#include "value.h"

void printValue(Value value) {
    std::cout << std::format("{:g}", value);
}