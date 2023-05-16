#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <boost/asio.hpp>
#include <unordered_map>

#include "TcpConnection.hpp"

class TcpServer : private TcpConnection::Observer {
 public:
  struct Observer {
    virtual void onConnectionAccepted(int connectionId);
    virtual void onReceived(int connectionId, const char *data, size_t size);
    virtual void onConnectionClosed(int connectionId);
  };

  TcpServer(boost::asio::io_context &ioContext, Observer &observer);

  bool listen(const boost::asio::ip::tcp &protocol, uint16_t port);
  void startAcceptingConnections();
  void send(int connectionId, const char *data, size_t size);
  void close();

 private:
  void doAccept();
  void onReceived(int connectionId, const char *data, size_t size) override;
  void onConnectionClosed(int connectionId) override;

  boost::asio::ip::tcp::acceptor m_acceptor;
  std::unordered_map<int, std::shared_ptr<TcpConnection>> m_connections;
  Observer &m_observer;
  int m_connectionCount;
  bool m_isAccepting;
  bool m_isClosing;
};

#endif
