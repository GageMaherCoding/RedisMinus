# RedisMinus

A minimal Redis-like in-memory key-value TCP server written in C++.

## Features

- Basic commands: `SET`, `GET`, `DEL`, `PING`, `QUIT`
- Simple TCP socket server for learning network programming
- Command parsing and response formatting similar to Redis protocol
- Single-threaded design for simplicity

## Getting Started

### Prerequisites

- Windows with Visual Studio 2022 or newer
- Winsock2 (Windows sockets) support (included in Windows SDK)

### Build and Run

1. Open the `MiniRedisCPP.sln` solution in Visual Studio.
2. Build the solution.
3. Run the executable.
4. Connect to the server using `telnet localhost 12345` (or your chosen port).

### Examples

```bash
> SET key value
OK
> GET key
value
> DEL key
1
