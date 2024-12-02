cmake_minimum_required(VERSION 3.14)

# Set the project name and version
project(RadioApp VERSION 1.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the required Qt packages
find_package(Qt6 COMPONENTS Core Network Quick REQUIRED)

# Add the sources
set(SOURCES
    sources/dab_table_entry.cpp
    sources/http.cpp
    sources/json.cpp
    sources/main.cpp
    sources/radio_hal.cpp
    sources/radio_methods.cpp
    sources/radio_properties.cpp
    sources/radio_station_model.cpp

    sources/dab_table_entry.h
    sources/http.h
    sources/json.h
    sources/parameters.h
    sources/radio_hal.h
    sources/radio_methods.h
    sources/radio_properties.h
    sources/radio_station_model.h
    sources/radio_stations.h
    sources/radio_main_widget.h

)

# Add the resources
set(RESOURCES
    bilder.qrc
    seiten.qrc
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add the Qt resources
qt_add_resources(RESOURCES_ADDED ${RESOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES_ADDED})

# Link the Qt libraries
target_link_libraries(${PROJECT_NAME}
    gemini-service-base-qml
    )

# Add the QML import paths
set(QML_IMPORT_PATH "")
set(QML_DESIGNER_IMPORT_PATH "")

set_target_properties(${PROJECT_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAgriCultureMain
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install the target
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
