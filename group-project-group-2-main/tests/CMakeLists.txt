add_executable(test_swaption ${SRC_FILES} test_swaption.cpp)
target_include_directories(test_swaption PUBLIC ${CMAKE_SOURCE_DIR}/extern/catch2 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_swaption COMMAND test_swaption)

add_executable(test_bond ${SRC_FILES} test_bond.cpp)
target_include_directories(test_bond PUBLIC ${CMAKE_SOURCE_DIR}/extern/catch2 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_bond COMMAND test_bond)

add_executable(test_models ${SRC_FILES} test_models.cpp)
target_include_directories(test_models PUBLIC ${CMAKE_SOURCE_DIR}/extern/catch2 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_models COMMAND test_models)

add_executable(test_integration ${SRC_FILES} test_integration.cpp)
target_include_directories(test_integration PUBLIC ${CMAKE_SOURCE_DIR}/extern/catch2 ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_integration COMMAND test_integration)