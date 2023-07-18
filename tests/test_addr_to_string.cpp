#include <deque>
#include <iterator>
#include <list>
#include <string_view>
#include <tuple>
#include <utility>

#include <gtest/gtest.h>

#include "print_ip.h"

TEST(AddressToString, ToStringInteger) {  // NOLINT
  ASSERT_EQ(detail::to_string(uint8_t{0}), "0");
  ASSERT_EQ(detail::to_string(uint8_t{0xFF}), "255");
  ASSERT_EQ(detail::to_string(int8_t{-1}), "255");

  ASSERT_EQ(detail::to_string(uint16_t{0x0000}), "0.0");
  ASSERT_EQ(detail::to_string(uint16_t{0xFF00}), "255.0");
  ASSERT_EQ(detail::to_string(int16_t{0x7F01}), "127.1");

  ASSERT_EQ(detail::to_string(uint32_t{0x0}), "0.0.0.0");
  ASSERT_EQ(detail::to_string(uint32_t{0x7F0000FF}), "127.0.0.255");
  ASSERT_EQ(detail::to_string(int32_t{0x7F0000FF}), "127.0.0.255");

  ASSERT_EQ(detail::to_string(uint64_t{0x7F0000FF}), "0.0.0.0.127.0.0.255");
  ASSERT_EQ(detail::to_string(uint64_t{0xABCDEF0102030405}),
            "171.205.239.1.2.3.4.5");
}

TEST(AddressToString, ToStringString) {  // NOLINT
  using namespace std::string_literals;
  using namespace std::string_view_literals;

  ASSERT_EQ(detail::to_string(""), ""s);
  ASSERT_EQ(detail::to_string(""sv), ""s);
  ASSERT_EQ(detail::to_string(""s), ""s);

  ASSERT_EQ(detail::to_string("42"), "42"s);
  ASSERT_EQ(detail::to_string("42"sv), "42"s);
  ASSERT_EQ(detail::to_string("42"s), "42"s);

  ASSERT_EQ(detail::to_string("Hello, World!"), "Hello, World!"s);

  {
    std::string hello_world = "Hello, World!";
    ASSERT_EQ(detail::to_string(std::move(hello_world)), "Hello, World!"s);
  }
}

TEST(AddressToString, ToStringVector) {  // NOLINT
  ASSERT_EQ(detail::to_string(std::vector<int>{}), "");
  ASSERT_EQ(detail::to_string(std::vector<int>{42}), "42");
  ASSERT_EQ(detail::to_string(std::vector<int>{42, 24}), "42.24");
  ASSERT_EQ(detail::to_string(std::vector<int>{42, 24, 77}), "42.24.77");
}

TEST(AddressToString, ToStringList) {  // NOLINT
  ASSERT_EQ(detail::to_string(std::list<int>{}), "");
  ASSERT_EQ(detail::to_string(std::list<int>{42}), "42");
  ASSERT_EQ(detail::to_string(std::list<int>{42, 24}), "42.24");
  ASSERT_EQ(detail::to_string(std::list<int>{42, 24, 77}), "42.24.77");
}

TEST(AddressToString, ToStringDeque) {  // NOLINT
  ASSERT_EQ(detail::to_string(std::deque<int>{}), "");
  ASSERT_EQ(detail::to_string(std::deque<int>{42}), "42");
  ASSERT_EQ(detail::to_string(std::deque<int>{42, 24}), "42.24");
  ASSERT_EQ(detail::to_string(std::deque<int>{42, 24, 77}), "42.24.77");
}

TEST(IsContainerTest, HasBegin) {  // NOLINT
  static_assert(detail::has_begin_v<std::vector<int>>, "vector");
  static_assert(detail::has_begin_v<std::vector<int>&>, "vector ref");
  static_assert(detail::has_begin_v<const std::vector<int>&>,
                "const vector ref");

  static_assert(detail::has_begin_v<std::list<int>>, "list");
  static_assert(detail::has_begin_v<std::deque<int>>, "deque");
  static_assert(detail::has_begin_v<std::set<int>>, "set");

  static_assert(detail::has_begin_v<int[2]>, "plain array");  // NOLINT
  // NOLINTNEXTLINE
  static_assert(detail::has_begin_v<const int[2]>, "const plain array");
  // NOLINTNEXTLINE
  static_assert(detail::has_begin_v<const int(&)[2]>, "const plain array ref");

  static_assert(!detail::has_begin_v<int>, "plain doest support std::begin");
}

TEST(IsContainerTest, HasEnd) {  // NOLINT
  static_assert(detail::has_end_v<std::vector<int>>, "vector");
  static_assert(detail::has_end_v<std::vector<int>&>, "vector ref");
  static_assert(detail::has_end_v<const std::vector<int>&>,
                "const vector ref");

  static_assert(detail::has_end_v<std::list<int>>, "list");
  static_assert(detail::has_end_v<std::deque<int>>, "deque");
  static_assert(detail::has_end_v<std::set<int>>, "set");

  static_assert(detail::has_end_v<int[2]>, "plain array");  // NOLINT
  // NOLINTNEXTLINE
  static_assert(detail::has_end_v<const int[2]>, "const plain array");
  // NOLINTNEXTLINE
  static_assert(detail::has_end_v<const int(&)[2]>, "const plain array ref");

  static_assert(!detail::has_end_v<int>, "plain doest support std::begin");
}


TEST(AddressToStringTest, TupleToString) { // NOLINT
  ASSERT_EQ(detail::to_string(std::tuple<>{}), "");
  ASSERT_EQ(detail::to_string(std::tuple<int>{42}), "42");
  ASSERT_EQ(detail::to_string(std::tuple<int>{24}), "24");
  ASSERT_EQ(detail::to_string(std::tuple<int, int>{42, 24}), "42.24");
  ASSERT_EQ(detail::to_string(std::tuple<int, int, int>{42, 24, 42}), "42.24.42");
  ASSERT_EQ(detail::to_string(std::tuple<int, int, int, int>{42, 24, 42, 24}), "42.24.42.24");
}


TEST(TupleTest, AllTypesEqual) { // NOLINT
static_assert(detail::tuple::is_all_same_v<>, "None types");
  static_assert(detail::tuple::is_all_same_v<int>, "One type");
  static_assert(detail::tuple::is_all_same_v<int, int>, "Two types equal");
  static_assert(!detail::tuple::is_all_same_v<int, float>, "Two types unequal");
}
