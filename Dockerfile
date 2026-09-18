FROM ubuntu:24.04 AS builder

# Install compiler and build tools
RUN apt-get update && \
    apt-get install -y \
    g++ \
    cmake \
    make \
    && rm -rf /var/lib/apt/lists/*

# Container working directory
WORKDIR /app

# Copy project into container
COPY . .

# Configure CMake
RUN cmake -S . -B build

# Compile
RUN cmake --build build

EXPOSE 8080

CMD ["./build/async_http_server"]