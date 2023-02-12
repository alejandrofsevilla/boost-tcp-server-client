#ifndef TCP_SERVER_CLIENT_TCP_SERVER_HPP
#define TCP_SERVER_CLIENT_TCP_SERVER_HPP

#include <boost/asio.hpp>
#include <unordered_map>

#include "TcpConnection.hpp"

namespace tcp_server_client {
/**
 * TcpServer class allows to listen and accept TCP connections at any network
 * interface, and to send and receive data streams to connected endpoints.
 */
class TcpServer : private TcpConnection::Observer {
 public:
  /**
   * Observer class allows monitoring of TcpServer events.
   */
  struct Observer {
    /**
     * virtual function called by TcpServer after a connection has been
     * accepted.
     *
     * @param connectionId unique identifier of the accepted connection.
     */
    virtual void onConnectionAccepted(int connectionId);
    /**
     * virtual function called by TcpServer after data is received.
     *
     * @param connectionId unique identifier of the receiving connection.
     * @param data received.
     * @param size of the received data.
     */
    virtual void onReceived(int connectionId, const char *data, size_t size);
    /**
     * virtual function called by TcpServer after a connection has been closed.
     *
     * @param connectionId unique identifier of the closed connection.
     */
    virtual void onConnectionClosed(int connectionId);
  };
  /**
   * Constructs a TcpServer object.
   *
   * @param ioContext required for asynchronous input and output operations.
   * @param observer to monitor TcpServer events.
   */
  TcpServer(boost::asio::io_context &ioContext, Observer &observer);
  /**
   * listen for connections at any interface with specified
   * protocol to specified port.
   *
   * @param protocol of the interfaces to listen at.
   * @param port to listen to.
   * @return true if success, false otherwise.
   */
  bool listen(const boost::asio::ip::tcp &protocol, uint16_t port);
  /**
   * starts accepting connections and associated asynchronous read and
   * write operations.
   */
  void startAcceptingConnections();
  /**
   * Sends data to peer associated to specified connection if exists.
   *
   * @param connectionId unique identifier associated to receiving peer.
   * @param data to send.
   * @param size of the data to send.
   */
  void send(int connectionId, const char *data, size_t size);
  /**
   * Close active connections and stops accepting new connections. In order to
   * restart operation, a call to functions listen() and
   * startAcceptingConnections() is required.
   */
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
}  // namespace tcp_server_client

#endif
