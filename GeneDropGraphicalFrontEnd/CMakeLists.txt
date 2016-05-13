set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(SET CMP0043 OLD)

find_package(Qt5Widgets REQUIRED)

IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

# Adding WIN32 option here would suppress terminal but have issue where Qt built
# with wrong compiler on dev. machine
add_executable(GeneDropGUI
    ${GUI_TYPE}
    geneDropGUI.cpp
    MainWindow.cpp
    MainWindow.h
    ProgramOptions.cpp
    ProgramOptions.h
)
qt5_use_modules(GeneDropGUI Widgets)
target_link_libraries(GeneDropGUI ${LIB_INCLUDE_DIR}/BackEnd)
