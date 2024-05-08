# Webserv: Understanding HTTP with Your Own HTTP Server

The Webserv project involves writing your own HTTP server in C++ 98. It provides an opportunity to delve into the intricacies of the HTTP protocol and gain practical experience in system programming. The project emphasizes non-blocking I/O operations, HTTP request handling, configuration file parsing, and server configuration.

## How to Run

### Prerequisites

Before running the Webserv program, ensure you have the following prerequisites installed:

- C++ compiler (supporting C++ 98 standard)
- Make build system
- Unix-like operating system (Linux, macOS)

### Compilation

1. Clone the project repository from Git:

    ```bash
    git clone <repository-url>
    ```

2. Navigate to the project directory:

    ```bash
    cd webserv
    ```

3. Compile the source code using the provided Makefile:

    ```bash
    make
    ```

### Execution

Once the compilation is successful, follow these steps to run the Webserv program:

1. Run the executable with the configuration file as an argument:

    ```bash
    ./webserv [configuration-file]
    ```

Replace `[configuration-file]` with the path to your configuration file. If no configuration file is provided, the program may use a default configuration file.

2. Access the HTTP server from a web browser:

    Open a web browser and enter the URL corresponding to the server's host and port specified in the configuration file. For example:

    ```
    http://localhost:8080
    ```

3. Test various HTTP methods and functionalities:

    - Use HTTP methods such as GET, POST, and DELETE to interact with the server.
    - Test the server's response to different requests and verify that it behaves as expected according to the project requirements.

### Cleanup

After testing the program, you can clean up the generated files by running:

```bash
make clean

