#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <boost/asio.hpp>
#include <memory>

#include "TcpConnection.hpp"

class TcpClient : private TcpConnection::Observer {
 public:
  struct Observer {
    virtual void onConnected();
    virtual void onReceived(const char *data, size_t size);
    virtual void onDisconnected();
  };

  TcpClient(boost::asio::io_context &ioContext, Observer &observer);

  void connect(const boost::asio::ip::tcp::endpoint &endpoint);
  void send(const char *data, size_t size);
  void disconnect();

 private:
  void onReceived(int connectionId, const char *data, size_t size) override;
  void onConnectionClosed(int connectionId) override;

  boost::asio::io_context &m_ioContext;
  std::shared_ptr<TcpConnection> m_connection;
  Observer &m_observer;
};

#endif
