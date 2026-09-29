# Dockerfile for testing NetIF with custom network configurations
# This provides an environment where we can create virtual network interfaces

FROM ubuntu:22.04

# Install required dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    iproute2 \
    iputils-ping \
    net-tools \
    && rm -rf /var/lib/apt/lists/*

# Create a working directory
WORKDIR /netif

# Copy the project files
COPY . /netif/

# Build the project
RUN mkdir -p build && cd build && \
    cmake -DENABLE_NETIF_TESTS=ON .. && \
    cmake --build .

# Entry point script
COPY tests/docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["test"]
