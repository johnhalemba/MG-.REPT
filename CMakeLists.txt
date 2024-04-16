cmake_minimum_required(VERSION 3.28)
project(ECOTE_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(ECOTE_Project main.cpp
        MacroProcessor.cpp
        MacroProcessor.h
        ExpressionEvaluator.cpp
        ExpressionEvaluator.h
        Utility.h
        Utility.cpp)
