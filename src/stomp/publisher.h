#ifndef STOMP_PUBLISHER_H
#define STOMP_PUBLISHER_H

#include <string>
#include <memory>
#include <map>

#include "listener.h"

namespace stomp {
  class Publisher {
    // A registry of listeners
  protected:
    std::map<std::string,ConnectionListenerPtr> listeners_ {};
  public:
    virtual ~Publisher() = default;
    // Set a named listener to use with this connection.
    virtual void setListener(std::string name, ConnectionListenerPtr listener) = 0;
    // Remove a listener.
    virtual void removeListener(std::string name) = 0;
    // Return the named listener.
    virtual ConnectionListenerPtr getListener(std::string name) = 0;
  };
  using PublisherPtr = std::shared_ptr<Publisher>;
}

#endif
