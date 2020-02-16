#ifndef STOMP_FRAME_H
#define STOMP_FRAME_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>

#define FRAME_CONNECTING               "CONNECTING"
#define FRAME_CONNECTED                "CONNECTED"
#define FRAME_DISCONNECTED             "DISCONNECTED"
#define FRAME_DISCONNECT               "DISCONNECT"
#define FRAME_HEARTBEAT_TIMEOUT        "HEARTBEAT_TIMEOUT"
#define FRAME_BEFORE_MESSAGE           "BEFORE_MESSAGE"
#define FRAME_MESSAGE                  "MESSAGE"
#define FRAME_RECEIPT                  "RECEIPT"
#define FRAME_ERROR                    "ERROR"
#define FRAME_SEND                     "SEND"
#define FRAME_HEARTBEAT                "HEARTBEAT"
#define FRAME_RECEIVER_LOOP_COMPLETED  "RECEIVER_LOOP_COMPLETED"
#define FRAME_ABORT                    "ABORT"

#define HEADER_RECEIPT                 "receipt"
#define HEADER_RECEIPT_ID              "receipt-id"
#define HEADER_CONTENT_LENGTH          "content-length"
#define HEADER_TRANSACTION             "transaction"

namespace stomp {
  using Headers = std::map<std::string,std::string>;

  class Frame {
  protected:
    std::string cmd_ {};
    Headers headers_ {};
    std::string body_ {};
  public:
    Frame(std::string cmd, Headers headers, std::string body) :
      cmd_ {cmd}, headers_ {headers}, body_ {body} {}
    Frame() {}
    std::string getCmd() const { return cmd_; }
    Headers getHeaders() const { return headers_; }
    void setHeaders(Headers headers) { headers_ = headers; }
    std::string getBody() const { return body_; }
    void setBody(std::string body) { body_ = body; }
    std::string getReceiptIdHeader() const { return headers_[HEADER_RECEIPT_ID]; }
    bool hasReceiptHeader() const { return headers_.count(HEADER_RECEIPT); }
    std::string getReceiptHeader() const { return headers_[HEADER_RECEIPT]; }
    std::string getContents() const {
      std::stringstream s {};
      s << cmd_ << std::endl;
      for (auto& [key, value] : headers_) {
        s << key << ":" << value << std::endl;
      }
      s << std::endl;
      s << body_;
      return s.str();
    }
  };
  using FramePtr = std::shared_ptr<Frame>;
}

#endif
