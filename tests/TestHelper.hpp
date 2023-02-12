#ifndef TCP_SERVER_CLIENT_TESTS_TEST_HELPER_HPP
#define TCP_SERVER_CLIENT_TESTS_TEST_HELPER_HPP

#include <random>

namespace tcp_server_client::tests {
inline std::string generateRandomString(size_t size) {
  std::random_device rd;
  std::mt19937 eng{rd()};
  std::uniform_int_distribution<char> dist;
  std::string str(size, {});
  std::generate(str.begin(), str.end(), std::bind(dist, eng));
  return str;
}
}  // namespace tcp_server_client::tests

#endif
