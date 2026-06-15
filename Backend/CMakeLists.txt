cmake_minimum_required(VERSION 3.14)
project(SpotkajMnieBackend VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Twój główny program
add_executable(SpotkajMnieBackend src/main.cpp)

# --- KONFIGURACJA TESTÓW (Google Test) ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Zabezpieczenie dla Windowsa
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Tworzymy osobną "aplikację" tylko do testów
add_executable(SpotkajMnieTests tests/EventSchedulingServiceTest.cpp)
target_link_libraries(SpotkajMnieTests gtest_main)

include(GoogleTest)
gtest_discover_tests(SpotkajMnieTests)