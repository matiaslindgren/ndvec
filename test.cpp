#include "ndvec.hpp"
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

using std::operator""s;

void assert(bool exp, std::string&& msg) {
  if (not exp) {
    throw std::runtime_error(msg);
  }
}

void assert_equal(auto&& lhs, auto&& rhs, std::string&& msg) {
  assert(
      lhs == rhs,
      std::format("{}: expected {} and {} to compare equal", msg, lhs, rhs)
  );
}

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
template <typename T> std::string demangle() {
  auto ti{typeid(T).name()};
  int status{};
  auto name{abi::__cxa_demangle(ti, nullptr, nullptr, &status)};
  return status == 0 ? name : ti;
}
#else
template <typename T> std::string demangle() { return typeid(T).name(); }
#endif

using namespace ndvec;

template <typename T> void test_vec1() {
  std::println("test_vec1<{}>", demangle<T>());
  static_assert(std::regular<vec1<T>>);
  {
    vec1<T> v;
    assert(v.x() == T{}, std::format("vec1: default init x should be {}", T{}));
  }
  {
    vec1<T> v(1);
    assert(v.x() == 1, "vec1(1) => x should be 1");
  }
  {
    vec1<T> v(1);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec1{}", std::tuple{1}),
        "unexpected ndvec format with 1 element"
    );
  }
  {
    vec1<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "expected vec1() == vec1()");
  }
  {
    vec1<T> lhs(1), rhs(1);
    assert((lhs <=> rhs) == 0, "expected vec1(1) == vec1(1)");
  }
  {
    vec1<T> lhs(1), rhs(2);
    assert((lhs <=> rhs) < 0, "expected vec1(1) < vec1(2)");
  }
  {
    vec1<T> lhs(2), rhs(1);
    assert((lhs <=> rhs) > 0, "expected vec1(2) > vec1(1)");
  }
  {
    vec1<T> lhs(6), rhs(-2);
    lhs += rhs;
    assert_equal(lhs, vec1<T>(4), "vec1(6) + vec1(-2)");
  }
  {
    vec1<T> lhs(2), rhs(6);
    lhs -= rhs;
    assert_equal(lhs, vec1<T>(-4), "vec1(2) - vec1(6)");
  }
  {
    vec1<T> lhs(6), rhs(-2);
    lhs *= rhs;
    assert_equal(lhs, vec1<T>(-12), "vec1(6) * vec1(-2)");
  }
  {
    vec1<T> lhs(-6), rhs(-2);
    lhs /= rhs;
    assert_equal(lhs, vec1<T>(3), "vec1(-6) / vec1(-2)");
  }
  {
    vec1<T> lhs(6), rhs(-2);
    assert_equal(lhs.min(rhs), vec1<T>(-2), "min(vec1(6), vec1(-2))");
  }
  {
    vec1<T> lhs(6), rhs(-2);
    assert_equal(lhs.max(rhs), vec1<T>(6), "max(vec1(6), vec1(-2))");
  }
  {
    std::istringstream is{"1"s};
    vec1<T> v;
    is >> v;
    assert_equal(v, vec1<T>(1), "parse string 1");
  }
}

template <typename T> void test_vec2() {
  std::println("test_vec2<{}>", demangle<T>());
  static_assert(std::regular<vec2<T>>);
  {
    vec2<T> v;
    assert(v.x() == T{}, std::format("vec2: default init x should be {}", T{}));
    assert(v.y() == T{}, std::format("vec2: default init y should be {}", T{}));
  }
  {
    vec2<T> v(1, 2);
    assert(v.x() == 1, "vec2(1, 2) => x should be 1");
    assert(v.y() == 2, "vec2(1, 2) => y should be 2");
  }
  {
    vec2<T> v(1, 2);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec2{}", std::tuple{1, 2}),
        "unexpected ndvec format with 2 elements"
    );
  }
  {
    vec2<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "expected vec2() == vec2()");
  }
  {
    vec2<T> lhs(1, 1), rhs(1, 1);
    assert((lhs <=> rhs) == 0, "expected vec2(1, 1) == vec2(1, 1)");
  }
  {
    vec2<T> lhs(1, 1), rhs(1, 2);
    assert((lhs <=> rhs) < 0, "expected vec2(1, 1) < vec2(1, 2)");
  }
  {
    vec2<T> lhs(1, 1), rhs(2, 2);
    assert((lhs <=> rhs) < 0, "expected vec2(1, 1) < vec2(2, 2)");
  }
  {
    vec2<T> lhs(1, 2), rhs(1, 1);
    assert((lhs <=> rhs) > 0, "expected vec2(1, 2) > vec2(1, 1)");
  }
  {
    vec2<T> lhs(2, 2), rhs(1, 1);
    assert((lhs <=> rhs) > 0, "expected vec2(2, 2) > vec2(1, 1)");
  }
  {
    vec2<T> lhs(6, -12), rhs(3, 2);
    lhs += rhs;
    assert_equal(lhs, vec2<T>(9, -10), "vec2(6, -12) + vec2(3, 2)");
  }
  {
    vec2<T> lhs(6, 12), rhs(-3, 2);
    lhs -= rhs;
    assert_equal(lhs, vec2<T>(9, 10), "vec2(6, 12) - vec2(-3, 2)");
  }
  {
    vec2<T> lhs(6, 12), rhs(3, -2);
    lhs *= rhs;
    assert_equal(lhs, vec2<T>(18, -24), "vec2(6, 12) * vec2(3, -2)");
  }
  {
    vec2<T> lhs(6, 12), rhs(3, 2);
    lhs /= rhs;
    assert_equal(lhs, vec2<T>(2, 6), "vec2(6, 12) / vec2(3, 2)");
  }
  {
    vec2<T> lhs(6, 12), rhs(3, 2);
    assert_equal(lhs.min(rhs), vec2<T>(3, 2), "vec2(6, 12).min(vec2(3, 2))");
  }
  {
    vec2<T> lhs(6, 12), rhs(3, 2);
    assert_equal(lhs.max(rhs), vec2<T>(6, 12), "vec2(6, 12).max(vec2(3, 2))");
  }
  {
    auto input{"1 2"s};
    std::istringstream is{input};
    vec2<T> v;
    is >> v;
    assert_equal(v, vec2<T>(1, 2), std::format("parse string '{}'", input));
  }
  {
    auto input{"1    2"s};
    std::istringstream is{input};
    vec2<T> v;
    is >> v;
    assert_equal(v, vec2<T>(1, 2), std::format("parse string '{}'", input));
  }
}

template <typename T> void test_vec3() {
  std::println("test_vec3<{}>", demangle<T>());
  static_assert(std::regular<vec3<T>>);
  {
    vec3<T> v;
    assert(v.x() == T{}, std::format("vec3: default init x should be {}", T{}));
    assert(v.y() == T{}, std::format("vec3: default init y should be {}", T{}));
    assert(v.z() == T{}, std::format("vec3: default init z should be {}", T{}));
  }
  {
    vec3<T> v(1, 2, 3);
    assert(v.x() == 1, "vec3(1, 2, 3) => x should be 1");
    assert(v.y() == 2, "vec3(1, 2, 3) => y should be 2");
    assert(v.z() == 3, "vec3(1, 2, 3) => z should be 3");
  }
  {
    vec3<T> v(1, 2, 3);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec3{}", std::tuple{1, 2, 3}),
        "unexpected ndvec format with 3 elements"
    );
  }
  {
    vec3<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "expected vec3() == vec3()");
  }
  {
    vec3<T> lhs(1, 1, 1), rhs(1, 1, 1);
    assert((lhs <=> rhs) == 0, "expected vec3(1, 1, 1) == vec3(1, 1, 1)");
  }
  {
    vec3<T> lhs(1, 1, 1), rhs(1, 1, 2);
    assert((lhs <=> rhs) < 0, "expected vec3(1, 1, 1) < vec3(1, 1, 2)");
  }
  {
    vec3<T> lhs(1, 1, 1), rhs(1, 2, 2);
    assert((lhs <=> rhs) < 0, "expected vec3(1, 1, 1) < vec3(1, 2, 2)");
  }
  {
    vec3<T> lhs(1, 1, 1), rhs(2, 2, 2);
    assert((lhs <=> rhs) < 0, "expected vec3(1, 1, 1) < vec3(2, 2, 2)");
  }
  {
    vec3<T> lhs(1, 1, 2), rhs(1, 1, 1);
    assert((lhs <=> rhs) > 0, "expected vec3(1, 1, 2) > vec3(1, 1, 1)");
  }
  {
    vec3<T> lhs(1, 2, 2), rhs(1, 1, 1);
    assert((lhs <=> rhs) > 0, "expected vec3(1, 2, 2) > vec3(1, 1, 1)");
  }
  {
    vec3<T> lhs(2, 2, 2), rhs(1, 1, 1);
    assert((lhs <=> rhs) > 0, "expected vec3(2, 2, 2) > vec3(1, 1, 1)");
  }
  {
    vec3<T> lhs(-2, 0, 2), rhs(1, 2, 3);
    lhs += rhs;
    assert_equal(lhs, vec3<T>(-1, 2, 5), "vec3(-2, 0, 2) + vec3(1, 2, 3)");
  }
  {
    vec3<T> lhs(-2, 0, 2), rhs(1, 2, 3);
    lhs -= rhs;
    assert_equal(lhs, vec3<T>(-3, -2, -1), "vec3(-2, 0, 2) - vec3(1, 2, 3)");
  }
  {
    vec3<T> lhs(-2, 0, 2), rhs(1, 2, 3);
    lhs *= rhs;
    assert_equal(lhs, vec3<T>(-2, 0, 6), "vec3(-2, 0, 2) * vec3(1, 2, 3)");
  }
  {
    vec3<T> lhs(-2, 0, 6), rhs(1, 2, 3);
    lhs /= rhs;
    assert_equal(lhs, vec3<T>(-2, 0, 2), "vec3(-2, 0, 2) / vec3(1, 2, 3)");
  }
  {
    vec3<T> lhs(-2, 0, 6), rhs(1, 2, 3);
    assert_equal(lhs.min(rhs), vec3<T>(-2, 0, 3), "vec3(-2, 0, 2).min(vec3(1, 2, 3))");
  }
  {
    vec3<T> lhs(-2, 0, 6), rhs(1, 2, 3);
    assert_equal(lhs.max(rhs), vec3<T>(1, 2, 6), "vec3(-2, 0, 2).max(vec3(1, 2, 3))");
  }
  {
    auto input{"1 2 3"s};
    std::istringstream is{input};
    vec3<T> v;
    is >> v;
    assert_equal(v, vec3<T>(1, 2, 3), std::format("parse string '{}'", input));
  }
  {
    auto input{"   1   -2    3"s};
    std::istringstream is{input};
    vec3<T> v;
    is >> v;
    assert_equal(v, vec3<T>(1, -2, 3), std::format("parse string '{}'", input));
  }
  {
    vec3<T> v(-1, 2, -3);
    assert_equal(v.abs(), vec3<T>(1, 2, 3), "vec3(-1, 2, -3).abs()");
  }
  {
    vec3<T> v(-1, 2, -3);
    assert_equal(v.signum(), vec3<T>(-1, 1, -1), "vec3(-1, 2, -3).signum()");
  }
  {
    vec3<T> v;
    assert_equal(v.sum(), 0, "vec3().sum()");
  }
  {
    vec3<T> v(-1, 2, -3);
    assert_equal(v.sum(), -2, "vec3(-1, 2, -3).sum()");
  }
  {
    vec3<T> v;
    assert_equal(v.distance(v), 0, "center distance to center");
  }
  {
    vec3<T> lhs(-1, 2, -3);
    assert_equal(lhs.distance(vec3<T>()), 6, "vec3(-1, 2, -3) distance to center");
    assert_equal(lhs.distance(lhs), 0, "vec3(-1, 2, -3) distance to self");
  }
  {
    vec3<T> lhs(-1, 2, -3);
    vec3<T> rhs(1, 2, 3);
    assert_equal(lhs.distance(rhs), 8, "vec3(-1, 2, -3) distance to vec3(1, 2, 3)");
  }
}

template <typename T> void test_vec4() {
  std::println("test_vec4<{}>", demangle<T>());
  static_assert(std::regular<vec4<T>>);
  {
    vec4<T> v;
    assert(v.x() == T{}, std::format("vec4: default init x should be {}", T{}));
    assert(v.y() == T{}, std::format("vec4: default init y should be {}", T{}));
    assert(v.z() == T{}, std::format("vec4: default init z should be {}", T{}));
    assert(v.w() == T{}, std::format("vec4: default init w should be {}", T{}));
  }
  {
    vec4<T> v(1, 2, 3, 4);
    assert(v.x() == 1, "vec4(1, 2, 3, 4) => x should be 1");
    assert(v.y() == 2, "vec4(1, 2, 3, 4) => y should be 2");
    assert(v.z() == 3, "vec4(1, 2, 3, 4) => z should be 3");
    assert(v.w() == 4, "vec4(1, 2, 3, 4) => w should be 4");
  }
  {
    vec4<T> v(1, 2, 3, 4);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec4{}", std::tuple{1, 2, 3, 4}),
        "unexpected ndvec format with 4 elements"
    );
  }
  {
    vec4<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "expected vec4() == vec4()");
  }
  {
    vec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs += rhs;
    assert_equal(lhs, vec4<T>(12, 12, 12, 12), "vec4(10, 10, 10, 10) + vec4(2, 2, 2, 2)");
  }
  {
    vec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs -= rhs;
    assert_equal(lhs, vec4<T>(8, 8, 8, 8), "vec4(10, 10, 10, 10) - vec4(2, 2, 2, 2)");
  }
  {
    vec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs *= rhs;
    assert_equal(lhs, vec4<T>(20, 20, 20, 20), "vec4(10, 10, 10, 10) * vec4(2, 2, 2, 2)");
  }
  {
    vec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs /= rhs;
    assert_equal(lhs, vec4<T>(5, 5, 5, 5), "vec4(10, 10, 10, 10) / vec4(2, 2, 2, 2)");
  }
  {
    vec4<T> lhs(10, 8, 6, 4), rhs(2, 4, 6, 8);
    assert_equal(
        lhs.min(rhs),
        vec4<T>(2, 4, 6, 4),
        "vec4(10, 8, 6, 4).min(vec4(2, 4, 6, 8))"
    );
  }
  {
    vec4<T> lhs(10, 8, 6, 4), rhs(2, 4, 6, 8);
    assert_equal(
        lhs.max(rhs),
        vec4<T>(10, 8, 6, 8),
        "vec4(10, 8, 6, 4).max(vec4(2, 4, 6, 8))"
    );
  }
  {
    auto input{"   1   -2    3   -1000"s};
    std::istringstream is{input};
    vec4<T> v;
    is >> v;
    assert_equal(v, vec4<T>(1, -2, 3, -1000), std::format("parse string '{}'", input));
  }
}

template <typename T> void test_hash() {
  std::println("test_hash<{}>", demangle<T>());
  auto hash{std::hash<T>{}};
  auto width{std::numeric_limits<std::size_t>::digits};
  {
    vec1<T> v;
    assert_equal(std::hash<vec1<T>>{}(v), hash(0), "vec1() hash");
  }
  {
    vec2<T> v;
    assert_equal(
        std::hash<vec2<T>>{}(v),
        hash(0) | (hash(0) << (width / 2)),
        "vec2() hash"
    );
  }
  {
    vec3<T> v;
    assert_equal(
        std::hash<vec3<T>>{}(v),
        hash(0) | (hash(0) << (width / 3)) | (hash(0) << (2 * (width / 3))),
        "vec3() hash"
    );
  }
  {
    vec4<T> v;
    assert_equal(
        std::hash<vec4<T>>{}(v),
        hash(0) | (hash(0) << (width / 4)) | (hash(0) << (2 * (width / 4)))
            | (hash(0) << (2 * (width / 4))),
        "vec4() hash"
    );
  }
  {
    vec1<T> v(0xabc);
    assert_equal(std::hash<vec1<T>>{}(v), hash(0xabc), "vec1(0xabc) hash");
  }
  {
    vec2<T> v(0xabc, 0x123);
    assert_equal(
        std::hash<vec2<T>>{}(v),
        (hash(0xabc) << 0) | (hash(0x123) << (width / 2)),
        "vec2(0xabc, 0x123) hash"
    );
  }
  {
    vec3<T> v(0xabc, 0x123, -0xfed);
    assert_equal(
        std::hash<vec3<T>>{}(v),
        (hash(0xabc) << 0) | (hash(0x123) << (width / 3))
            | (hash(-0xfed) << ((2 * (width / 3)))),
        "vec3(0xabc, 0x123, -0xfed) hash"
    );
  }
  {
    vec4<T> v(0xabc, 0x123, 0, -0xfed);
    assert_equal(
        std::hash<vec4<T>>{}(v),
        (hash(0xabc) << 0) | (hash(0x123) << (width / 4)) | (hash(0) << (2 * (width / 4)))
            | (hash(-0xfed) << (3 * (width / 4))),
        "vec4(0xabc, 0x123, 0, -0xfed) hash"
    );
  }
}

template <typename... Ts> void test_vec() {
  (test_vec1<Ts>(), ...);
  (test_vec2<Ts>(), ...);
  (test_vec3<Ts>(), ...);
  (test_vec4<Ts>(), ...);
}

template <typename T> consteval void test_vec_compile_time_impl() {
  static_assert(std::regular<vec4<T>>);
  {
    constexpr vec4<T> v;
    static_assert(v.x() == T{}, "vec4: default init x should be T{}");
    static_assert(v.y() == T{}, "vec4: default init y should be T{}");
    static_assert(v.z() == T{}, "vec4: default init z should be T{}");
    static_assert(v.w() == T{}, "vec4: default init w should be T{}");
  }
  {
    constexpr vec4<T> v(1, 2, 3, 4);
    static_assert(v.x() == 1, "vec4(1, 2, 3, 4) => x should be 1");
    static_assert(v.y() == 2, "vec4(1, 2, 3, 4) => y should be 2");
    static_assert(v.z() == 3, "vec4(1, 2, 3, 4) => z should be 3");
    static_assert(v.w() == 4, "vec4(1, 2, 3, 4) => w should be 4");
  }
  {
    constexpr vec4<T> lhs, rhs;
    static_assert((lhs <=> rhs) == 0, "expected vec4() == vec4()");
  }
  {
    constexpr vec4<T> lhs(-1, 0, 1, 2);
    constexpr vec4<T> rhs(-1, 0, 1, 2);
    static_assert((lhs <=> rhs) == 0, "expected vec4(-1, 0, 1, 2) == vec4(-1, 0, 1, 2)");
  }
  {
    constexpr vec4<T> lhs(-1, 0, 1, 1);
    constexpr vec4<T> rhs(-1, 0, 1, 2);
    static_assert((lhs <=> rhs) < 0, "expected vec4(-1, 0, 1, 1) < vec4(-1, 0, 1, 2)");
    static_assert((rhs <=> lhs) > 0, "expected vec4(-1, 0, 1, 2) > vec4(-1, 0, 1, 1)");
  }
  {
    constexpr vec4<T> lhs(-1, 0, 0, 3);
    constexpr vec4<T> rhs(-1, 0, 1, 2);
    static_assert((lhs <=> rhs) < 0, "expected vec4(-1, 0, 0, 3) < vec4(-1, 0, 1, 2)");
    static_assert((rhs <=> lhs) > 0, "expected vec4(-1, 0, 1, 2) < vec4(-1, 0, 0, 3)");
  }
  {
    constexpr vec4<T> lhs(-2, 0, 2, 3);
    constexpr vec4<T> rhs(-1, 0, 1, 2);
    static_assert((lhs <=> rhs) < 0, "expected vec4(-2, 0, 2, 3) < vec4(-1, 0, 1, 2)");
    static_assert((rhs <=> lhs) > 0, "expected vec4(-1, 0, 1, 2) > vec4(-2, 0, 2, 3)");
  }
  {
    constexpr vec4<T> lhs(1, 2, 3, 4);
    constexpr vec4<T> rhs(12, 10, 8, 6);
    constexpr vec4<T> res(13, 12, 11, 10);
    static_assert((lhs + rhs) == res, "vec4(1, 2, 3, 4) + vec4(12, 10, 8, 6)");
  }
  {
    constexpr vec4<T> lhs(7, 6, 5, 4);
    constexpr vec4<T> rhs(4, 5, 6, 7);
    constexpr vec4<T> res(3, 1, -1, -3);
    static_assert((lhs - rhs) == res, "vec4(7, 6, 5, 4) - vec4(4, 5, 6, 7)");
  }
  {
    constexpr vec4<T> lhs(1, -2, 0, 4);
    constexpr vec4<T> rhs(8, 6, 4, 2);
    constexpr vec4<T> res(8, -12, 0, 8);
    static_assert((lhs * rhs) == res, "vec4(1, -2, 0, 4) * vec4(8, 6, 4, 2)");
  }
  {
    constexpr vec4<T> lhs(10, 20, 0, 40);
    constexpr vec4<T> rhs(10, -5, 3, 2);
    constexpr vec4<T> res(1, -4, 0, 20);
    static_assert((lhs / rhs) == res, "vec4(10, 20, 0, 40) / vec4(10, -5, 3, 2)");
  }
  {
    constexpr vec4<T> lhs(10, 20, 0, 40);
    constexpr vec4<T> rhs(10, -5, 3, 2);
    constexpr vec4<T> res(10, -5, 0, 2);
    static_assert(lhs.min(rhs) == res, "vec4(10, 20, 0, 40).min(vec4(10, -5, 3, 2))");
  }
  {
    constexpr vec4<T> lhs(10, 20, 0, 40);
    constexpr vec4<T> rhs(10, -5, 3, 2);
    constexpr vec4<T> res(10, 20, 3, 40);
    static_assert(lhs.max(rhs) == res, "vec4(10, 20, 0, 40).max(vec4(10, -5, 3, 2))");
  }
  {
    constexpr vec4<T> v;
    static_assert(v.distance(v) == 0, "wrong distance from center to center");
  }
  {
    constexpr vec4<T> lhs(-1, 2, -4, 3);
    constexpr vec4<T> center;
    static_assert(
        lhs.distance(center) == 10,
        "wrong distance from vec4(-1, 2, -4, 3) to center"
    );
    static_assert(
        lhs.distance(lhs) == 0,
        "wrong distance from vec4(-1, 2, -4, 3) to self"
    );
  }
  {
    constexpr vec4<T> lhs(-1, 2, -4, 3);
    constexpr vec4<T> rhs(1, 2, 4, 3);
    static_assert(
        lhs.distance(rhs) == 10,
        "wrong distance from vec4(-1, 2, -4, 3) to vec4(1, 2, 4, 3)"
    );
  }
  {
    constexpr vec4<T> lhs(-1, 2, -4, 3);
    constexpr vec4<T> res(-1, 1, -1, 1);
    static_assert(
        lhs.signum() == res,
        "expected vec4(-1, 2, -4, 3).signum() == vec4(-1, 1, -1, 1)"
    );
  }
}

template <typename... Ts> consteval void test_vec_compile_time() {
  (test_vec_compile_time_impl<Ts>(), ...);
}

template <typename... Ts> void test_vec_hash() { (test_hash<Ts>(), ...); }

int main() {
  test_vec<short, int, long, long long, float, double, long double>();
  test_vec_hash<short, int, long, long long>();
  test_vec_compile_time<short, int, long, long long, float, double, long double>();
  return 0;
}
