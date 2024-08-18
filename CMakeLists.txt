cmake_minimum_required(VERSION 3.14)

# Project name
project(CurrencyConverter)

# Use Qt6
find_package(Qt6 COMPONENTS Widgets Network QUIET)
find_package(Qt5 COMPONENTS Widgets Network QUIET)

# Enable automatic MOC, RCC, and UIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(SOURCES main.cpp CurrencyConverter.cpp CurrencyConverter.h)

# Add source files
if(WIN32)
    list(APPEND SOURCES CurrencyConverterWindows.cpp)
elseif(LINUX)
    list(APPEND SOURCES CurrencyConverterLinux.cpp)
elseif(Qt5_FOUND OR Qt6_FOUND)
    list(APPEND SOURCES CurrencyConverterQT.cpp)
endif()



# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

if(Qt5_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
elseif(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winhttp)
elseif(LINUX)
    find_package(CURL REQUIRED)
    target_link_libraries(${PROJECT_NAME} CURL::libcurl)
elseif(Qt5_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt5::Network)
elseif(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt6::Network)
endif()


# Include directories for jsoncpp
target_include_directories(${PROJECT_NAME} PRIVATE rapidjson/include)

# Include current directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

