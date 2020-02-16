#ifndef STOMP_EXCEPTION_H
#define STOMP_EXCEPTION_H

#include <stdexcept>

namespace stomp {
  using ConnectFailedException = std::runtime_error;
}

#endif
