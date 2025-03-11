# WiFi Setup

WiFi Setup is a client-server application that allows users to select and authenticate WiFi networks through a graphical user interface. It consists of a GUI client (`wifisetup`) and a console-based server (`wifiverification`), using HTTP for communication.

## Features
- Displays available WiFi networks.
- Allows users to input a password for authentication.
- Communicates with the server via HTTP requests.
- Provides real-time status updates on the connection process.

## Requirement
The requirement that app end-users can possibly use devices led to the decision to use HTTP for client-server communication.

## API
### 1) `/get_wifi_networks`
- **Request:**  
  - HTTP `GET` request with no body.
- **Response (application/json):**  
  - **Error:**  
    ```json
    {"error": "<err-message>"}
    ```
    - **Status:** `400 Bad Request`  
      - When no credentials are available.
  - **Success:**  
    ```json
    {"wifi_ids": {"id": "wifi_1", "id": "wifi_2"}}
    ```
    - **Status:** `200 OK`

### 2) `/validate_wifi_password`
- **Request:**  
  - HTTP `POST` request with json body.
- **Request Example (application/json):**  
  ```json
  {"id": "wifi_1", "auth": "!QAZxsw2#EDCvfr4"}
- **Response (application/json):**  
  - **Error:**  
    ```json
    {"error": "<err-message>"}
    ```
    - **Status:** `400 Bad Request`
      - When can't extract request payload.
      - When there is no 'id' or 'auth' key in request-json
      -	When requested 'id' key is not found
    - **Status:** `401 Unauthorized`
      - When password is not correct
  - **Success:**  
    ```json
    {"message": "<test-message>"}
    ```
    - **Status:** `200 OK`

## Design Decision

- **Static Library: `communication`**  
  - Contains base classes for HTTP Server and Client.

- **GUI Application: `wifisetup` (Client)**  
  1. Implements a specific HTTP client:  
     - `WifiHttpClient` class, which sends specific requests to the server and processes responses.  
  2. Uses a custom UI model:  
     - `WifiNetworkModel`, which is set as a model for the network selector.

- **Console Application: `wifiverification` (Server)**  
  1. Implements a specific HTTP server:  
     - `WifiHttpServer` class, which handles client requests and responds in JSON format.

