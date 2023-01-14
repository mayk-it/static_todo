# static_todo

[![Tests (Ubuntu)](https://github.com/mayk-it/static_todo/workflows/ubuntu/badge.svg)](https://github.com/mayk-it/static_todo/actions/workflows/ubuntu.yml)
[![Tests (Mac)](https://github.com/mayk-it/static_todo/workflows/macos/badge.svg)](https://github.com/mayk-it/static_todo/actions/workflows/macos.yml)
[![Tests (Windows)](https://github.com/mayk-it/static_todo/workflows/windows/badge.svg)](https://github.com/mayk-it/static_todo/actions/workflows/windows.yml)
[![License](https://img.shields.io/github/license/mayk-it/static_todo.svg)](LICENSE)

`static_todo` is a C++20 header which exports a `TODO_BEFORE` macro,
which can be used to create compilation "time bombs", so that
compilation breaks when a `TODO_BEFORE` is not fixed before the
given deadline.

Inspired by: https://github.com/aurelienrb/cpp-static-todo

## Features

* C++20
* Header-only
* Dependency-free

## Examples

- Example when a TODO_BEFORE goes past its deadline:
  ```bash
  /Users/jatin/dev/static_todo/example/example.cpp:3:1: error: static_assert failed due to requirement '!static_todo::should_break_compilation("Jan 10 2023", 2023, static_todo::get_month_from_string("Jan"), "jatin", "jatin")' "TODO: update this example to pass the CI"
  TODO_BEFORE (Jan, 2023, "jatin", "update this example to pass the CI");
  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /Users/jatin/dev/static_todo/include/static_todo/static_todo.hpp:279:5: note: expanded from macro 'TODO_BEFORE'
      static_assert (! static_todo::should_break_compilation (__DATE__, year, static_todo::get_month_from_string (#month), STATIC_TODO_GIT_USERNAME, user_query), "TODO: " msg); \
      ^              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  1 error generated.
  ```

- Example when a TODO_BEFORE approaches its deadline (clang only):
  ```bash
  /Users/jatin/dev/static_todo/example/example.cpp:3:1: warning: TODO_BEFORE deadline coming soon! [-Wuser-defined-warnings]
  TODO_BEFORE (Jan, 2023, "jatin", "update this example to pass the CI");
  ^
  /Users/jatin/dev/static_todo/include/static_todo/static_todo.hpp:280:234: note: expanded from macro 'TODO_BEFORE'
  auto STATIC_TODO_UNIQUE_NAME (static_todo_warner_) = static_todo::static_warn (! static_todo::should_warn_about_upcoming_deadline (__DATE__, year, static_todo::get_month_from_string (#month), STATIC_TODO_GIT_USERNAME, user_query))
  ^
  /Users/jatin/dev/static_todo/include/static_todo/static_todo.hpp:247:55: note: from 'diagnose_if' attribute on 'static_warn':
  consteval int static_warn (bool test) __attribute__ ((diagnose_if (! (test), "TODO_BEFORE deadline coming soon!", "warning")))
  ```
  
## Options

When using `static_todo` with CMake, the CMake script will
use `git` to get the user's Git username. If you'd like
to override this, you could call CMake with `-DSTATIC_TODO_GIT_USERNAME="My Name"`.

The CMake option `STATIC_TODO_DISABLE` can be used to disable
`static_todo` in case you need to make a build and don't want
to fight with your expired `TODO_BEFORE`s.

The CMake option `STATIC_TODO_ENABLE_DEADLINE_APPROACHING_WARNINGS`
can be used to turn on or off the "deadline approaching" warnings.
Even though these warnings are on by default, they are currently only
function with Clang.

## Integration

You should add required file [static_todo.hpp](include/static_todo/static_todo.hpp).

You can also use something like [CPM](https://github.com/TheLartians/CPM) which is based on CMake's `Fetch_Content` module.

```cmake
CPMAddPackage(
    NAME static_todo
    GITHUB_REPOSITORY mayk-it/static_todo
    GIT_TAG x.y.z # Where `x.y.z` is the release version you want to use.
)
```

## Compiler compatibility

* Visual Studio >= 2019
* Xcode >= 11
* GCC >= 11

## License

`static_todo` is licensed under the MIT license. Enjoy!
