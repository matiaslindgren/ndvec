#ifndef NDVEC_HEADER_INCLUDED
#define NDVEC_HEADER_INCLUDED

#include <algorithm>
#include <concepts>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ndvec {

template <typename T, std::same_as<T>... Ts>
  requires(std::regular<T> and std::is_arithmetic_v<T>)
class ndvec {
public:
  static constexpr std::size_t ndim{sizeof...(Ts) + 1};

  using value_type = T;
  using values_type = std::tuple<T, Ts...>;
  using axes_indices = std::make_index_sequence<ndim>;

private:
  values_type data{};

public:
  constexpr ndvec() = default;

  constexpr explicit ndvec(value_type x, Ts... rest) : data(x, rest...) {}

  template <std::size_t axis>
    requires(axis < ndim)
  constexpr const value_type& get() const noexcept {
    return std::get<axis>(data);
  }

  template <std::size_t axis>
    requires(axis < ndim)
  constexpr value_type& get() noexcept {
    return std::get<axis>(data);
  }

  constexpr const values_type& values() const noexcept { return data; }

private:
  template <std::size_t axis, typename Fn, typename... Args>
    requires(axis < ndim and (... and std::same_as<ndvec, std::decay_t<Args>>))
  constexpr void apply_on_axis(Fn&& fn, Args&&... args) noexcept {
    get<axis>() = fn(args.template get<axis>()...);
  }

  template <typename Fn, std::size_t... axes, typename... Args>
  constexpr ndvec&
  apply_impl(Fn&& fn, std::index_sequence<axes...>, Args&&... args) noexcept {
    (apply_on_axis<axes>(std::forward<Fn>(fn), *this, std::forward<Args>(args)...), ...);
    return *this;
  }

public:
  template <std::regular_invocable<value_type> UnaryFn>
  constexpr ndvec& apply(UnaryFn&& fn) noexcept {
    return apply_impl(std::forward<UnaryFn>(fn), axes_indices{});
  }

  template <std::regular_invocable<value_type, value_type> BinaryFn>
  constexpr ndvec& apply(BinaryFn&& fn, const ndvec& rhs) noexcept {
    return apply_impl(std::forward<BinaryFn>(fn), axes_indices{}, rhs);
  }

  constexpr ndvec& operator+=(const ndvec& rhs) noexcept {
    return apply(std::plus<value_type>{}, rhs);
  }
  constexpr ndvec& operator-=(const ndvec& rhs) noexcept {
    return apply(std::minus<value_type>{}, rhs);
  }
  constexpr ndvec& operator*=(const ndvec& rhs) noexcept {
    return apply(std::multiplies<value_type>{}, rhs);
  }
  constexpr ndvec& operator/=(const ndvec& rhs) noexcept {
    return apply(std::divides<value_type>{}, rhs);
  }

  [[nodiscard]] constexpr ndvec operator+(const ndvec& rhs) const noexcept {
    ndvec lhs{*this};
    return lhs += rhs;
  }
  [[nodiscard]] constexpr ndvec operator-(const ndvec& rhs) const noexcept {
    ndvec lhs{*this};
    return lhs -= rhs;
  }
  [[nodiscard]] constexpr ndvec operator*(const ndvec& rhs) const noexcept {
    ndvec lhs{*this};
    return lhs *= rhs;
  }
  [[nodiscard]] constexpr ndvec operator/(const ndvec& rhs) const noexcept {
    ndvec lhs{*this};
    return lhs /= rhs;
  }
  [[nodiscard]] constexpr ndvec min(const ndvec& rhs) const noexcept {
    ndvec lhs{*this};
    return lhs.apply(
        [](value_type a, value_type b) constexpr noexcept -> value_type {
          return std::min(a, b);
        },
        rhs
    );
  }
  [[nodiscard]] constexpr ndvec max(const ndvec& rhs) const noexcept {
    ndvec lhs{*this};
    return lhs.apply(
        [](value_type a, value_type b) constexpr noexcept -> value_type {
          return std::max(a, b);
        },
        rhs
    );
  }

  [[nodiscard]] constexpr ndvec abs() const noexcept {
    ndvec res{*this};
    return res.apply([](value_type val) constexpr noexcept -> value_type {
      // TODO
      // clang-18 does not implement constexpr std::abs as of 2024-04-28
      return val < 0 ? -val : val;
    });
  }

  [[nodiscard]] constexpr ndvec signum() const noexcept {
    ndvec res{*this};
    return res.apply([](value_type val) constexpr noexcept -> value_type {
      return (value_type{} < val) - (val < value_type{});
    });
  }

  [[nodiscard]] constexpr value_type sum() const noexcept {
    return std::apply(
        [](std::same_as<value_type> auto... vs) constexpr noexcept -> value_type {
          return (... + vs);
        },
        data
    );
  }

  [[nodiscard]] constexpr value_type prod() const noexcept {
    return std::apply(
        [](std::same_as<value_type> auto... vs) constexpr noexcept -> value_type {
          return (... * vs);
        },
        data
    );
  }

  [[nodiscard]] constexpr value_type min() const noexcept {
    return std::apply(
        [](std::same_as<value_type> auto... vs) constexpr noexcept -> value_type {
          return std::min(std::initializer_list<value_type>{vs...});
        },
        data
    );
  }

  [[nodiscard]] constexpr value_type max() const noexcept {
    return std::apply(
        [](std::same_as<value_type> auto... vs) constexpr noexcept -> value_type {
          return std::max(std::initializer_list<value_type>{vs...});
        },
        data
    );
  }

  [[nodiscard]] constexpr auto operator<=>(const ndvec&) const noexcept = default;

  [[nodiscard]] constexpr value_type distance(const ndvec& rhs) const noexcept {
    return (*this - rhs).abs().sum();
  }

  constexpr void swap(ndvec& other) noexcept { data.swap(other.data); }

  constexpr auto&& x(this auto&& self) noexcept { return self.template get<0>(); }
  constexpr auto&& y(this auto&& self) noexcept { return self.template get<1>(); }
  constexpr auto&& z(this auto&& self) noexcept { return self.template get<2>(); }
  constexpr auto&& w(this auto&& self) noexcept { return self.template get<3>(); }

  constexpr ndvec& rotate_left() noexcept
    requires(ndim == 2)
  {
    x() = std::exchange(y(), -x());
    return *this;
  }

  constexpr ndvec& rotate_right() noexcept
    requires(ndim == 2)
  {
    x() = -std::exchange(y(), x());
    return *this;
  }

  [[nodiscard]] constexpr auto adjacent() const noexcept
    requires(ndim == 2)
  {
    return std::array{
        *this - ndvec(0, 1),
        *this - ndvec(1, 0),
        *this + ndvec(1, 0),
        *this + ndvec(0, 1),
    };
  }
};

template <typename T> using vec1 = ndvec<T>;
template <typename T> using vec2 = ndvec<T, T>;
template <typename T> using vec3 = ndvec<T, T, T>;
template <typename T> using vec4 = ndvec<T, T, T, T>;

} // namespace ndvec

template <std::integral... Ts> struct std::hash<ndvec::ndvec<Ts...>> {
private:
  using vec = ndvec::ndvec<Ts...>;
  using axes = vec::axes_indices;
  using T = vec::value_type;
  static constexpr auto slot_width{std::numeric_limits<std::size_t>::digits / vec::ndim};

  template <std::size_t... axes>
  constexpr auto hash_impl(const vec& v, std::index_sequence<axes...>) const noexcept {
    return (... | (std::hash<T>{}(v.template get<axes>()) << (slot_width * axes)));
  }

public:
  constexpr auto operator()(const vec& v) const noexcept { return hash_impl(v, axes{}); }
};

template <std::formattable<char>... Ts> struct std::formatter<ndvec::ndvec<Ts...>, char> {
private:
  using vec = ndvec::ndvec<Ts...>;

public:
  template <typename ParseContext> constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext> auto format(const vec& v, FormatContext& ctx) const {
    return std::format_to(ctx.out(), "ndvec{}{}", vec::ndim, v.values());
  }
};

template <typename... Ts>
std::istream& operator>>(std::istream& is, ndvec::ndvec<Ts...>& v) {
  using vec = ndvec::ndvec<Ts...>;
  using axes = vec::axes_indices;
  if (vec parsed;
      [&]<std::size_t... axis>(std::index_sequence<axis...>) -> std::istream& {
        return (is >> ... >> parsed.template get<axis>());
      }(axes{})) {
    v = parsed;
  }
  return is;
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const ndvec::ndvec<Ts...>& v) {
  return os << std::format("{}", v);
}

#endif // NDVEC_HEADER_INCLUDED
