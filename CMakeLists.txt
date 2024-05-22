cmake_minimum_required(VERSION 3.28)
project(main)
project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(main main.cpp
        MacroProcessor.cpp
        MacroProcessor.h
        ExpressionEvaluator.cpp
        ExpressionEvaluator.h
        Utility.h
        Utility.cpp)

add_executable(tests ExpressionEvaluatorTests.cpp
        MacroProcessor.cpp
        MacroProcessor.h
        ExpressionEvaluator.cpp
        ExpressionEvaluator.h
        Utility.h
        Utility.cpp
        MacroGeneratorTests.cpp)

target_link_libraries(
        tests
        GTest::gtest_main
)

include(GoogleTest)