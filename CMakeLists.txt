# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProgram)

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

# Add more source files here if needed
set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/Bond.cpp 
                ${CMAKE_SOURCE_DIR}/src/Bond.hpp
                ${CMAKE_SOURCE_DIR}/src/CIRModel.cpp
                ${CMAKE_SOURCE_DIR}/src/CIRModel.hpp
                ${CMAKE_SOURCE_DIR}/src/InterestRateModel.hpp
                ${CMAKE_SOURCE_DIR}/src/RateSimulator.cpp
                ${CMAKE_SOURCE_DIR}/src/RateSimulator.hpp
                ${CMAKE_SOURCE_DIR}/src/VasicekModel.cpp
                ${CMAKE_SOURCE_DIR}/src/VasicekModel.hpp
                ${CMAKE_SOURCE_DIR}/src/Swaption.cpp
                ${CMAKE_SOURCE_DIR}/src/Swaption.hpp)

add_executable(my_program ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(my_program m)

# Ensure the data directory exists after build
add_custom_command(TARGET my_program POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data)



####################### TESTING STUFF STARTS HERE ###########################################################

enable_testing()
add_subdirectory(tests)