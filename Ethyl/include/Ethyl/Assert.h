#ifndef ETHYL_ASSERT_H
#define ETHYL_ASSERT_H

#include <stdexcept>

#define ETHYL_BREAK(_str) throw ::std::runtime_error(_str)
#define ETHYL_ASSERT(_b, _str) { if (!_b) ETHYL_BREAK(_str); }

#endif // ETHYL_ASSERT_H