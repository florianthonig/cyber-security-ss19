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
    message("Botan found!")
ELSE(BOTAN_FOUND)
    message("Botan not found!")
ENDIF(BOTAN_FOUND)

find_package(GMP REQUIRED)
find_package(OpenMP)


# Suchen nach Botan
#find_package(Botan REQUIRED)

include_directories("${BOTAN_INCLUDE_DIRS}")
include_directories("${GMP_INCLUDE_DIRS}")

add_executable(rsa-encrypt
        rsa-encrypt.cpp rsa-impl-replacement.h)
add_executable(rsa-decrypt
        rsa-decrypt.cpp rsa-impl-replacement.h)
add_executable(rsa-keygen
        rsa-keygen.cpp rsa-impl-replacement.h)

add_executable(pollard
        pollard.cpp rsa-impl-replacement.h)

add_executable(knj
        knj.cpp rsa-impl-replacement.h)

add_executable(sieve
        sieve.cpp bigint.h rsa-impl-replacement.h)

add_executable(shor
        shor.cpp rsa-impl-replacement.h)

target_link_libraries(rsa-encrypt "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(rsa-decrypt "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(rsa-keygen "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(pollard "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(knj "${BOTAN_LIBRARIES}" "${GMP_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(sieve "${BOTAN_LIBRARIES}" OpenMP::OpenMP_CXX -static-libgcc -static-libstdc++ Threads::Threads)
target_link_libraries(shor "${BOTAN_LIBRARIES}" -static-libgcc -static-libstdc++ Threads::Threads)