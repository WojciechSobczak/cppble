cmake_minimum_required(VERSION 3.19)
project("cppble")
set(CMAKE_CXX_STANDARD 23)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Detected x86 architecture which is unsupported one. Exiting...")
endif()


#Mode and system detection
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Detected DEBUG mode.")
    set(DEBUG_BUILD true)
else()
    message("Detected RELEASE mode.")
    set(DEBUG_BUILD false)
endif()

if (MSVC)
    set(WINDOWS TRUE)
    message("Detected WINDOWS OS")
    add_compile_definitions(CPPBLE_WINDOWS_OS)
elseif(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    message("Detected LINUX OS")
    add_compile_definitions(CPPBLE_LINUX_OS)
else()
    message(FATAL_ERROR "Unsupported OS")
endif()


set(SRC_FILES
    "src/scanner.cpp"
)

if (WINDOWS)
    add_compile_options(/W4 /await /EHsc /utf-8)
    link_libraries("windowsapp" "bluetoothapis")
    list(APPEND SRC_FILES
        "src/windows/win_adapter.cpp"
        "src/windows/win_scanner.cpp"
        "src/windows/win_device.cpp"
    )
elseif(LINUX)
    message(FATAL_ERROR "Unsupported OS yet")
else()
    message(FATAL_ERROR "Unsupported OS")
endif()

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

if (${BLUETOOTHCPP_DEV_BUILD})
    message("Dev test executable build")
    add_executable("test" "src/main.cpp")
    target_link_libraries("test" ${PROJECT_NAME})
endif()