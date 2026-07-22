#include <format>

#include "value.h"

std::string valueToString(Value value) {
    return std::format("{:g}", value);
}