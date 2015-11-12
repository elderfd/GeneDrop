set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

# Adding WIN32 option here would suppress terminal but have issue where Qt built
# with wrong compiler on dev. machine
add_executable(GeneDropPretty
    geneDropPretty.cpp
    MainWindow.cpp
    MainWindow.h
    ProgramOptions.cpp
    ProgramOptions.h
)
qt5_use_modules(GeneDropPretty Widgets)
# target_link_libraries(GeneDropPretty Qt5::Widgets)
include_directories($(BOOST_INCLUDE_DIR))
