# C++ CRUD API and WebSocket Server

This project implements a simple CRUD API and a WebSocket server in C++ using Boost.Beast. The API supports basic key-value operations (insert, retrieve, delete) and uses a globally accessible in-memory database (an `std::unordered_map`). Additionally, all operations are broadcasted to WebSocket clients, providing a real-time log stream of changes to the database.

## Features

### CRUD API
- **POST**: Insert or update a key-value pair in the in-memory database.
- **GET**: Retrieve the value associated with a key.
- **DELETE**: Remove a key-value pair from the database.

### WebSocket Logging
Clients can connect via WebSocket to receive real-time notifications of CRUD operations in the database. Each notification includes:
- Operation type (Insert, Retrieve, Delete)
- Key and value
- Timestamp of the operation

## Project Structure

```plaintext
src
├── app
│   └── main.cpp                    # Entry point of the application
├── controllers
│   └── CrudController.hpp, .cpp    # Handles HTTP CRUD operations
├── models
│   └── KeyValueModel.hpp           # Model representing a key-value entry
├── services
│   ├── KeyValueService.hpp         # Interface for CRUD operations
│   └── InMemoryKeyValueService.cpp # In-memory implementation of KeyValueService
├── utils
│   ├── JsonUtil.hpp                # JSON serialization/deserialization
│   └── WebSocketBroadcaster.hpp, .cpp # Broadcasts messages to WebSocket clients
└── server
    ├── HttpServer.hpp, .cpp        # Manages HTTP server operations
    └── WebSocketServer.hpp, .cpp   # Manages WebSocket connections
```


## Requirements

- **C++17** or newer
- **Boost.Beast** and **Boost.Asio** for HTTP and WebSocket functionality
- **nlohmann/json** or **boost::json** for JSON serialization

## Running the Server

1. Compile the code using CMake.
2. Run the application:
   ```bash
   ./app/main
   ```
3. The HTTP API will be available on port 8080, and the WebSocket server will be on port 8081.

## API Endpoints

- **POST** `/api`: Insert or update a key-value pair.
- **GET** `/api?key=<key>`: Retrieve the value for a specified key.
- **DELETE** `/api?key=<key>`: Delete the specified key-value pair.

## Design Patterns Used

1. **Dependency Injection**  
   The `CrudController` class receives dependencies (`KeyValueService` and `WebSocketBroadcaster`) through its constructor, promoting loose coupling and allowing easy substitution of different `KeyValueService` implementations.

2. **Interface Segregation Principle**  
   `KeyValueService` defines an interface for database operations (insert, retrieve, remove). This design allows the use of different storage backends by implementing the `KeyValueService` interface. In this project, we use an in-memory service implementation (`InMemoryKeyValueService`).

3. **Single Responsibility Principle**  
   Each class in this project has a specific responsibility:
   - `CrudController`: Handles HTTP request processing for CRUD operations.
   - `InMemoryKeyValueService`: Manages the key-value data in memory.
   - `WebSocketBroadcaster`: Handles broadcasting of messages to WebSocket clients.

4. **Observer Pattern**  
   `WebSocketBroadcaster` serves as an observer by notifying connected WebSocket clients of CRUD operations in real time. Whenever an operation occurs, a message is sent to all connected clients, detailing the operation, key, value, and timestamp.

## Example Workflow

1. **Insert Key-Value**: Send a POST request with a JSON payload containing `"key"` and `"value"`.
2. **Retrieve Value**: Send a GET request with the key as a query parameter.
3. **Delete Key-Value**: Send a DELETE request with the key as a query parameter.
4. **WebSocket Notification**: WebSocket clients receive a JSON message each time an operation (insert, retrieve, delete) occurs, including details like operation type, key, value, and timestamp.

  POST Request (Insert/Update)
  ```{
    "key": "sampleKey",
    "value": {"field1": "data", "field2": 123}
  }
```
  
  GET Request (Retrieve)
    ```GET /api?key=sampleKey```
    
  DELETE Request (Delete)
    ```DELETE /api?key=sampleKey```
