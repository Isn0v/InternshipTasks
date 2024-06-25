#include "Railroads.hpp"

#include <sstream>

namespace Railroads {

std::size_t merge(std::size_t* arr1, std::size_t len1, std::size_t* arr2,
                  std::size_t len2, std::size_t* buf) {
  std::size_t i = 0, j = 0, k = 0;
  std::size_t inv = 0;
  while (i < len1 && j < len2) {
    if (arr1[i] >= arr2[j]) {
      std::swap(buf[k], arr1[i]);
      i++;
    } else {
      std::swap(buf[k], arr2[j]);
      j++;
      inv += len1 - i;
    }
    k++;
  }

  while (i < len1) {
    std::swap(buf[k], arr1[i]);
    k++;
    i++;
  }
  while (j < len2) {
    std::swap(buf[k], arr2[j]);
    k++;
    j++;
  }
  return inv;
}

std::size_t inverse_merge_sort_impl(std::size_t* arr, std::size_t len,
                                    std::size_t* buf) {
  if (len == 1) {
    return 0;
  }

  std::size_t mid = len / 2;

  std::size_t inv = inverse_merge_sort_impl(arr, mid, buf);
  inv += inverse_merge_sort_impl(arr + mid, len - mid, buf);
  inv += merge(arr, mid, arr + mid, len - mid, buf);

  for (std::size_t i = 0; i < len; i++) {
    std::swap(arr[i], buf[i]);
  }
  return inv;
}

bool can_marshal_coaches(std::size_t* arr, std::size_t num_coaches) {
  std::size_t* buf = new std::size_t[num_coaches];
  std::size_t inv = inverse_merge_sort_impl(arr, num_coaches, buf);
  delete[] buf;
  return inv % 2 == 0;
}

std::string handle_coaches(const std::string& input) {
  std::stringstream result;
  std::istringstream ss(input);
  std::size_t num_coaches, tmp;
  while (1) {
    ss >> num_coaches;
    if (num_coaches == 0) {
      result << "\n";
      break;
    }
    result << "\n";
    while (1) {
      ss >> tmp;
      if (tmp == 0) {
        break;
      }

      std::size_t arr[num_coaches];
      arr[0] = tmp;
      for (std::size_t i = 1; i < num_coaches; i++) {
        ss >> arr[i];
      }
      if (can_marshal_coaches(arr, num_coaches)) {
        result << "YES\n";
      } else {
        result << "NO\n";
      }
    }
  }
  return result.str();
}

}  // namespace Railroads