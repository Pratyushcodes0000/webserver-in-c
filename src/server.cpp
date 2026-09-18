#include "../includes/server.h"

std::string body;
std::string type = "type/plain";
Server server;

void Server::set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int Server::create_socket() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  set_nonblocking(server_fd);
  return server_fd;
}

void Server::Bind(int server_fd) {
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

int Server::create_epoll_instance(int server_fd) {
  int epoll_fd = epoll_create1(0);

  epoll_event ev{};
  ev.events = EPOLLIN;
  ev.data.fd = server_fd;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

  return epoll_fd;
}

void Server::handle_new_client(int server_fd, int epoll_fd) {

  int client_fd = accept(server_fd, nullptr, nullptr);
  set_nonblocking(client_fd);

  epoll_event client_ev{};
  client_ev.data.fd = client_fd;
  client_ev.events = EPOLLIN;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev);

  server.connections[client_fd];
}

void Server::parse_req_line(const std::string &buffer, std::string &path,
                            std::string &method, std::string &version) {
  std::istringstream iss(buffer);
  iss >> method >> path >> version;
}

void Server::route_matching(std::string &path) {
  // route
  for (const auto &vec_path : route_vec) {
    if (path == vec_path) {
      body = "{ \"status\": \"ok\" }";
      type = "application/json";
    } else {
      body = "Not Found";
    }
  }
}

// void Server::find_conenction(int fd){
//   for (auto& [connection_fd,connection] : server.connections){
//         if(fd == connection_fd){

//         }
//   }
// }

void Server::send_response(int fd) {
  std::string response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: " +
                         type +
                         "\r\n"
                         "Content-Length: " +
                         std::to_string(body.size()) +
                         "\r\n"
                         "\r\n" +
                         body;

  send(fd, response.data(), response.size(), 0);

  close(fd);
}

int main() {
  Connection connection;
  Route route;

  route.create("/api/health");

  // create socket
  int server_fd = server.create_socket();

  // bind
  server.Bind(server_fd);
  listen(server_fd, SOMAXCONN);

  cout << "Server running on http://localhost:8080\n";

  // creating epoll instance
  int epoll_fd = server.create_epoll_instance(server_fd);

  // epoll buffer
  epoll_event events[MAX_EVENT];

  while (true) {
    int n = epoll_wait(epoll_fd, events, MAX_EVENT, -1);

    for (int i = 0; i < n; i++) {
      int fd = events[i].data.fd;

      if (fd == server_fd) {
        server.handle_new_client(server_fd, epoll_fd);
      } else {
        int byte = read(fd, server.connections[fd].InputBuffer.data(), sizeof(server.connections[fd].InputBuffer) - 1);

        std::string request(server.connections[fd].InputBuffer.data(), byte);

        if (byte <= 0) {
          close(fd);
          continue;
        }

        // insert a newline(bute > 0)
        // std::cout << buffer << std::endl;

        std::string path;
        std::string method;
        std::string version;
        server.parse_req_line(request, path, method, version);

        // route here
        server.route_matching(path);

        // send response here
        // response goes after matching routes
        server.send_response(fd);
      }
    }
  }
}
