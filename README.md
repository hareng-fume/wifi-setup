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
- **Client Request:**  
  - No body (HTTP `GET` request).
- **Server Response (application/json content-type):**  
  - **If rrror:**  
    ```json
    {"error": "<err-message>"}
    ```
    - **HTTP Status:** `400 Bad Request` when no credentials are available.
  - **If success:**  
    ```json
    {"wifi_ids": {"id": "wifi_1", "id": "wifi_2"}}
    ```
    - **HTTP Status:** `200 OK`

### 2) `/validate_wifi_password`
- **Client Request:**  
  - HTTP `POST` request with json body.
- **Request Example (application/json):**  
  ```json
  {"id": "wifi_1", "auth": "!QAZxsw2#EDCvfr4"}
- **Server Response (application/json content-type):**  
  - **If error:**  
    ```json
    {"error": "<err-message>"}
    ```
    - **HTTP Status:** `400 Bad Request` when:
      - The request payload is missing or malformed.
      - The request JSON does not contain id or auth keys.
      -	The requested id key is not found.
    - **HTTP Status:** `401 Unauthorized` when password is incorrect.
  - **If success:**  
    ```json
    {"message": "<test-message>"}
    ```
    - **HTTP Status:** `200 OK` when the network with key 'id' exists and the password with key 'auth' is correct.

## Launch

&nbsp;&nbsp;&nbsp;&nbsp;To start the application find winsetup.exe (client app), winverification.exe (server app) in relative bin directory.

	- winsetup.exe (127.0.0.1, 8080 - are default)
	- winsetup.exe --host=127.0.0.1 --port=8080


**seggins.json** could be fing in sources/client/settings/wifi.json
	- winverification.exe --config=<json-path> (127.0.0.1, 8080 - are default)
	- winverification.exe --config=<json-path> --host=127.0.0.1 --port=8080



## Design Decision

- **Static Library: `communication`** contains base classes for HTTP Server and Client.
- **GUI app: `wifisetup` (client)** contains:  
  1. A specific implementation of HttpClient, class WifiHttpClient, which sends specific requests to the server and processes responses.
  2. A custom UI model class WifiNetworkModel, which is used as a model for the network selector.  

- **Console app: `wifiverification` (server)**  
  1. A specific implementation of HttpServer, class WifiHttpServer, which handles specific client requests and responds in JSON format.

## Dependencies
- **Qt5.15.2:** The core functionality is implemented in Qt5 and C++17 
- **QML: ** The user interface is designed using QML.
- **CMake: ** Used for building the project.


