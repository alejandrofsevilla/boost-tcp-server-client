#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <mutex>

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  struct Observer {
    virtual void onReceived(int connectionId, const char *data, size_t size);
    virtual void onConnectionClosed(int connectionId);
  };

  static std::shared_ptr<TcpConnection> create(
      boost::asio::ip::tcp::socket &&socket, Observer &observer, int id = 0);

  void startReading();
  void send(const char *data, size_t size);
  void close();

 private:
  TcpConnection(boost::asio::ip::tcp::socket &&socket, Observer &observer,
                int id);
  void doRead();
  void doWrite();

  boost::asio::ip::tcp::socket m_socket;
  boost::asio::streambuf m_readBuffer;
  boost::asio::streambuf m_writeBuffer;
  std::mutex m_writeBufferMutex;
  Observer &m_observer;
  bool m_isReading;
  bool m_isWritting;
  int m_id;
};

#endif
