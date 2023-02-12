#ifndef TCP_SERVER_CLIENT_TCP_CLIENT_HPP
#define TCP_SERVER_CLIENT_TCP_CLIENT_HPP

#include <boost/asio.hpp>
#include <memory>

#include "TcpConnection.hpp"

namespace tcp_server_client {
/**
 * TcpClient class allows to connect to remote TCP endpoint, and send and
 * receive data streams.
 */
class TcpClient : private TcpConnection::Observer {
 public:
  /**
   * Observer class allows monitoring of TcpClient events.
   */
  struct Observer {
    /**
     * virtual function called by TcpClient after connection success.
     */
    virtual void onConnected();
    /**
     * virtual function called by TcpClient after data is received.
     *
     * @param data received.
     * @param size of the received data.
     */
    virtual void onReceived(const char *data, size_t size);
    /**
     * virtual function called by TcpClient after connection is closed or
     * after an attempted connection fails.
     */
    virtual void onDisconnected();
  };
  /**
   * Constructs a TcpClient object.
   *
   * @param ioContext required for asynchronous input and ouput operations.
   * @param observer to monitor TcpClient events.
   */
  TcpClient(boost::asio::io_context &ioContext, Observer &observer);
  /**
   * attempts to connect asynchronously to specified endpoint.
   * Observer::onConnected() will be invoked on success,
   * Observer::onDisconnected() will be invoked if an error occurs.
   *
   * @param endpoint to connect to.
   */
  void connect(const boost::asio::ip::tcp::endpoint &endpoint);
  /**
   * sends string data to peer connected to TcpConnection if
   * exists.
   *
   * @param data to send.
   * @param size of the data to send.
   */
  void send(const char *data, size_t size);
  /**
   * Closes connection.
   */
  void disconnect();

 private:
  void onReceived(int connectionId, const char *data, size_t size) override;
  void onConnectionClosed(int connectionId) override;

  boost::asio::io_context &m_ioContext;
  std::shared_ptr<TcpConnection> m_connection;
  Observer &m_observer;
};
}  // namespace tcp_server_client

#endif
