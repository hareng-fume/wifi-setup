#pragma once

namespace Communication {

inline static constexpr const char *WIFI_NETWORKS = "/get_wifi_networks";
inline static constexpr const char *VALIDATE_WIFI_PWD = "/validate_wifi_password";

enum class HttpMethod {
    GET,
    POST // add verbs if required
};

} // namespace Communication
