#ifndef __PRINT_IP_H_J5IOZ3FM63OQ__
#define __PRINT_IP_H_J5IOZ3FM63OQ__

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace detail {

template <typename Integral,
          typename Require = std::enable_if_t<std::is_integral_v<Integral>>>
auto to_string(Integral integral) -> std::string {
  std::ostringstream out;

  constexpr auto bytes_count = sizeof(integral);
  constexpr auto bits_per_byte = 8;
  constexpr auto BYTE_MASK = 0xFF;

  for (uint8_t i = 0; i < bytes_count - 1; i++) {
    auto shift = (bytes_count - 1 - i) * bits_per_byte;
    uint8_t byte = (integral >> shift) & BYTE_MASK;
    out << static_cast<std::size_t>(byte) << ".";
  }

  out << static_cast<std::size_t>(static_cast<uint8_t>(integral & BYTE_MASK));

  return out.str();
}

template <typename String,
          typename Require = std::enable_if_t<
              std::is_constructible_v<std::string, std::decay_t<String>>>>
auto to_string(String&& string) -> std::string {
  return std::string{std::forward<String>(string)};
}

template <typename Func, typename... Args>
struct has_overload {
  template <typename... U>
  static auto detect(Func&& f, U&&... args) -> decltype(f(args...));

  static auto detect(...) -> std::false_type;

  static constexpr bool value =
      !std::is_same_v<decltype(detect(std::declval<Func>(),
                                      std::declval<Args>()...)),
                      std::false_type>;
};

constexpr auto begin = [](auto&&... args) -> decltype(std::begin(args...)) {
  return std::begin(args...);
};

template <typename T>
constexpr auto has_begin_v = has_overload<decltype(begin), T>::value;

constexpr auto end = [](auto&&... args) -> decltype(std::end(args...)) {
  return std::end(args...);
};

template <typename T>
constexpr auto has_end_v = has_overload<decltype(end), T>::value;

template <class Container,
          typename Require = std::enable_if_t<
              has_begin_v<Container> && has_end_v<Container> &&
              !std::is_constructible_v<std::string, Container>>>
auto to_string(const Container& container) -> std::string {
  if (container.empty()) {
    return "";
  }

  std::ostringstream out;
  std::copy(std::begin(container), std::prev(std::end(container)),
            std::ostream_iterator<typename Container::value_type>{out, "."});
  out << *std::prev(std::end(container));

  return out.str();
}

namespace tuple {

inline auto write_per_dot(std::ostream& out) {}

template <typename T, typename... Other>
auto write_per_dot(std::ostream& out, const T& current,
                   const Other&... others) {
  if constexpr (sizeof...(others) == 0) {
    out << current;
  } else {
    out << current << ".";
    write_per_dot(out, others...);
  }
}

// Somehow works...
template <typename T = void, typename... Types>
constexpr auto is_all_same_v = (std::is_same_v<T, Types> && ...);

};  // namespace tuple

template <template <typename...> typename Tuple, typename... Elements,
          typename Require = std::enable_if_t<
              std::is_same_v<Tuple<Elements...>, std::tuple<Elements...>>>>
auto to_string(const Tuple<Elements...>& tuple) -> std::string {
  std::ostringstream out;

  static_assert(tuple::is_all_same_v<Elements...>, "All types must be same");

  std::apply(
      [&out](auto&&... args) {
        tuple::write_per_dot(out, std::forward<decltype(args)>(args)...);
      },
      tuple);

  return out.str();
}

}  // namespace detail

#endif  // __PRINT_IP_H_J5IOZ3FM63OQ__
