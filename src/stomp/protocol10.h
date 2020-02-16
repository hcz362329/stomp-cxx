#ifndef STOMP_PROTOCOL_10_H
#define STOMP_PROTOCOL_10_H

#include "listener.h"
#include "base_transport.h"
#include "exception.h"

namespace stomp {
using OptString = std::optional<std::string>;

class Protocol10 : public ConnectionListener {
  protected:
    TransportPtr transport_;
    bool autoContentLength_ {true};
    std::string version_ {"1.0"};
  public:
    Protocol10(TransportPtr transport, bool autoContentLength = true) :
      transport_ {transport}, autoContentLength_ {autoContentLength} {
        transport_->setListener("protocol-listener", std::make_shared<ConnectionListener>(*this));
      }
    // Encode and send a stomp frame through the underlying transport.
    void sendFrame(std::string cmd, Headers headers = {}, std::string body="") {
      FramePtr frame = std::make_shared<Frame>(cmd, headers, body);
      transport_->transmit(frame);
    }
    // Abort a transaction.
    void abort(std::string transaction, Headers headers = {}) {
      headers[HEADER_TRANSACTION] = transaction;
      this->sendFrame(FRAME_ABORT, headers);
    }
    // Acknowledge 'consumption' of a message by id.
    void ack(std::string id, OptString transaction = std::nullopt, OptString receipt = std::nullopt) {
      Headers headers {{HEADER_MESSAGE_ID, id}};
      if (transaction) headers[HEADER_TRANSACTION] = transaction.value();
      if (receipt) headers[HEADER_RECEIPT] = receipt.value();
      this->sendFrame(FRAME_ACK, headers);
    }
    // Begin a transaction.
    std::string begin(OptString transaction = std::nullopt, Headers headers = {}) {
      std::string transactionId {transaction? transaction.value(): generateUuid()};
      headers[HEADER_TRANSACTION] = transactionId;
      this->sendFrame(FRAME_BEGIN, headers);
      return transactionId;
    }
    void commit(std::string transaction, Headers headers = {}) {
      headers[HEADER_TRANSACTION] = transaction;
      this->sendFrame(FRAME_COMMIT, headers);
    }
    void connect(OptString username, OptString passcode, bool wait = false, Headers headers = {}) {
      headers[HEADER_ACCEPT_VERSION] = version_;
      if (username) headers[HEADER_LOGIN] = username.value();
      if (passcode) headers[HEADER_PASSCODE] = passcode.value();
      this->sendFrame(FRAME_CONNECT, headers);
      if (wait) {
        transport_->waitForConnection();
        if (transport_->hasConnectError()) throw ConnectFailedException("connect failed");
      }
    }
    void disconnect(OptString receipt = std::nullopt, Headers headers = {}) {
      std::string receiptId {receipt? receipt.value(): generateUuid()};
      headers[HEADER_RECEIPT] = receiptId;
      transport_->setReceipt(receiptId, FRAME_DISCONNECT);
      this->sendFrame(FRAME_DISCONNECT, headers);
    }
    void send(std::string destination, std::string body, OptString contentType = std::nullopt, Headers headers = {}) {
      headers[HEADER_DESTINATION] = destination;
      if (contentType) headers[HEADER_CONTENT_TYPE] = contentType.value();
      if (autoContentLength_ && headers.count(HEADER_CONTENT_LENGTH) == 0) {
        headers[HEADER_CONTENT_LENGTH] = body.size();
      }
      this->sendFrame(FRAME_SEND, headers, body);
    }
    void subscribe(std::string destination, OptString id = std::nullopt, std::string ack = "auto", Headers headers = {}) {
      headers[HEADER_DESTINATION] = destination;
      if (id) headers[HEADER_ID] = id.value();
      headers[HEADER_ACK] = ack;
      this->sendFrame(FRAME_SUBSCRIBE, headers);
    }
    void unsubscribeDestination(std::string destination, Headers headers = {}) {
      headers[HEADER_DESTINATION] = destination;
      this->sendFrame(FRAME_UNSUBSCRIBE, headers);
    }
    void unsubscribeId(std::string id, Headers headers = {}) {
      headers[HEADER_ID] = id;
      this->sendFrame(FRAME_UNSUBSCRIBE, headers);
    }
  };
}

#endif
