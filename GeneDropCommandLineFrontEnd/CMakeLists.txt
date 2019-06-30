set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(GeneDrop
    src/CommandLineArg.h
	src/CommandLineArg.cpp
    src/commandLineFrontEnd.cpp
    src/CommandLineParser.h
	src/CommandLineParser.cpp
)

include_directories(${BOOST_INCLUDE_DIR})
target_link_libraries(GeneDrop Backend)
target_link_libraries(GeneDrop Threads::Threads)
