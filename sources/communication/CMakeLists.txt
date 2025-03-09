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
    Qt5::Core
    Qt5::Network
    Qt5::Quick
)
