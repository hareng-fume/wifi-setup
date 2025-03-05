add_library(communication STATIC
  client.cpp
  client.h
  server.h
  server.cpp
  settings.h
  settings.cpp
  wifi_api.h
)

target_link_libraries(communication PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Quick
)
