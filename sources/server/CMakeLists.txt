link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

 add_executable(wifiverification
    main.cpp
    wifi_server.h
    wifi_server.cpp
 )

target_link_libraries(wifiverification PRIVATE
    communication Qt5::Core Qt5::Network)

target_include_directories(wifiverification PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)
