# `ndvec`: constexpr Euclidean vector

[godbolt](https://godbolt.org/z/zhxMjdPh8)

## Description

Consider this code from main.cpp:
```c++
#include "ndvec.hpp"

int main() {
  using ndvec::ndvec;
  ndvec v1(1, -2, 3);
  ndvec v2(-3, 2, -1);
  ndvec v3(5, 6, 3);
  ndvec v4(1, 2, -3, -2, 4, 10);
  return v4.signum().sum() + (v1 - v2).sum() * v1.distance(v3);
}
```
Compiling with Clang 18
```
make CXX=clang-18 main
```
The program exits with 50
```
./main || echo $?
# output: 50
```
Disassembling the output
```sh
objdump --disassemble main | grep --context 5 '<main>'
```
We see the main function compiles to just `return 50`:
```
 7cc:	d503201f 	nop

00000000000007d0 <frame_dummy>:
 7d0:	17ffffdc 	b	740 <register_tm_clones>

00000000000007d4 <main>:
 7d4:	52800640 	mov	w0, #0x32                  	// #50
 7d8:	d65f03c0 	ret

Disassembly of section .fini:
```

## Run in Docker

```
./run_image.sh
```

## Test

```
make CXX=clang-18 test && ./test
```

## Advent of Code examples

### `minmax` over a range of points to find grid corners
[Year 2018 day 06](https://github.com/matiaslindgren/advent-of-code-cpp/blob/abf152bc6e519a1262b971950e111d8638b72358/src/2018/06.cpp#L6-L21)
```c++
using Vec2 = ndvec::vec2<int>;
using Points = std::vector<Vec2>;

auto find_grid_corners(const Points& points) {
  return std::ranges::fold_left(
      points,
      std::pair{points.at(0), points.at(0)},
      [](const auto& corners, const Vec2& p) {
        auto&& [tl, br]{corners};
        return std::pair{tl.min(p), br.max(p)};
      }
  );
}
```

### Hashing `ndvec::vec2<int>` objects: BFS over 2D grid

[Year 2016 day 24](https://github.com/matiaslindgren/advent-of-code-cpp/blob/349aafbb6d8e41f0795730751e71530d4d446636/src/2016/24.cpp#L15-L40)
```c++
using Vec2 = ndvec::vec2<int>;

struct Distance {
  int dst{};
  int len{};
};

struct Grid {
  std::unordered_map<Vec2, int> targets;
  std::unordered_map<Vec2, char> tiles;
  std::size_t width{};

  [[nodiscard]]
  auto bfs(const Vec2 begin) const {
    std::vector<Distance> dist2target;
    std::unordered_set<Vec2> visited;
    for (std::deque q{std::pair{begin, 0}}; not q.empty(); q.pop_front()) {
      auto [pos, len]{q.front()};
      if (not tiles.contains(pos) or tiles.at(pos) == '#') {
        continue;
      }
      if (auto [_, is_new]{visited.insert(pos)}; is_new) {
        if (targets.contains(pos)) {
          dist2target.emplace_back(targets.at(pos), len);
        }
        for (Vec2 adj : pos.adjacent()) {
          q.emplace_back(adj, len + 1);
        }
      }
    }
    return dist2target;
  }
};
```


### Parse a grid of ASCII digits

[Year 2022 day 08](https://github.com/matiaslindgren/advent-of-code-cpp/blob/abf152bc6e519a1262b971950e111d8638b72358/src/2022/08.cpp#L5C1-L46C2)
```c++
using Vec2 = ndvec::vec2<int>;
using Grid = std::unordered_map<Vec2, int>;

Grid parse_grid(std::istream& is) {
  Grid g;
  Vec2 p;
  for (std::string line; std::getline(is, line); p.y() += 1) {
    for (p.x() = 0; unsigned char ch : line) {
      if (not std::isdigit(ch)) {
        throw std::runtime_error("all non-whitespace input must be digits");
      }
      g[p] = ch - '0';
      p.x() += 1;
    }
  }
  return g;
}
```

### "game-of-life-like" grid simulation

[Year 2018 day 18](https://github.com/matiaslindgren/advent-of-code-cpp/blob/abf152bc6e519a1262b971950e111d8638b72358/src/2018/18.cpp#L18-L67)
```c++
using Vec2 = ndvec::vec2<int>;

enum class Tile : char {
  open = '.',
  tree = '|',
  yard = '#',
};

struct Grid {
  std::map<Vec2, Tile> tiles;

  [[nodiscard]]
  int count_adjacent(const Vec2& center, Tile t) const {
    int n{};
    for (Vec2 d(-1, -1); d.x() <= 1; d.x() += 1) {
      for (d.y() = -1; d.y() <= 1; d.y() += 1) {
        if (Vec2 p{center + d}; p != center and tiles.contains(p) and tiles.at(p) == t) {
          n += 1;
        }
      }
    }
    return n;
  }

  [[nodiscard]]
  Grid step() const {
    Grid after{*this};
    for (auto&& [p, tile] : tiles) {
      const auto n_trees{count_adjacent(p, Tile::tree)};
      const auto n_yards{count_adjacent(p, Tile::yard)};
      switch (tile) {
        case Tile::open: {
          if (n_trees >= 3) {
            after.tiles[p] = Tile::tree;
          }
        } break;
        case Tile::tree: {
          if (n_yards >= 3) {
            after.tiles[p] = Tile::yard;
          }
        } break;
        case Tile::yard: {
          if (n_yards > 0 and n_trees > 0) {
            after.tiles[p] = Tile::yard;
          } else {
            after.tiles[p] = Tile::open;
          }
        } break;
      }
    }
    return after;
  }
};
```

[Year 2020 day 12](https://github.com/matiaslindgren/advent-of-code-cpp/blob/abf152bc6e519a1262b971950e111d8638b72358/src/2020/12.cpp#L5-L62)
```c++
using Vec2 = ndvec::vec2<double>;

struct Action {
  enum class Type : char {
    north = 'N',
    south = 'S',
    east = 'E',
    west = 'W',
    left = 'L',
    right = 'R',
    forward = 'F',
  } type{};
  int value{};
};

auto deg2rad(int deg) {
  return deg * std::numbers::pi_v<double> / 180.0;
}

auto rotate(const Vec2& p, int deg) {
  const auto cos{std::round(std::cos(deg2rad(deg)))};
  const auto sin{std::round(std::sin(deg2rad(deg)))};
  return Vec2(p.x() * cos - p.y() * sin, p.x() * sin + p.y() * cos);
}

auto find_part1(const auto& actions) {
  using Type = Action::Type;

  Vec2 pos(0, 0);
  Vec2 dir(1, 0);
  for (const Action& a : actions) {
    switch (a.type) {
      case Type::north: {
        pos.y() += a.value;
      } break;
      case Type::south: {
        pos.y() -= a.value;
      } break;
      case Type::east: {
        pos.x() += a.value;
      } break;
      case Type::west: {
        pos.x() -= a.value;
      } break;
      case Type::left: {
        dir = rotate(dir, a.value);
      } break;
      case Type::right: {
        dir = rotate(dir, -a.value);
      } break;
      case Type::forward: {
        pos.x() += dir.x() * a.value;
        pos.y() += dir.y() * a.value;
      } break;
    }
  }
  return pos.distance(Vec2());
}
```
