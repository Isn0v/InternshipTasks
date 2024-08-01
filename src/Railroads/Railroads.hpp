#pragma once

#include <sstream>
#include <string>
#include <utility>

namespace Railroads {

bool can_marshal_coaches(std::size_t* arr, std::size_t num_coaches);
std::string handle_coaches(std::istream& input);

}  // namespace Railroads
