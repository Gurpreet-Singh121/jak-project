#include "ReplClient.h"

#include "common/cross_sockets/XSocket.h"

#include "third-party/fmt/core.h"
#include "common/versions.h"

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

void ReplClient::eval(std::string form) {
  if (!is_connected()) {
    return;
  }
  // TODO - split this up into two writes
  u32 dataLength = form.length();
  ReplServerHeader header = {dataLength, ReplServerMessageType::EVAL};

  auto const ptr = reinterpret_cast<char*>(&header);
  std::vector<char> buffer(ptr, ptr + sizeof header);

  buffer.insert(buffer.end(), form.begin(), form.end());

  int result = write_to_socket(client_socket, buffer.data(), buffer.size());
  if (result == -1) {
    // TODO - log
    disconnect();
  }
}
