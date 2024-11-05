cmake_minimum_required(VERSION 3.15)
project(CPP_CRUD_API)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system filesystem json)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(CPP_CRUD_API
    src/app/main.cpp
    src/controllers/CrudController.cpp
    src/services/InMemoryKeyValueService.hpp
    src/utils/JsonUtil.hpp
    src/utils/WebSocketBroadcaster.cpp
)

target_link_libraries(CPP_CRUD_API ${Boost_LIBRARIES})
