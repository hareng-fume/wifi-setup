qt_standard_project_setup()

link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(PROJECT_SOURCES
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(wifiverification
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        wifi_server.h
        wifi_server.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET wifiverification APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(wifiverification SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(wifiverification
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(wifiverification PRIVATE
    communication Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(wifiverification PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)
