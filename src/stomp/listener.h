#ifndef STOMP_CONNECTION_LISTENER_H
#define STOMP_CONNECTION_LISTENER_H

extern "C"
{
#include <uuid/uuid.h>
}

#include <string>
#include <memory>

#include "frame.h"

namespace stomp {
  class ConnectionListener {
    // This class should be used as a base class for objects registered
    // using Connection.setListener().
  public:
    virtual ~ConnectionListener() = default;
    virtual void notify(FramePtr frame, HostAndPortPtr hostAndPort = nullptr) {
      std::string frameType = frame->getCmd();
      if (frameType == FRAME_CONNECTING) {
        this->onConnecting(hostAndPort);
      } else if (frameType == FRAME_CONNECTED) {
        this->onConnected(frame);
      } else if (frameType == FRAME_DISCONNECTED) {
        this->onDisconnected();
      } else if (frameType == FRAME_HEARTBEAT_TIMEOUT) {
        this->onHeartbeatTimeout();
      } else if (frameType == FRAME_BEFORE_MESSAGE) {
        this->onBeforeMessage(frame);
      } else if (frameType == FRAME_MESSAGE) {
        this->onMessage(frame);
      } else if (frameType == FRAME_RECEIPT) {
        this->onReceipt(frame);
      } else if (frameType == FRAME_ERROR) {
        this->onError(frame);
      } else if (frameType == FRAME_SEND) {
        this->onSend(frame);
      } else if (frameType == FRAME_HEARTBEAT) {
        this->onHeartbeat();
      } else if (frameType == FRAME_RECEIVER_LOOP_COMPLETED) {
        this->onReceiverLoopCompleted(frame);
      }
    }
    std::string generateUuid() {
      uuid_t uuid;
      uuid_generate_random ( uuid );
      char s[37];
      uuid_unparse ( uuid, s );
      return s;
    }
    // Called by the STOMP connection once a TCP/IP connection to the
    // STOMP server has been established or re-established. Note that
    // at this point, no connection has been established on the STOMP
    // protocol level. For this, you need to invoke the "connect"
    // method on the connection.
    virtual void onConnecting(HostAndPortPtr hostAndPort) {}
    // Called by the STOMP connection when a CONNECTED frame is
    // received (after a connection has been established or
    // re-established).
    virtual void onConnected(FramePtr frame) {}
    // Called by the STOMP connection when a TCP/IP connection to the
    // STOMP server has been lost.  No messages should be sent via
    // the connection until it has been reestablished.
    virtual void onDisconnected() {}
    // Called by the STOMP connection when a heartbeat message has not been
    // received beyond the specified period.
    virtual void onHeartbeatTimeout() {}
    // Called by the STOMP connection before a message is returned to the client app. Returns a tuple
    // containing the headers and body (so that implementing listeners can pre-process the content).
    virtual void onBeforeMessage(FramePtr frame) {}
    // Called by the STOMP connection when a MESSAGE frame is received.
    virtual void onMessage(FramePtr frame) {}
    // Called by the STOMP connection when a RECEIPT frame is
    // received, sent by the server if requested by the client using
    // the 'receipt' header.
    virtual void onReceipt(FramePtr frame) {}
    // Called by the STOMP connection when an ERROR frame is received.
    virtual void onError(FramePtr frame) {}
    // Called by the STOMP connection when it is in the process of sending a message.
    virtual void onSend(FramePtr frame) {}
    // Called on receipt of a heartbeat.
    virtual void onHeartbeat() {}
    // Called when the connection receiver_loop has finished.
    virtual void onReceiverLoopCompleted(FramePtr frame) {}
  };
  using ConnectionListenerPtr = std::shared_ptr<ConnectionListener>;
}

#endif
