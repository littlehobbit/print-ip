#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <utility>

#include "print_ip.h"

template <typename T>
void print_ip(T&& ip) {
  std::cout << detail::to_string(ip) << std::endl;
}

int main() {
  print_ip(int8_t{-1});                    // NOLINT 255
  print_ip(int16_t{0});                    // NOLINT 0.0
  print_ip(int32_t{2130706433});           // NOLINT 127.0.0.1
  print_ip(int64_t{8875824491850138409});  // NOLINT 123.45.67.89.101.112.131.41
  print_ip(std::string{"Hello, World!"});  // NOLINT Hello, World!
  print_ip(std::vector<int>{100, 200, 300, 400});  // NOLINT 100.200.300.400
  print_ip(std::list<short>{400, 300, 200, 100});  // NOLINT 400.300.200.100
  print_ip(std::make_tuple(123, 456, 789, 0));     // NOLINT 123.456.789.0
}