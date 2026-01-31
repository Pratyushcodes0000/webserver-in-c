⚡ Async HTTP Server in C++ (epoll-based Event Loop)

A lightweight asynchronous HTTP server implemented in C++ using Linux’s epoll for efficient event-driven I/O.

This project demonstrates how to build a simple event loop from scratch using:

Non-blocking sockets

epoll system calls

Manual HTTP request parsing

Low-level network programming in C++

✨ Features

✅ Non-blocking TCP server

✅ Event-driven I/O using epoll

✅ Handles multiple clients concurrently

✅ Simple HTTP request parsing

✅ Route handling (/, /api/health)

✅ JSON response support

✅ Minimal and fast

🧠 Concepts Covered:
This project helps understand:
Event loop design
Asynchronous networking
Linux epoll
File descriptor monitoring
Socket programming (TCP/IP)
HTTP protocol basics
System calls (socket, bind, listen, accept, read, send)
Non-blocking I/O (fcntl)

⚙️ Build Instructions:
Requirements
Linux OS (for epoll)
GCC or Clang
C++17 or later
Compile
g++ -std=c++17 server.cpp -o server

▶️ Run the Server
./server

You should see:
Server running on http://localhost:8080

⚠️ Limitations:
Not production ready
No HTTPS
No keep-alive connections
Minimal HTTP parsing
No error handling for malformed requests
Single-threaded
This project is intended for learning and experimentation.


📚 Learning Resources:
Linux epoll documentation
Beej’s Guide to Network Programming
Tsoding’s networking videos
Node.js event loop internals


👨‍💻 Author:
Pratyush Chowdhury
C++ | Networking | Systems Programming
