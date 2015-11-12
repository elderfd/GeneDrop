add_executable(GeneDrop
    CommandLineArg.h
	CommandLineArg.cpp
    commandLineFrontEnd.cpp
    CommandLineParser.h
	CommandLineParser.cpp
)
target_link_libraries(GeneDrop ${LIB_INCLUDE_DIR}/BackEnd)
include_directories(${LIB_INCLUDE_DIR})
include_directories($(BOOST_INCLUDE_DIR))
