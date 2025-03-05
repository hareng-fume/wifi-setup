#ifndef WIFI_API_H
#define WIFI_API_H

namespace Communication {

namespace GET {
inline static constexpr const char *WIFI_NETWORKS = "/get_wifi_networks";
}

namespace POST {
inline static constexpr const char *VALIDATE_WIFI_PWD = "/validate_wifi_password";
}

} // namespace Communication

#endif // WIFI_API_H
