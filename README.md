# TCP Server/Client
Asynchronous TCP server and client using Boost.Asio. 

## Requirements
- C++17
- cmake 3.22.0
- boost 1.74.0
- googletest 1.11.0

## Example
### Server:
```cpp
#include <TcpServer.hpp>
#include <iostream>
#include <thread>
\\...
struct : TcpServer::Observer {
  void onConnectionAccepted(int id) {
    std::cout << "New client connected with id " << id << std::endl;
  };
  void onReceived(int id, const char* data, size_t size) {
    std::cout << "Data recived from client with id " << id << ": ";
    std::cout.write(data, size);
    std::cout << std::endl;
  }
  void onConnectionClosed(int id) {
    std::cout << "Client disconnected with id " << id << std::endl;
  };
} observer;

boost::asio::io_context context;
TcpServer server{context, observer};

constexpr uint16_t port{1234};
const auto protocol{boost::asio::ip::tcp::v4()};
server.listen(protocol, port);

server.startAcceptingConnections();
std::thread thread{[&context]() { context.run(); }};
\\...
```
