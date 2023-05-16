#include "TcpConnection.hpp"

#include <iostream>

namespace {
constexpr auto f_maxPackageSize{1024};
}  // namespace

void TcpConnection::Observer::onReceived([[maybe_unused]] int connectionId,
                                         [[maybe_unused]] const char *data,
                                         [[maybe_unused]] const size_t size) {}

void TcpConnection::Observer::onConnectionClosed(
    [[maybe_unused]] int connectionId) {}

TcpConnection::TcpConnection(boost::asio::ip::tcp::socket &&socket,
                             Observer &observer, int id)
    : m_socket{std::move(socket)},
      m_readBuffer{},
      m_writeBuffer{},
      m_writeBufferMutex{},
      m_observer{observer},
      m_isReading{false},
      m_isWritting{false},
      m_id{id} {}

std::shared_ptr<TcpConnection> TcpConnection::create(
    boost::asio::ip::tcp::socket &&socket, Observer &observer, int id) {
  return std::shared_ptr<TcpConnection>(
      new TcpConnection{std::move(socket), observer, id});
}

void TcpConnection::startReading() {
  if (!m_isReading) {
    doRead();
  }
}

void TcpConnection::send(const char *data, size_t size) {
  std::lock_guard<std::mutex> guard{m_writeBufferMutex};
  std::ostream bufferStream{&m_writeBuffer};
  bufferStream.write(data, size);
  if (!m_isWritting) {
    doWrite();
  }
}

void TcpConnection::close() {
  try {
    m_socket.cancel();
    m_socket.close();
  } catch (const std::exception &e) {
    std::cerr << "TcpConnection::Close() caught exception: " +
                     static_cast<std::string>(e.what())
              << std::endl;
    return;
  }
  m_observer.onConnectionClosed(m_id);
}

void TcpConnection::doRead() {
  m_isReading = true;
  auto buffers = m_readBuffer.prepare(f_maxPackageSize);
  auto self = shared_from_this();
  m_socket.async_read_some(
      buffers, [this, self](const auto &error, auto bytesTransferred) {
        if (error) {
          std::cerr << "TcpConnection::doRead() found error: " + error.message()
                    << std::endl;
          return close();
        }
        m_readBuffer.commit(bytesTransferred);
        m_observer.onReceived(
            m_id, static_cast<const char *>(m_readBuffer.data().data()),
            bytesTransferred);
        m_readBuffer.consume(bytesTransferred);
        doRead();
      });
}

void TcpConnection::doWrite() {
  m_isWritting = true;
  auto self = shared_from_this();
  m_socket.async_write_some(m_writeBuffer.data(), [this, self](
                                                      const auto &error,
                                                      auto bytesTransferred) {
    if (error) {
      std::cerr << "TcpConnection::doWrite() found error: " + error.message()
                << std::endl;
      return close();
    }
    std::lock_guard<std::mutex> guard{m_writeBufferMutex};
    m_writeBuffer.consume(bytesTransferred);
    if (m_writeBuffer.size() == 0) {
      m_isWritting = false;
      return;
    }
    doWrite();
  });
}
