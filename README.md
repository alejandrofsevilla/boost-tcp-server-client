# [![LinuxWorkflow](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/Linux.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/Linux.yml) [![MacOsWorkflow](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/MacOs.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/MacOs.yml)
# Boost TCP Server/Client
Asynchronous [Boost.Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html) TCP server and client example. 

## Requirements
- C++17 compiler
- CMake 3.22.0
- Boost 1.74.0
- GoogleTest 1.11.0

## Usage
### Server
```cpp
#include <TcpServer.hpp>
#include <iostream>
#include <thread>

struct : TcpServer::Observer {
  void onConnectionAccepted(int id) {
    std::cout << "New client connected with id " << id << std::endl;
  };
  void onReceived(int id, const char* data, size_t size) {
    std::cout << "Data received from client with id " << id << ": ";
    std::cout.write(data, size);
    std::cout << std::endl;
  }
  void onConnectionClosed(int id) {
    std::cout << "Client disconnected with id " << id << std::endl;
  };
} observer;

boost::asio::io_context context;
std::thread thread{[&context]() { context.run(); }};

TcpServer server{context, observer};

constexpr uint16_t port{1234};
const auto protocol{boost::asio::ip::tcp::v4()};
server.listen(protocol, port);

server.startAcceptingConnections();

```
### Client 
```cpp
#include <TcpClient.hpp>
#include <iostream>
#include <thread>

struct : TcpClient::Observer {
  void onConnected() { std::cout << "Client was connected" << std::endl; };
  void onReceived(const char* data, size_t size) {
    std::cout << "Data received: ";
    std::cout.write(data, size);
    std::cout << std::endl;
  }
  void onDisconnected() {
    std::cout << "Client was disconnected " << std::endl;
  };
} observer;

boost::asio::io_context context;
std::thread thread{[&context]() { context.run(); }};

TcpClient client{context, observer};

constexpr uint16_t port{1234};
auto address{boost::asio::ip::address::from_string("127.0.0.1")};
client.connect({address, port});

```
## How to build
- Install dependencies.
  - linux 
   ```terminal
   sudo apt-get install libboost-dev
   sudo apt-get install libgtest-dev
   ```
  - macOs
   ```terminal
   brew install boost
   brew install googletest
   ```
- Clone repository.
   ```terminal
   git clone https://github.com/alejandrofsevilla/boost-tcp-server-client.git
   cd boost-tcp-server-client
   ```
- Build.
   ```terminal
   cmake -S . -B build
   cmake --build build
   ```
- Run tests.
   ```terminal
   ./build/tests/boost-tcp-server-client-tests 
   ```
