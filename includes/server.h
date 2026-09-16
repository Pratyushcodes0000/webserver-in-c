#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "../includes/Route.h"

using namespace std;

constexpr int PORT = 8080;
constexpr int MAX_EVENT = 100;
const int BUFFER_SIZE = 5000;
const int PATH_SIZE = 20;
const int METHOD_SIZE = 10;
const int VERSION_SIZE = 20;



void set_nonblocking(int fd);
int create_socket();
void bind(int server_fd);
int create_epoll_instance(int server_fd);
void handle_new_client(int server_fd,int epoll_fd);
void parse_req_line(char (&buffer)[BUFFER_SIZE], char (&path)[PATH_SIZE] , char (&method)[METHOD_SIZE] ,char (&version)[VERSION_SIZE]);
void route_matching(char (&path)[20]);
void send_response(int fd);

void string_to_carr(std::string path);