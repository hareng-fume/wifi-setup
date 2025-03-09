link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

add_executable(wifisetup
    main.cpp
    qml.qrc

    wifi_client.cpp
    wifi_client.h
)

target_link_libraries(wifisetup PRIVATE
    communication Qt5::Core Qt5::Quick)

target_include_directories(wifisetup PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../communication
)
