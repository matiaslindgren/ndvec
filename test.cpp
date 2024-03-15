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

template <typename T> void test_vec1() {
  std::println("test_vec1<{}>", demangle<T>());
  {
    ndvec1<T> v;
    assert(v.x() == T{}, std::format("ndvec1: default init x should be {}", T{}));
  }
  {
    ndvec1<T> v(1);
    assert(v.x() == 1, "ndvec1(1) => x should be 1");
  }
  {
    ndvec1<T> v(1);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec1{}", std::tuple{1}),
        "unexpected ndvec format"
    );
  }
  {
    ndvec1<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "ndvec1() <=> ndvec1() should compare equal");
  }
  {
    ndvec1<T> lhs(1), rhs(1);
    assert((lhs <=> rhs) == 0, "ndvec1(1) <=> ndvec1(1) should compare equal");
  }
  {
    ndvec1<T> lhs(1), rhs(2);
    assert((lhs <=> rhs) < 0, "ndvec1(1) <=> ndvec1(2) should compare less than");
  }
  {
    ndvec1<T> lhs(2), rhs(1);
    assert((lhs <=> rhs) > 0, "ndvec1(2) <=> ndvec1(1) should compare greater than");
  }
  {
    ndvec1<T> lhs(6), rhs(-2);
    lhs += rhs;
    assert_equal(lhs, ndvec1<T>(4), "ndvec1(6) + ndvec1(-2)");
  }
  {
    ndvec1<T> lhs(2), rhs(6);
    lhs -= rhs;
    assert_equal(lhs, ndvec1<T>(-4), "ndvec1(2) - ndvec1(6)");
  }
  {
    ndvec1<T> lhs(6), rhs(-2);
    lhs *= rhs;
    assert_equal(lhs, ndvec1<T>(-12), "ndvec1(6) * ndvec1(-2)");
  }
  {
    ndvec1<T> lhs(-6), rhs(-2);
    lhs /= rhs;
    assert_equal(lhs, ndvec1<T>(3), "ndvec1(-6) / ndvec1(-2)");
  }
  {
    std::istringstream is{"1"s};
    ndvec1<T> v;
    is >> v;
    assert_equal(v, ndvec1<T>(1), "parse string 1");
  }
}

template <typename T> void test_vec2() {
  std::println("test_vec2<{}>", demangle<T>());
  {
    ndvec2<T> v;
    assert(v.x() == T{}, std::format("ndvec2: default init x should be {}", T{}));
    assert(v.y() == T{}, std::format("ndvec2: default init y should be {}", T{}));
  }
  {
    ndvec2<T> v(1, 2);
    assert(v.x() == 1, "ndvec2(1, 2) => x should be 1");
    assert(v.y() == 2, "ndvec2(1, 2) => y should be 2");
  }
  {
    ndvec2<T> v(1, 2);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec2{}", std::tuple{1, 2}),
        "unexpected ndvec format"
    );
  }
  {
    ndvec2<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "ndvec2() <=> ndvec2() should compare equal");
  }
  {
    ndvec2<T> lhs(1, 1), rhs(1, 1);
    assert((lhs <=> rhs) == 0, "ndvec2(1, 1) <=> ndvec2(1, 1) should compare equal");
  }
  {
    ndvec2<T> lhs(1, 1), rhs(1, 2);
    assert((lhs <=> rhs) < 0, "ndvec2(1, 1) <=> ndvec2(1, 2) should compare less than");
  }
  {
    ndvec2<T> lhs(1, 1), rhs(2, 2);
    assert((lhs <=> rhs) < 0, "ndvec2(1, 1) <=> ndvec2(2, 2) should compare less than");
  }
  {
    ndvec2<T> lhs(1, 2), rhs(1, 1);
    assert(
        (lhs <=> rhs) > 0,
        "ndvec2(1, 2) <=> ndvec2(1, 1) should compare greater than"
    );
  }
  {
    ndvec2<T> lhs(2, 2), rhs(1, 1);
    assert(
        (lhs <=> rhs) > 0,
        "ndvec2(2, 2) <=> ndvec2(1, 1) should compare greater than"
    );
  }
  {
    ndvec2<T> lhs(6, -12), rhs(3, 2);
    lhs += rhs;
    assert_equal(lhs, ndvec2<T>(9, -10), "ndvec2(6, -12) + ndvec2(3, 2)");
  }
  {
    ndvec2<T> lhs(6, 12), rhs(-3, 2);
    lhs -= rhs;
    assert_equal(lhs, ndvec2<T>(9, 10), "ndvec2(6, 12) - ndvec2(-3, 2)");
  }
  {
    ndvec2<T> lhs(6, 12), rhs(3, -2);
    lhs *= rhs;
    assert_equal(lhs, ndvec2<T>(18, -24), "ndvec2(6, 12) * ndvec2(3, -2)");
  }
  {
    ndvec2<T> lhs(6, 12), rhs(3, 2);
    lhs /= rhs;
    assert_equal(lhs, ndvec2<T>(2, 6), "ndvec2(6, 12) / ndvec2(3, 2)");
  }
  {
    auto input{"1 2"s};
    std::istringstream is{input};
    ndvec2<T> v;
    is >> v;
    assert_equal(v, ndvec2<T>(1, 2), std::format("parse string '{}'", input));
  }
  {
    auto input{"1    2"s};
    std::istringstream is{input};
    ndvec2<T> v;
    is >> v;
    assert_equal(v, ndvec2<T>(1, 2), std::format("parse string '{}'", input));
  }
}

template <typename T> void test_vec3() {
  std::println("test_vec3<{}>", demangle<T>());
  {
    ndvec3<T> v;
    assert(v.x() == T{}, std::format("ndvec3: default init x should be {}", T{}));
    assert(v.y() == T{}, std::format("ndvec3: default init y should be {}", T{}));
    assert(v.z() == T{}, std::format("ndvec3: default init z should be {}", T{}));
  }
  {
    ndvec3<T> v(1, 2, 3);
    assert(v.x() == 1, "ndvec3(1, 2, 3) => x should be 1");
    assert(v.y() == 2, "ndvec3(1, 2, 3) => y should be 2");
    assert(v.z() == 3, "ndvec3(1, 2, 3) => z should be 3");
  }
  {
    ndvec3<T> v(1, 2, 3);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec3{}", std::tuple{1, 2, 3}),
        "unexpected ndvec format"
    );
  }
  {
    ndvec3<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "ndvec3() <=> ndvec3() should compare equal");
  }
  {
    ndvec3<T> lhs(1, 1, 1), rhs(1, 1, 1);
    assert(
        (lhs <=> rhs) == 0,
        "ndvec3(1, 1, 1) <=> ndvec3(1, 1, 1) should compare equal"
    );
  }
  {
    ndvec3<T> lhs(1, 1, 1), rhs(1, 1, 2);
    assert(
        (lhs <=> rhs) < 0,
        "ndvec3(1, 1, 1) <=> ndvec3(1, 1, 2) should compare less than"
    );
  }
  {
    ndvec3<T> lhs(1, 1, 1), rhs(1, 2, 2);
    assert(
        (lhs <=> rhs) < 0,
        "ndvec3(1, 1, 1) <=> ndvec3(1, 2, 2) should compare less than"
    );
  }
  {
    ndvec3<T> lhs(1, 1, 1), rhs(2, 2, 2);
    assert(
        (lhs <=> rhs) < 0,
        "ndvec3(1, 1, 1) <=> ndvec3(2, 2, 2) should compare less than"
    );
  }
  {
    ndvec3<T> lhs(1, 1, 2), rhs(1, 1, 1);
    assert(
        (lhs <=> rhs) > 0,
        "ndvec3(1, 1, 2) <=> ndvec3(1, 1, 1) should compare greater than"
    );
  }
  {
    ndvec3<T> lhs(1, 2, 2), rhs(1, 1, 1);
    assert(
        (lhs <=> rhs) > 0,
        "ndvec3(1, 2, 2) <=> ndvec3(1, 1, 1) should compare greater than"
    );
  }
  {
    ndvec3<T> lhs(2, 2, 2), rhs(1, 1, 1);
    assert(
        (lhs <=> rhs) > 0,
        "ndvec3(2, 2, 2) <=> ndvec3(1, 1, 1) should compare greater than"
    );
  }
  {
    ndvec3<T> lhs(-2, 0, 2), rhs(1, 2, 3);
    lhs += rhs;
    assert_equal(lhs, ndvec3<T>(-1, 2, 5), "ndvec3(-2, 0, 2) + ndvec3(1, 2, 3)");
  }
  {
    ndvec3<T> lhs(-2, 0, 2), rhs(1, 2, 3);
    lhs -= rhs;
    assert_equal(lhs, ndvec3<T>(-3, -2, -1), "ndvec3(-2, 0, 2) - ndvec3(1, 2, 3)");
  }
  {
    ndvec3<T> lhs(-2, 0, 2), rhs(1, 2, 3);
    lhs *= rhs;
    assert_equal(lhs, ndvec3<T>(-2, 0, 6), "ndvec3(-2, 0, 2) * ndvec3(1, 2, 3)");
  }
  {
    ndvec3<T> lhs(-2, 0, 6), rhs(1, 2, 3);
    lhs /= rhs;
    assert_equal(lhs, ndvec3<T>(-2, 0, 2), "ndvec3(-2, 0, 2) / ndvec3(1, 2, 3)");
  }
  {
    auto input{"1 2 3"s};
    std::istringstream is{input};
    ndvec3<T> v;
    is >> v;
    assert_equal(v, ndvec3<T>(1, 2, 3), std::format("parse string '{}'", input));
  }
  {
    auto input{"   1   -2    3"s};
    std::istringstream is{input};
    ndvec3<T> v;
    is >> v;
    assert_equal(v, ndvec3<T>(1, -2, 3), std::format("parse string '{}'", input));
  }
  {
    ndvec3<T> v(-1, 2, -3);
    assert_equal(v.abs(), ndvec3<T>(1, 2, 3), "ndvec3(-1, 2, -3).abs()");
  }
  {
    ndvec3<T> v(-1, 2, -3);
    assert_equal(v.signum(), ndvec3<T>(-1, 1, -1), "ndvec3(-1, 2, -3).signum()");
  }
  {
    ndvec3<T> v;
    assert_equal(v.sum(), 0, "ndvec3().sum()");
  }
  {
    ndvec3<T> v(-1, 2, -3);
    assert_equal(v.sum(), -2, "ndvec3(-1, 2, -3).sum()");
  }
  {
    ndvec3<T> v;
    assert_equal(v.distance(v), 0, "center distance to center");
  }
  {
    ndvec3<T> lhs(-1, 2, -3);
    assert_equal(lhs.distance(ndvec3<T>()), 6, "ndvec3(-1, 2, -3) distance to center");
    assert_equal(lhs.distance(lhs), 0, "ndvec3(-1, 2, -3) distance to self");
  }
  {
    ndvec3<T> lhs(-1, 2, -3);
    ndvec3<T> rhs(1, 2, 3);
    assert_equal(lhs.distance(rhs), 8, "ndvec3(-1, 2, -3) distance to ndvec3(1, 2, 3)");
  }
}

template <typename T> void test_vec4() {
  std::println("test_vec4<{}>", demangle<T>());
  {
    ndvec4<T> v;
    assert(v.x() == T{}, std::format("ndvec4: default init x should be {}", T{}));
    assert(v.y() == T{}, std::format("ndvec4: default init y should be {}", T{}));
    assert(v.z() == T{}, std::format("ndvec4: default init z should be {}", T{}));
    assert(v.w() == T{}, std::format("ndvec4: default init w should be {}", T{}));
  }
  {
    ndvec4<T> v(1, 2, 3, 4);
    assert(v.x() == 1, "ndvec4(1, 2, 3, 4) => x should be 1");
    assert(v.y() == 2, "ndvec4(1, 2, 3, 4) => y should be 2");
    assert(v.z() == 3, "ndvec4(1, 2, 3, 4) => z should be 3");
    assert(v.w() == 4, "ndvec4(1, 2, 3, 4) => w should be 4");
  }
  {
    ndvec4<T> v(1, 2, 3, 4);
    assert_equal(
        std::format("{}", v),
        std::format("ndvec4{}", std::tuple{1, 2, 3, 4}),
        "unexpected ndvec format"
    );
  }
  {
    ndvec4<T> lhs, rhs;
    assert((lhs <=> rhs) == 0, "ndvec4() <=> ndvec4() should compare equal");
  }
  {
    ndvec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs += rhs;
    assert_equal(
        lhs,
        ndvec4<T>(12, 12, 12, 12),
        "ndvec4(10, 10, 10, 10) + ndvec4(2, 2, 2, 2)"
    );
  }
  {
    ndvec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs -= rhs;
    assert_equal(
        lhs,
        ndvec4<T>(8, 8, 8, 8),
        "ndvec4(10, 10, 10, 10) - ndvec4(2, 2, 2, 2)"
    );
  }
  {
    ndvec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs *= rhs;
    assert_equal(
        lhs,
        ndvec4<T>(20, 20, 20, 20),
        "ndvec4(10, 10, 10, 10) * ndvec4(2, 2, 2, 2)"
    );
  }
  {
    ndvec4<T> lhs(10, 10, 10, 10), rhs(2, 2, 2, 2);
    lhs /= rhs;
    assert_equal(
        lhs,
        ndvec4<T>(5, 5, 5, 5),
        "ndvec4(10, 10, 10, 10) / ndvec4(2, 2, 2, 2)"
    );
  }
  {
    auto input{"   1   -2    3   -1000"s};
    std::istringstream is{input};
    ndvec4<T> v;
    is >> v;
    assert_equal(v, ndvec4<T>(1, -2, 3, -1000), std::format("parse string '{}'", input));
  }
}

template <typename T> void test_hash() {
  std::println("test_hash<{}>", demangle<T>());
  auto hash{std::hash<T>{}};
  auto width{std::numeric_limits<std::size_t>::digits};
  {
    ndvec1<T> v(0xabc);
    assert_equal(std::hash<ndvec1<T>>{}(v), hash(0xabc), "ndvec1(0xabc) hash");
  }
  {
    ndvec2<T> v(0xabc, 0x123);
    assert_equal(
        std::hash<ndvec2<T>>{}(v),
        (hash(0xabc) << 0) | (hash(0x123) << (width / 2)),
        "ndvec2(0xabc, 0x123) hash"
    );
  }
  {
    ndvec3<T> v(0xabc, 0x123, -0xfed);
    assert_equal(
        std::hash<ndvec3<T>>{}(v),
        (hash(0xabc) << 0) | (hash(0x123) << (width / 3))
            | (hash(-0xfed) << ((2 * (width / 3)))),
        "ndvec3(0xabc, 0x123, -0xfed) hash"
    );
  }
  {
    ndvec4<T> v(0xabc, 0x123, 0, -0xfed);
    assert_equal(
        std::hash<ndvec4<T>>{}(v),
        (hash(0xabc) << 0) | (hash(0x123) << (width / 4)) | (hash(0) << (2 * (width / 4)))
            | (hash(-0xfed) << (3 * (width / 4))),
        "ndvec4(0xabc, 0x123, 0, -0xfed) hash"
    );
  }
}

template <typename... Ts> void test_vec() {
  (test_vec1<Ts>(), ...);
  (test_vec2<Ts>(), ...);
  (test_vec3<Ts>(), ...);
  (test_vec4<Ts>(), ...);
}

template <typename... Ts> void test_vec_hash() { (test_hash<Ts>(), ...); }

int main() {
  test_vec<short, int, long, long long, float, double, long double>();
  test_vec_hash<short, int, long, long long>();
  return 0;
}
