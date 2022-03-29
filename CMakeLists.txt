cmake_minimum_required(VERSION 3.16)

project(ModbusSim
    LANGUAGES CXX
    DESCRIPTION "Modbus Slave Simulation"
    VERSION 0.0.0
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake to run moc when necessary:
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

set(QT_LIB
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Xml
    Qt::PrintSupport
    Qt::Network
    Qt::SerialBus
)

include_directories(SYSTEM ${QT_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/testslave)

SET(APP_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/registerdatamodel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.ui

    ${CMAKE_CURRENT_SOURCE_DIR}/src/app_icon.rc
)

SET(TESTSLAVE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/testslave/testslavedata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testslave/testslavemodbus.cpp
)

# Default GUI type is blank
set(GUI_TYPE "")

if(WIN32)
    set(GUI_TYPE WIN32)
    set(target_dir "bin/win")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(target_dir "bin/linux")
else()
    message(SEND_ERROR "You are on an unsupported platform! (Not Win or Unix)")
endif(WIN32)

add_executable(${PROJECT_NAME} ${GUI_TYPE} ${APP_SRCS} ${TESTSLAVE_SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${target_dir} )

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SCOPESOURCE}
    ${QT_LIB}
)

# Reset GUI type to blank
set(GUI_TYPE "")

# Do platform specific post target stuff
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

elseif(WIN32)
    # not required
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
