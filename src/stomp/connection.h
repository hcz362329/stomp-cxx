#ifndef STOMP_CONNECTION_H
#define STOMP_CONNECTION_H

#include "publisher.h"
#include "base_transport.h"

namespace stomp {
  class BaseConnection : public Publisher {
  protected:
    TransportPtr transport_;
  public:
    BaseConnection(TransportPtr transport) : transport_ {transport} {}
    virtual void setListener(std::string name, ConnectionListenerPtr listener) {
      transport_->setListener(name, listener);
    }
    virtual void removeListener(std::string name) {
      transport_->removeListener(name);
    }
    virtual ConnectionListenerPtr getListener(std::string name) {
      return transport_->getListener(name);
    }
    virtual bool isConnected() { return transport_->isConnected(); }
    virtual void setReceipt(std::string receiptId, std::optional<std::string> value) {
      transport_->setReceipt(receiptId, value);
    }
  };
}

#endif
