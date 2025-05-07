# [![LinuxBuildWorkflow](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/LinuxBuild.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/LinuxBuild.yml?event=push) [![TestsWorkflow](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/LinuxBuildAndTest.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-tcp-server-client/actions/workflows/LinuxBuildAndTest.yml?event=push)
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
struct : TcpServer::Observer {
  void onConnectionAccepted(int id) {
    std::cout << "New client connected with id " << id << std::endl;
  };
  void onConnectionClosed(int id) {
    std::cout << "Client " << id << " disconnected" << std::endl;
  };
  void onReceived(int id, const char* data, size_t size) {
    std::cout << "Data received from client " << id << ": ";
    std::cout.write(data, size);
    std::cout << std::endl;
  }
} observer;

boost::asio::io_context context;
std::thread thread{[&context]() { context.run(); }};

TcpServer server{context, observer};
server.listen(boost::asio::ip::tcp::v4(), 1234);
server.startAcceptingConnections();

```
### Client 
```cpp
struct : TcpClient::Observer {
  void onConnected() { std::cout << "Client connected" << std::endl; };
  void onDisconnected() { std::cout << "Client disconnected" << std::endl; };
  void onReceived(const char* data, size_t size) {
    std::cout << "Data received from server: ";
    std::cout.write(data, size);
    std::cout << std::endl;
  }
} observer;

boost::asio::io_context context;
std::thread thread{[&context]() { context.run(); }};

TcpClient client{context, observer};
client.connect({boost::asio::ip::address::from_string("127.0.0.1"), 1234});
```
## Build and test
- Install dependencies.
   ```terminal
   sudo apt-get install libboost-dev
   sudo apt-get install libgtest-dev
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
### Include in CMake project
   ```cmake
   include(FetchContent)
   Fetchcontent_Declare(boost-tcp-server-client
     GIT_REPOSITORY http://github.com/alejandrofsevilla/boost-tcp-server-client.git
     GIT_TAG "main")
   FetchContent_Makeavailable(boost-tcp-server-client)

   target_link_libraries(${PROJECT_NAME} PRIVATE boost-tcp-server-client)
   ```
## Design

```mermaid
classDiagram
    class C_0008186492458287629887["TcpConnection"]
    class C_0008186492458287629887 {
        +close() : void
        -doRead() : void
        -doWrite() : void
        +send(const char * data, size_t size) : void
        +startReading() : void
        -m_id : int
        -m_isReading : bool
        -m_isWritting : bool
        -m_readBuffer : boost::asio::streambuf
        -m_socket : boost::asio::ip::tcp::socket
        -m_writeBuffer : boost::asio::streambuf
        -m_writeBufferMutex : std::mutex
    }
    class C_0009297713916002777393["TcpConnection::Observer"]
    class C_0009297713916002777393 {
        +onConnectionClosed(int connectionId) : void
        +onReceived(int connectionId, const char * data, size_t size) : void
    }
    class C_0015464091438354772672["TcpClient"]
    class C_0015464091438354772672 {
        +connect(const boost::asio::ip::tcp::endpoint & endpoint) : void
        +disconnect() : void
        -onConnectionClosed(int connectionId) : void
        -onReceived(int connectionId, const char * data, size_t size) : void
        +send(const char * data, size_t size) : void
        -m_ioContext : boost::asio::io_context &
    }
    class C_0013920056604237995185["TcpClient::Observer"]
    class C_0013920056604237995185 {
        +onConnected() : void
        +onDisconnected() : void
        +onReceived(const char * data, size_t size) : void
    }
    class C_0002490684498279635516["TcpServer"]
    class C_0002490684498279635516 {
        +close() : void
        -doAccept() : void
        +listen(const boost::asio::ip::tcp & protocol, uint16_t port) : bool
        -onConnectionClosed(int connectionId) : void
        -onReceived(int connectionId, const char * data, size_t size) : void
        +send(int connectionId, const char * data, size_t size) : void
        +startAcceptingConnections() : void
        -m_acceptor : boost::asio::ip::tcp::acceptor
        -m_connectionCount : int
        -m_isAccepting : bool
        -m_isClosing : bool
    }
    class C_0004458731545202809661["TcpServer::Observer"]
    class C_0004458731545202809661 {
        +onConnectionAccepted(int connectionId) : void
        +onConnectionClosed(int connectionId) : void
        +onReceived(int connectionId, const char * data, size_t size) : void
    }
    C_0008186492458287629887 --> C_0009297713916002777393 : -m_observer
    C_0015464091438354772672 --> C_0008186492458287629887 : -m_connection
    C_0015464091438354772672 --> C_0013920056604237995185 : -m_observer
    C_0002490684498279635516 o-- C_0008186492458287629887 : -m_connections
    C_0002490684498279635516 --> C_0004458731545202809661 : -m_observer
    C_0009297713916002777393 <|-- C_0015464091438354772672 : 
    C_0009297713916002777393 <|-- C_0002490684498279635516 : 

%% Generated with clang-uml, version 0.6.0
%% LLVM version Ubuntu clang version 15.0.
```
