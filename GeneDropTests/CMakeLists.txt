add_executable(Tests tests.cpp catch.hpp)
target_link_libraries(Tests ${LIB_INCLUDE_DIR}/BackEnd)
include_directories(${LIB_INCLUDE_DIR})
include_directories($(BOOST_INCLUDE_DIR))

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TestFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
