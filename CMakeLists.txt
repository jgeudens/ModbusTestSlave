cmake_minimum_required(VERSION 3.5)

project(ModbusSim
    LANGUAGES CXX
    DESCRIPTION "Test module to test Modbus communication"
    VERSION 0.0.0
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

# set platform specific for compilation
if(WIN32)
    set(target_dir "bin/win")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(target_dir "bin/linux")
else()
        message(SEND_ERROR "You are on an unsupported platform! (Not Win, Mac OS X or Unix)")
endif(WIN32)

add_executable(modbussim ${APP_SRCS} ${TESTSLAVE_SRCS})

set_target_properties(modbussim PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${target_dir} )

target_link_libraries(modbussim ${QT_LIB})

# Do platform specific post target stuff
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    install(TARGETS modbussim RUNTIME DESTINATION bin)

elseif(WIN32)
    # not required
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
