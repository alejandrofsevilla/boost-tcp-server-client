#include "TcpClient.hpp"

#include <iostream>

void TcpClient::Observer::onConnected() {}

void TcpClient::Observer::onReceived([[maybe_unused]] const char *data,
                                     [[maybe_unused]] size_t size) {}

void TcpClient::Observer::onDisconnected() {}

TcpClient::TcpClient(boost::asio::io_context &ioContext, Observer &observer)
    : m_ioContext{ioContext}, m_connection{}, m_observer{observer} {}

void TcpClient::connect(const boost::asio::ip::tcp::endpoint &endpoint) {
  if (m_connection) {
    return;
  }
  auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_ioContext);
  socket->async_connect(endpoint, [this, socket](const auto &error) {
    if (error) {
      std::cerr << "TcpClient::connect() error: " + error.message() + ".\n";
      m_observer.onDisconnected();
      return;
    }
    m_connection = TcpConnection::create(std::move(*socket), *this);
    m_connection->startReading();
    std::cout << "TCPClient connected.\n";
    m_observer.onConnected();
  });
}

void TcpClient::send(const char *data, size_t size) {
  if (!m_connection) {
    std::cerr << "TcpClient::send() error: no connection.\n";
    return;
  }
  m_connection->send(data, size);
}

void TcpClient::disconnect() {
  if (m_connection) {
    m_connection->close();
  }
}

void TcpClient::onReceived([[maybe_unused]] int connectionId, const char *data,
                           size_t size) {
  m_observer.onReceived(data, size);
}

void TcpClient::onConnectionClosed([[maybe_unused]] int connectionId) {
  if (m_connection) {
    m_connection.reset();
    std::cout << "TCPClient disconnected.\n";
    m_observer.onDisconnected();
  }
}
