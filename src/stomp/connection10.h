#ifndef STOMP_CONNECTION_10_H
#define STOMP_CONNECTION_10_H

#include <memory>

#include "connection.h"
#include "protocol10.h"
#include "transport.h"

namespace stomp {
  class Connection10 : public BaseConnection, public Protocol10 {
  protected:
  public:
    Connection10(HostsAndPorts hostsAndPorts = {}, bool autoDecode = true, std::string encoding = "utf8", bool autoContentLength = true) :
      BaseConnection {std::make_shared<Transport>(hostsAndPorts, autoDecode, encoding)}, Protocol10 {BaseConnection::transport_, autoContentLength} {}
    void connect() {
      BaseConnection::transport_->start();
      Protocol10::connect();
    }
    void disconnect() {
      Protocol10::disconnect();
      BaseConnection::transport_->stop();
    }
  };
  using Connection10Ptr = std::shared_ptr<Connection10>;
}

#endif
