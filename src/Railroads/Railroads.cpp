#include "Railroads.hpp"

#include <sstream>

namespace Railroads {

/**
 * @brief Merges two sorted arrays into a single sorted array and
 * counts the number of inversions.
 *
 * This function merges two sorted arrays `arr1` and `arr2` into a single
 * sorted array `buf`. It maintains a count of the number of inversions
 * encountered during the merge. An inversion is considered to be a pair
 * of elements `(a, b)` in which `a` is greater than `b` but appears in a
 * position where `b` should appear.
 *
 * @param arr1 Pointer to the first array.
 * @param len1 Length of the first array.
 * @param arr2 Pointer to the second array.
 * @param len2 Length of the second array.
 * @param buf Pointer to the buffer array where the merged array is stored.
 * @return The number of inversions encountered during the merge.
 */
std::size_t merge(std::size_t* arr1, std::size_t len1, std::size_t* arr2,
                  std::size_t len2, std::size_t* buf) {
  // Initialize pointers and counters
  std::size_t i = 0, j = 0, k = 0;
  std::size_t inv = 0;

  // Merge the arrays
  while (i < len1 && j < len2) {
    if (arr1[i] >= arr2[j]) {
      // If arr1[i] is greater than or equal to arr2[j], swap arr1[i] with
      // buf[k]
      std::swap(buf[k], arr1[i]);
      i++;  // Increment the index of arr1
    } else {
      // If arr1[i] is less than arr2[j], swap arr2[j] with buf[k]
      std::swap(buf[k], arr2[j]);
      j++;              // Increment the index of arr2
      inv += len1 - i;  // Count the number of inversions
    }
    k++;  // Increment the index of buf
  }

  // Copy the remaining elements from arr1 (if any) to buf
  while (i < len1) {
    std::swap(buf[k], arr1[i]);
    k++;
    i++;
  }

  // Copy the remaining elements from arr2 (if any) to buf
  while (j < len2) {
    std::swap(buf[k], arr2[j]);
    k++;
    j++;
  }

  // Return the number of inversions
  return inv;
}

/**
 * Recursive implementation of inverse merge sort.
 *
 * @param arr Pointer to the array to be sorted.
 * @param len Length of the array.
 * @param buf Temporary buffer used for merging.
 *
 * @return The number of inversions in the array.
 */
std::size_t inverse_merge_sort_impl(std::size_t* arr, std::size_t len,
                                    std::size_t* buf) {
  // Base case: If the array has only one element, no inversions.
  if (len == 1) {
    return 0;
  }

  // Calculate the midpoint of the array.
  std::size_t mid = len / 2;

  // Recursively sort the first half of the array.
  std::size_t inv_l = inverse_merge_sort_impl(arr, mid, buf);

  // Recursively sort the second half of the array.
  std::size_t inv_r = inverse_merge_sort_impl(arr + mid, len - mid, buf);

  // Merge the sorted halves and count the number of inversions.
  std::size_t inv_m = merge(arr, mid, arr + mid, len - mid, buf);

  // Copy the sorted array back to the original array.
  for (std::size_t i = 0; i < len; i++) {
    std::swap(arr[i], buf[i]);
  }

  // Return the total number of inversions.
  return inv_l + inv_r + inv_m;
}

/**
 * Sorts the array using inverse merge sort and checks if the number of
 * inversions is even.
 *
 * @param arr Pointer to the array to be sorted.
 * @param num_coaches Length of the array.
 *
 * @return True if the number of inversions is even, false otherwise.
 */
bool can_marshal_coaches(std::size_t* arr, std::size_t num_coaches) {
  // Allocate a temporary buffer of the same size as the input array.
  std::size_t* buf = new std::size_t[num_coaches];

  // Sort the array using inverse merge sort and count the number of inversions.
  std::size_t inv = inverse_merge_sort_impl(arr, num_coaches, buf);

  // Free the temporary buffer.
  delete[] buf;

  // Return true if the number of inversions is even, false otherwise.
  return inv % 2 == 0;
}

/**
 * Reads coach numbers from the input stream, sorts them using inverse merge
 * sort and checks if the number of inversions is even.
 *
 * @param input The input stream to read coach numbers from.
 *
 * @return A string containing the result of each test case. Each test case is
 *         separated by a newline character and the result of the test case is
 *         either "YES" or "NO".
 */
std::string handle_coaches(std::istream& input) {
  // String stream to store the result of each test case.
  std::stringstream result;

  // Read the number of coaches in the test case.
  std::size_t num_coaches, tmp;
  while (1) {
    input >> num_coaches;

    // If the number of coaches is 0, it means the end of the input.
    if (num_coaches == 0) {
      result << "\n";
      break;
    }

    // Start a new test case.
    result << "\n";

    // Read the coach numbers in the test case.
    while (1) {
      input >> tmp;
      if (tmp == 0) {
        // If the coach number is 0, it means the end of the coaches in the test
        // case.
        break;
      }

      // Allocate memory for the array of coach numbers.
      std::size_t* arr = new std::size_t[num_coaches];

      // Initialize the first coach number.
      arr[0] = tmp;

      // Read the remaining coach numbers.
      for (std::size_t i = 1; i < num_coaches; i++) {
        input >> arr[i];
      }

      // Check if the number of inversions is even.
      if (can_marshal_coaches(arr, num_coaches)) {
        result << "YES\n";
      } else {
        result << "NO\n";
      }

      // Free the memory allocated for the array of coach numbers.
      delete[] arr;
    }
  }

  // Return the result of each test case.
  return result.str();
}

}  // namespace Railroads