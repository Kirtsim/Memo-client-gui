cmake_minimum_required(VERSION 3.20)
project(Memo_client_gui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt6_DIR $ENV{QT6_CMAKE_DIR})
set(CMAKE_PREFIX_PATH $ENV{QT6_INSTALL_DIR})
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Memo_client_gui)

set(memo_proto_lib memo_proto)
add_subdirectory(${CMAKE_SOURCE_DIR}/Memo-proto)

target_include_directories(Memo_client_gui
        PRIVATE
        src
        )

target_sources(Memo_client_gui
        PRIVATE
        src/main.cpp
        src/MainWindow.cpp
        src/model/Memo.cpp
        src/model/Tag.cpp
        )
target_link_libraries(Memo_client_gui
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
