#ifndef ETHYL_ASSERT_H
#define ETHYL_ASSERT_H

#include <stdexcept>
#include <fmt/core.h>

// #define ETHYL_BREAK(_str) (fprintf(stderr, "%s", _str), std::exit(EXIT_FAILURE))
#define ETHYL_BREAK(_str, ...) throw ::std::runtime_error(fmt::format((_str), __VA_ARGS__));
#define ETHYL_ASSERT(_b, _str, ...) { if (!(_b)) ETHYL_BREAK((_str), __VA_ARGS__); }

#endif // ETHYL_ASSERT_H