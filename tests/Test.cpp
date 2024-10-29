#include <gtest/gtest.h>

#include <TcpClient.hpp>
#include <TcpServer.hpp>
#include <algorithm>
#include <functional>
#include <random>
#include <thread>

namespace {
inline std::string generateRandomString(size_t size) {
  std::random_device rd;
  std::mt19937 eng{rd()};
  std::uniform_int_distribution<int> dist;
  std::string str(size, {});
  std::generate(str.begin(), str.end(), std::bind(dist, eng));
  return str;
}
} // namespace

TEST(TcpTest, ServerListens) {
  constexpr uint16_t port{1234};
  const auto protocol{boost::asio::ip::tcp::v4()};
  boost::asio::io_context context;
  TcpServer::Observer observer;
  TcpServer server{context, observer};
  EXPECT_EQ(server.listen(protocol, port), true);
}

TEST(TcpTest, ServerAcceptsAndClientConnects) {
  constexpr uint16_t port{1234};
  const auto protocol{boost::asio::ip::tcp::v4()};
  boost::asio::io_context context;
  struct : TcpServer::Observer {
    bool hasAccepted{false};
    void onConnectionAccepted(int) override { hasAccepted = true; };
  } serverObserver;
  TcpServer server{context, serverObserver};
  server.listen(protocol, port);
  server.startAcceptingConnections();
  std::thread thread{[&context]() { context.run(); }};
  struct : TcpClient::Observer {
    bool isConnected{false};
    void onConnected() override { isConnected = true; };
  } clientObserver;
  TcpClient client{context, clientObserver};
  client.connect({protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(serverObserver.hasAccepted, true);
  EXPECT_EQ(clientObserver.isConnected, true);
  context.stop();
  thread.join();
}

TEST(TcpTest, ClientSends) {
  constexpr uint16_t port{1234};
  constexpr size_t messageSize{5000};
  const auto protocol{boost::asio::ip::tcp::v4()};
  boost::asio::io_context context;
  struct : TcpServer::Observer {
    std::string msg{};
    size_t messageCount{0};
    void onReceived(int, const char *data, size_t size) override {
      msg.append(data, size);
    };
  } serverObserver;
  TcpServer server{context, serverObserver};
  server.listen(protocol, port);
  server.startAcceptingConnections();
  std::thread thread{[&context]() { context.run(); }};
  TcpClient::Observer clientObserver;
  TcpClient client{context, clientObserver};
  client.connect({protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto msg{generateRandomString(messageSize)};
  client.send(msg.c_str(), msg.size());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(msg, serverObserver.msg);
  context.stop();
  thread.join();
}

TEST(TcpTest, ServerSends) {
  constexpr uint16_t port{1234};
  constexpr size_t messageSize{5000};
  const auto protocol{boost::asio::ip::tcp::v4()};
  boost::asio::io_context context;
  struct : TcpServer::Observer {
    int connectionId;
    void onConnectionAccepted(int id) override { connectionId = id; };
  } serverObserver;
  TcpServer server{context, serverObserver};
  server.listen(protocol, port);
  server.startAcceptingConnections();
  std::thread thread{[&context]() { context.run(); }};
  struct : TcpClient::Observer {
    std::string msg{};
    void onReceived(const char *data, size_t size) override {
      msg.append(data, size);
    };
  } clientObserver;
  TcpClient client{context, clientObserver};
  client.connect({protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto msg{generateRandomString(messageSize)};
  server.send(serverObserver.connectionId, msg.data(), msg.size());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(clientObserver.msg, msg);
  context.stop();
  thread.join();
}

TEST(TcpTest, ClientDisconnects) {
  constexpr uint16_t port{1234};
  const auto protocol{boost::asio::ip::tcp::v4()};
  boost::asio::io_context context;
  struct : TcpServer::Observer {
    bool clientIsConnected{false};
    void onConnectionAccepted(int) override { clientIsConnected = true; };
    void onConnectionClosed(int) override { clientIsConnected = false; };
  } serverObserver;
  TcpServer server{context, serverObserver};
  server.listen(protocol, port);
  server.startAcceptingConnections();
  std::thread thread{[&context]() { context.run(); }};
  TcpClient::Observer clientObserver;
  TcpClient client{context, clientObserver};
  client.connect({protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(serverObserver.clientIsConnected, true);
  client.disconnect();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(serverObserver.clientIsConnected, false);
  context.stop();
  thread.join();
}

TEST(TcpTest, ServerCloses) {
  constexpr uint16_t port{1234};
  const auto protocol{boost::asio::ip::tcp::v4()};
  boost::asio::io_context context;
  TcpServer::Observer serverObserver;
  TcpServer server{context, serverObserver};
  server.listen(protocol, port);
  server.startAcceptingConnections();
  std::thread thread{[&context]() { context.run(); }};
  struct : TcpClient::Observer {
    bool isConnected{false};
    void onConnected() override { isConnected = true; };
    void onDisconnected() override { isConnected = false; };
  } clientObserver;
  TcpClient client{context, clientObserver};
  client.connect({protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(clientObserver.isConnected, true);
  server.close();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(clientObserver.isConnected, false);
  context.stop();
  thread.join();
}
