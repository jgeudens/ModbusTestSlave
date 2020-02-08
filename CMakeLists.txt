cmake_minimum_required(VERSION 3.5)

project(ModbusTestSlave
    LANGUAGES CXX
    DESCRIPTION "Test module to test Modbus communication"
    VERSION 0.0.0
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
    Xml
    PrintSupport
    Network
    SerialBus

    REQUIRED
)

set(QT_INCLUDES
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Xml_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5SerialBus_INCLUDE_DIRS}
)

set(QT_LIB
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::PrintSupport
    Qt5::Network
    Qt5::SerialBus
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/testslave

    ${QT_INCLUDES}
)

SET(APP_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/registerdatamodel.cpp
)

SET(APP_UIS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.ui
)

SET(TESTSLAVE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/testslave/testslavedata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testslave/testslavemodbus.cpp
)

# Default GUI type is blank
set(GUI_TYPE "")

if(WIN32)
    set(GUI_TYPE WIN32)
endif()

add_executable(${PROJECT_NAME} ${GUI_TYPE} ${APP_SRCS} ${APP_UIS} ${TESTSLAVE_SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_LIB}
)

# Reset GUI type to blank
set(GUI_TYPE "")
