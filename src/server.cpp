#include "../includes/server.h"
#include <exception>
#include <iostream>

std::string body;
std::string type = "type/plain";

void set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int create_socket() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  set_nonblocking(server_fd);
  return server_fd;
}

void bind(int server_fd) {
  // binding Ip and port
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  try {
    int x = bind(server_fd, (sockaddr *)&address, sizeof(address));
    std::cout << "Value returned by bind" << x << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error in binding" << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown error in binding" << std::endl;
  }
}

int create_epoll_instance(int server_fd) {
  int epoll_fd = epoll_create1(0);

  epoll_event ev{};
  ev.events = EPOLLIN;
  ev.data.fd = server_fd;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

  return epoll_fd;
}

void handle_new_client(int server_fd, int epoll_fd) {

  int client_fd = accept(server_fd, nullptr, nullptr);
  set_nonblocking(client_fd);

  epoll_event client_ev{};
  client_ev.data.fd = client_fd;
  client_ev.events = EPOLLIN;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev);
}

void parse_req_line(
    const std::string& buffer,
    std::string& path,
    std::string& method,
    std::string& version
) {
    std::istringstream iss(buffer);
    iss >> method >> path >> version;

} 

void route_matching(std::string &path) {
  // route
  for (const auto &vec_path : route_vec) {
     if (path == vec_path ) {
      body = "{ \"status\": \"ok\" }";
      type = "application/json";
    } else {
      body = "Not Found";
    }
  }
}

void send_response(int fd)
{
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + type + "\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" +
        body;

    send(fd, response.data(), response.size(), 0);

    close(fd);
}

int main() {
  Route route;
  route.create("/api/health");
  route.create("/api/get");
  // create socket
  int server_fd = create_socket();

  // bind
  bind(server_fd);
  listen(server_fd, SOMAXCONN);

  cout << "Server running on http://localhost:8080\n";

  // creating epoll instance
  int epoll_fd = create_epoll_instance(server_fd);

  // epoll buffer
  epoll_event events[MAX_EVENT];

  while (true) {
    int n = epoll_wait(epoll_fd, events, MAX_EVENT, -1);

    for (int i = 0; i < n; i++) {
      int fd = events[i].data.fd;

      if (fd == server_fd) {
        handle_new_client(server_fd, epoll_fd);
      } else {

        std::vector<char> buffer(BUFFER_SIZE);

        int byte = read(fd, buffer.data(), sizeof(buffer) - 1);

        std::string request(buffer.data(), byte);

        if (byte <= 0) {
          close(fd);
          continue;
        }

        // insert a newline(bute > 0)
        buffer[byte] = '\0';
        // std::cout << buffer << std::endl;

        std::string path;
        std::string method;
        std::string version;
        parse_req_line(request, path, method, version);

        // route here
        route_matching(path);

        // send response here
        // response goes after matching routes
        send_response(fd);
      }
    }
  }
}