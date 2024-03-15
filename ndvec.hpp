#include <algorithm>
#include <concepts>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename T, std::same_as<T>... Ts>
  requires(std::regular<T> and std::is_arithmetic_v<T>)
struct ndvec {
  static constexpr std::size_t ndim{sizeof...(Ts) + 1};
  std::tuple<T, Ts...> elements;

  using value_type = T;
  using axes_indices = std::make_index_sequence<ndim>;

  constexpr ndvec() = default;

  constexpr explicit ndvec(value_type x, Ts... rest) : elements(x, rest...) {}

  // TODO deducing this
  template <std::size_t axis>
    requires(axis < ndim)
  constexpr value_type& get() noexcept {
    return std::get<axis>(elements);
  }
  template <std::size_t axis>
    requires(axis < ndim)
  constexpr const value_type& get() const noexcept {
    return std::get<axis>(elements);
  }

  // TODO deducing this
  constexpr value_type& x() noexcept { return get<0>(); }
  constexpr value_type& y() noexcept { return get<1>(); }
  constexpr value_type& z() noexcept { return get<2>(); }
  constexpr const value_type& x() const noexcept { return get<0>(); }
  constexpr const value_type& y() const noexcept { return get<1>(); }
  constexpr const value_type& z() const noexcept { return get<2>(); }

  [[nodiscard]] constexpr auto operator<=>(const ndvec&) const noexcept = default;

private:
  template <typename Fn, std::size_t... axes, typename... Args>
    requires(
        (... and (axes < ndim))
        and (... and std::same_as<ndvec, std::decay_t<Args>>)
    )
  constexpr ndvec&
  apply_impl(Fn&& fn, std::index_sequence<axes...>, Args&&... args) noexcept {
    auto apply_on_axis{[&]<std::size_t axis>() {
      get<axis>() = fn(args.template get<axis>()...);
    }};
    (apply_on_axis.template operator()<axes>(), ...);
    return *this;
  }

public:
  template <std::regular_invocable<value_type> UnaryFn>
  constexpr ndvec& apply(UnaryFn&& fn) noexcept {
    return apply_impl(std::forward<UnaryFn>(fn), axes_indices{}, *this);
  }

  template <std::regular_invocable<value_type, value_type> BinaryFn>
  constexpr ndvec& apply(BinaryFn&& fn, const ndvec& rhs) noexcept {
    return apply_impl(std::forward<BinaryFn>(fn), axes_indices{}, *this, rhs);
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

  [[nodiscard]] constexpr ndvec abs() const noexcept {
    ndvec res{*this};
    return res.apply([](value_type val) noexcept -> value_type {
      return std::abs(val);
    });
  }

  [[nodiscard]] constexpr ndvec signum() const noexcept {
    ndvec res{*this};
    return res.apply([](value_type val) noexcept -> value_type {
      return (value_type{} < val) - (val < value_type{});
    });
  }

  [[nodiscard]] constexpr value_type sum() const noexcept {
    return std::apply(
        [](std::same_as<value_type> auto... vs) noexcept -> value_type {
          return (... + vs);
        },
        elements
    );
  }

  [[nodiscard]] constexpr value_type distance(const ndvec& rhs) const noexcept {
    return (*this - rhs).abs().sum();
  }

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

template <std::integral... Ts> struct std::hash<ndvec<Ts...>> {
  using ndvec = ndvec<Ts...>;
  using T = ndvec::value_type;
  static constexpr auto slot_width{
      std::numeric_limits<std::size_t>::digits / ndvec::ndim
  };

  template <typename axes = ndvec::axes_indices>
  constexpr auto operator()(const ndvec& v) const noexcept {
    return [&]<std::size_t... axis>(std::index_sequence<axis...>) {
      return (
          ... | (std::hash<T>{}(v.template get<axis>()) << (slot_width * axis))
      );
    }(axes{});
  }
};

template <std::formattable<char>... Ts>
struct std::formatter<ndvec<Ts...>, char> {
  template <typename ParseContext> constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ndvec<Ts...>& v, FormatContext& ctx) const {
    return std::format_to(ctx.out(), "ndvec{}{}", ndvec<Ts...>::ndim, v.elements);
  }
};

template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const ndvec<Ts...>& v) {
  return os << std::format("{}", v);
}

template <typename... Ts>
std::istream& operator>>(std::istream& is, ndvec<Ts...>& v) {
  using axes = ndvec<Ts...>::axes;
  if (ndvec<Ts...> parsed;
      [&]<std::size_t... axis>(std::index_sequence<axis...>) {
        return (is >> ... >> parsed.template get<axis>());
      }(axes{})) {
    v = parsed;
  }
  return is;
}
