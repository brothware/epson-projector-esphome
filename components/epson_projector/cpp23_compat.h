#pragma once

#include <type_traits>
#include <utility>
#include <variant>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
#include <expected>
#endif

namespace esphome::epson_projector::compat {

#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
#warning "C++23 std::to_underlying is available. Remove compat::to_underlying and use std::to_underlying directly."

using std::to_underlying;

#else

template <typename Enum>
constexpr auto to_underlying(Enum e) noexcept -> std::underlying_type_t<Enum> {
  return static_cast<std::underlying_type_t<Enum>>(e);
}

#endif

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
#warning "C++23 std::string::contains is available. Remove compat::contains and use str.contains() directly."

template <typename StringType, typename T>
constexpr bool contains(const StringType &str, const T &substr) {
  return str.contains(substr);
}

#else

template <typename StringType, typename T>
constexpr bool contains(const StringType &str, const T &substr) {
  return str.find(substr) != StringType::npos;
}

#endif

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202211L
#warning "C++23 std::expected is available. Remove compat::expected/unexpected and use std::expected directly."

using std::expected;
using std::unexpected;

#else

template <typename E>
class unexpected {
 public:
  constexpr unexpected(const unexpected &) = default;
  constexpr unexpected(unexpected &&) = default;

  template <typename Err = E>
  requires(!std::is_same_v<std::remove_cvref_t<Err>, unexpected> &&
           std::is_constructible_v<E, Err>) constexpr explicit unexpected(Err &&e)
      : error_(std::forward<Err>(e)) {}

  constexpr unexpected &operator=(const unexpected &) = default;
  constexpr unexpected &operator=(unexpected &&) = default;

  [[nodiscard]] constexpr const E &error() const & noexcept { return error_; }
  [[nodiscard]] constexpr E &error() & noexcept { return error_; }
  [[nodiscard]] constexpr const E &&error() const && noexcept { return std::move(error_); }
  [[nodiscard]] constexpr E &&error() && noexcept { return std::move(error_); }

 private:
  E error_;
};

template <typename E>
unexpected(E) -> unexpected<E>;

template <typename T, typename E>
class expected {
 public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  constexpr expected() : storage_(T{}), has_value_(true) {}

  constexpr expected(const expected &) = default;
  constexpr expected(expected &&) = default;

  template <typename U = T>
  requires(!std::is_same_v<std::remove_cvref_t<U>, expected> &&
           !std::is_same_v<std::remove_cvref_t<U>, unexpected<E>> &&
           std::is_constructible_v<T, U>) constexpr expected(U &&value)
      : storage_(std::forward<U>(value)), has_value_(true) {}

  template <typename G>
  requires std::is_constructible_v<E, const G &> constexpr expected(const unexpected<G> &u)
      : storage_(u.error()), has_value_(false) {}

  template <typename G>
  requires std::is_constructible_v<E, G> constexpr expected(unexpected<G> &&u)
      : storage_(std::move(u).error()), has_value_(false) {}

  constexpr expected &operator=(const expected &) = default;
  constexpr expected &operator=(expected &&) = default;

  template <typename U = T>
  requires(!std::is_same_v<std::remove_cvref_t<U>, expected> && std::is_constructible_v<T, U> &&
           std::is_assignable_v<T &, U>) constexpr expected &
  operator=(U &&value) {
    storage_ = std::forward<U>(value);
    has_value_ = true;
    return *this;
  }

  template <typename G>
  requires(std::is_constructible_v<E, const G &> &&std::is_assignable_v<E &, const G &>) constexpr expected &operator=(
      const unexpected<G> &u) {
    storage_ = u.error();
    has_value_ = false;
    return *this;
  }

  template <typename G>
  requires(std::is_constructible_v<E, G> &&std::is_assignable_v<E &, G>) constexpr expected &operator=(
      unexpected<G> &&u) {
    storage_ = std::move(u).error();
    has_value_ = false;
    return *this;
  }

  [[nodiscard]] constexpr explicit operator bool() const noexcept { return has_value_; }
  [[nodiscard]] constexpr bool has_value() const noexcept { return has_value_; }

  [[nodiscard]] constexpr T &value() & { return std::get<T>(storage_); }

  [[nodiscard]] constexpr const T &value() const & { return std::get<T>(storage_); }

  [[nodiscard]] constexpr T &&value() && { return std::get<T>(std::move(storage_)); }

  [[nodiscard]] constexpr const T &&value() const && { return std::get<T>(std::move(storage_)); }

  [[nodiscard]] constexpr E &error() & { return std::get<E>(storage_); }
  [[nodiscard]] constexpr const E &error() const & { return std::get<E>(storage_); }
  [[nodiscard]] constexpr E &&error() && { return std::get<E>(std::move(storage_)); }
  [[nodiscard]] constexpr const E &&error() const && { return std::get<E>(std::move(storage_)); }

  [[nodiscard]] constexpr T *operator->() noexcept { return &std::get<T>(storage_); }
  [[nodiscard]] constexpr const T *operator->() const noexcept { return &std::get<T>(storage_); }

  [[nodiscard]] constexpr T &operator*() & noexcept { return std::get<T>(storage_); }
  [[nodiscard]] constexpr const T &operator*() const & noexcept { return std::get<T>(storage_); }
  [[nodiscard]] constexpr T &&operator*() && noexcept { return std::get<T>(std::move(storage_)); }
  [[nodiscard]] constexpr const T &&operator*() const && noexcept { return std::get<T>(std::move(storage_)); }

  template <typename U>
  [[nodiscard]] constexpr T value_or(U &&default_value) const & {
    return has_value_ ? std::get<T>(storage_) : static_cast<T>(std::forward<U>(default_value));
  }

  template <typename U>
  [[nodiscard]] constexpr T value_or(U &&default_value) && {
    return has_value_ ? std::get<T>(std::move(storage_)) : static_cast<T>(std::forward<U>(default_value));
  }

 private:
  std::variant<T, E> storage_;
  bool has_value_;
};

#endif

}  // namespace esphome::epson_projector::compat
