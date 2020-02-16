#ifndef STOMP_TRANSPORT_H
#define STOMP_TRANSPORT_H

#include <memory>
#include <algorithm>
#include <cmath>

#include "base_transport.h"
#include "../socket/socket.h"

namespace stomp {
  using HostsAndPorts = std::vector<HostAndPortPtr>;
  using SocketPtr = std::shared_ptr<TCPSocket>;

  class Transport : public BaseTransport {
    // Represents a STOMP client 'transport'. Effectively this is the communications mechanism without the definition of
    // the protocol.
  protected:
    HostsAndPorts hostsAndPorts_ {};
    double reconnectSleepInitial_ {0.1};
    double reconnectSleepIncrease_ {0.5};
    double reconnectSleepJitter_ {0.1};
    double reconnectSleepMax_ {60.0};
    int reconnectAttemptsMax_ {3};
    int recvBytes {1024};
    SocketPtr socket {};
  public:
    Transport(HostsAndPorts hostsAndPorts = {}, bool autoDecode = true, std::string encoding = "utf8") :
      BaseTransport {autoDecode, encoding}, hostsAndPorts_ {hostsAndPorts} {
        if (hostsAndPorts_.empty()) hostsAndPorts_.push_back(std::make_shared<HostAndPort>("localhost", 61613));
      }
    virtual bool isConnected() {
      return (socket != nullptr) && BaseTransport::isConnected();
    }
    virtual void disconnectSocket() {
      running_ = false;
      // TODO maybe do socket shutdown
      currentHostAndPort_ = nullptr;
      socket = nullptr;
      this->notify(std::make_shared<Frame>(FRAME_DISCONNECTED));
    }
    virtual void send(std::string content) {
      if (socket) {
        // TODO use socket semaphore
        socket->send(content.c_str(), content.size());
      } else {
        throw SocketException {"Not connected!"};
      }
    }
    virtual std::string receive() {
      return socket->recv(recvBytes);
    }
    virtual void cleanup() {
      socket = nullptr;
    }
    double rand() {
      return 1.0 * std::rand() / RAND_MAX;
    }
    // Try connecting to the (host, port) tuples specified at construction time.
    virtual void attemptConnection() {
      connectionError_ = false;
      int sleepExp {1};
      int connectCount {0};
      while (running_ && socket == nullptr && (connectCount < reconnectAttemptsMax_ || reconnectAttemptsMax_ == -1)) {
        for (auto hostAndPort : hostsAndPorts_) {
          try {
            socket = std::make_shared<TCPSocket>(hostAndPort->first, std::to_string(hostAndPort->second));
            currentHostAndPort_ = hostAndPort;
            break;
          } catch (auto& e) {
            socket = nullptr;
            connectCount++;
          }
        }
        if (socket == nullptr) {
          int sleepDuration = (std::min(reconnectSleepMax_,
              ((reconnectSleepInitial_ / (1.0 + reconnectSleepIncrease_))
               * std::pow(1.0 + reconnectSleepIncrease_, sleepExp)))
          * (1.0 + rand() * reconnectSleepJitter_));
          // TODO sleep for sleepDuration
          if (sleepDuration < reconnectSleepMax_) sleepExp++;
        }
      }
      if (socket == nullptr) {
        throw SocketException {"Connection failed!"};
      }
    }
  };
}

#endif
