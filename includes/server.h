#include "../includes/Route.h"
#include <arpa/inet.h>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/epoll.h>
#include <unistd.h>
#include <unordered_map>

using namespace std;

constexpr int PORT = 8080;
constexpr int MAX_EVENT = 100;
constexpr int BUFFER_SIZE = 5000;

struct Connection {
  std::vector<char> InputBuffer;
  std::vector<char> OutputBuffer;
};

class Server {
public:
  std::unordered_map<int, Connection> connections;
  void send_response(int fd);
  int create_socket();
  void Bind(int server_fd);
  int create_epoll_instance(int server_fd);
  void handle_new_client(int server_fd, int epoll_fd);
  void parse_req_line(const std::string &buffer, std::string &path,
                      std::string &method, std::string &version);
  void route_matching(std::string &path);
  void find_conenction(int fd);

  private:
    void set_nonblocking(int fd);

};
