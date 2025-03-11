#pragma once

#include <QString>

namespace Communication {

inline static constexpr const char *WIFI_NETWORKS = "/get_wifi_networks";
inline static constexpr const char *VALIDATE_WIFI_PWD =
    "/validate_wifi_password";

enum class HttpMethod {
  GET,
  POST // add verbs if required
};

namespace Status {
inline static constexpr const char *OK = "200 OK";
inline static constexpr const char *UNAUTHORIZED = "401 Unauthorized";
inline static constexpr const char *BAD_REQUEST = "400 Bad Request";
inline static constexpr const char *NOT_FOUND = "404 Not Found";
inline static constexpr const char *INTERNAL_SERVER_ERROR =
    "500 Internal Server Error";

QString getMessageFrom(int i_statusCode);
} // namespace Status
} // namespace Communication
