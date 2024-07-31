#pragma once

#include <string>
#include <utility>
#include <sstream>

namespace Railroads {

/**
 * @brief Checks if the given array of coach sizes can be marshaled.
 *
 * @param arr Pointer to an array of coach sizes.
 * @param num_coaches Number of coaches.
 * @return True if the coaches can be marshaled, false otherwise.
 *
 * A coach can be marshaled if the number of inversions in its reversed (in
 * descending sequence) mergesort is even.
 */
bool can_marshal_coaches(std::size_t* arr, std::size_t num_coaches);

/**
 * @brief Parses the input string and generates a response based on the
 * coaches information.
 *
 * The input string is expected to be in the following format:
 *
 * [number_of_coaches] [coach1_size] [coach2_size] ... [coachN_size] with 0 at
 * the end.
 *
 * The function reads the input string and generates a response for each set
 * of coaches. The response is generated based on whether the coaches can be
 * marshaled or not.
 *
 * @param input The input string containing the coaches information.
 *
 * @return The response string containing the result for each set of coaches
 *
 * If the coaches can be marshaled, the response string will contain "YES".
 * Otherwise, it will contain "NO".
 */
std::string handle_coaches(std::istream& input);

}  // namespace Railroads
