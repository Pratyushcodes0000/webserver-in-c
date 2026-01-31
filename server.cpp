#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

using namespace std;

constexpr int PORT = 8080;
constexpr int MAX_EVENT = 100;

void set_nonblocking(int fd){
  int flags = fcntl(fd , F_GETFL , 0);
  fcntl(fd , F_SETFL , flags | O_NONBLOCK);
}

int main(){
    //create socket 
    int server_fd = socket(AF_INET , SOCK_STREAM,0);
    set_nonblocking(server_fd);

    //binding Ip and port
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd , (sockaddr*)&address , sizeof(address));
    listen(server_fd ,SOMAXCONN);

     cout << "Server running on http://localhost:8080\n";

     //creating epoll instance
     int epoll_fd = epoll_create1(0);
     
     epoll_event ev{};
     ev.events = EPOLLIN;
     ev.data.fd = server_fd;

     epoll_ctl(epoll_fd , EPOLL_CTL_ADD , server_fd , &ev);

     epoll_event events[MAX_EVENT];

     while (true){
        int n = epoll_wait(epoll_fd , events , MAX_EVENT , -1);

        for (int i = 0; i < n; i++){
            int fd = events[i].data.fd;

            if (fd == server_fd){
              int client_fd = accept(server_fd , nullptr , nullptr);
              set_nonblocking(client_fd);

              epoll_event client_ev{};
              client_ev.data.fd = client_fd;
              client_ev.events = EPOLLIN;

              epoll_ctl(epoll_fd , EPOLL_CTL_ADD , client_fd , &client_ev);
            }else {
              char buffer[5000];
              int byte = read(fd,buffer,sizeof(buffer)-1);

              if (byte <=0){
                close(fd);
                continue;
              }

              buffer[byte] = '\0';

              //parse req line
              char method[10] ,path[20],version[50];
              sscanf(buffer , "%s %s %s" , method, path,version);

              const char* body;
              const char* type = "type/plain";


               if(strcmp(path , "/") == 0){
                     body = "Home (async)";
                }
                else if(strcmp(path , "/api/health") == 0){
                    body = "{ \"status\": \"ok\" }";
                    type = "application/json";
                }
                else {
                    body = "Not Found";
                }
             
                char response[100];

               snprintf(
                    response,
                    sizeof(response),
                     "HTTP/1.1 200 OK\r\n"
                    "Content-Type: %s\r\n"
                    "Content-Length: %zu\r\n"
                    "\r\n"
                    "%s",
                    type,
                    strlen(body),
                    body
                );

                send(fd,response,sizeof(response),0);
                close(fd);

              
            }
            
        }
        
     }
     
}