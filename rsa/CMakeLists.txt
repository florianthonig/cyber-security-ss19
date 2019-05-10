cmake_minimum_required(VERSION 3.11)

project(rsa)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
# Wir brauchen module zum suchen und finden von Botan
find_package(Threads REQUIRED)
# Threads zum Linken für Windows
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

pkg_check_modules(BOTAN botan-2)
IF(BOTAN_FOUND)
    message(INFO "Botan found!")
ELSE(BOTAN_FOUND)
    message(FATAL_ERROR "Botan not found!")
ENDIF(BOTAN_FOUND)


# Suchen nach Botan
#find_package(Botan REQUIRED)

include_directories("${BOTAN_INCLUDE_DIRS}")

add_executable(rsa-encrypt
        rsa-encrypt.cpp)
add_executable(rsa-decrypt
        rsa-decrypt.cpp)
add_executable(rsa-keygen
        rsa-keygen.cpp)

target_link_libraries(rsa-encrypt "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(rsa-decrypt "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(rsa-keygen "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)