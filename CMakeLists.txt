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
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/Memo-proto/model
        )

target_sources(Memo_client_gui
        PRIVATE
        src/main.cpp
        src/MainWindow.cpp
        src/model/Memo.cpp
        src/model/Tag.cpp
        src/model/conversions.cpp
        src/remote/GrpcClient.cpp
        src/remote/GrpcClientAdapter.cpp
        src/remote/ClientAdapterUtils.cpp
        src/remote/model/ListMemos.cpp
        src/remote/model/AddMemo.cpp
        src/remote/model/UpdateMemo.cpp
        src/remote/model/RemoveMemo.cpp
        src/remote/model/ListTags.cpp
        src/remote/model/AddTag.cpp
        src/remote/model/UpdateTag.cpp
        src/remote/model/RemoveTag.cpp

        PUBLIC
        src/remote/IGrpcClient.hpp
        src/remote/GrpcClient.hpp
        src/remote/IGrpcClientAdapter.hpp
        src/remote/GrpcClientAdapter.hpp
        src/remote/ClientAdapterUtils.hpp
        src/remote/GrpcResponse.hpp
        src/remote/IGrpcClientAdapter.hpp
        src/remote/model/ModelView.hpp
        src/remote/model/OperationStatus.hpp
        src/remote/model/ListMemos.hpp
        src/remote/model/AddMemo.hpp
        src/remote/model/UpdateMemo.hpp
        src/remote/model/RemoveMemo.hpp
        src/remote/model/ListTags.hpp
        src/remote/model/AddTag.hpp
        src/remote/model/UpdateTag.hpp
        src/remote/model/RemoveTag.hpp
        )
target_link_libraries(Memo_client_gui
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${memo_proto_lib}
        )
