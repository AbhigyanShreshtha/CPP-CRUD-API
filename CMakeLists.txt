cmake_minimum_required(VERSION 3.15)
project(CPP_CRUD_API)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Specify paths for Boost libraries installed via Homebrew
set(BOOST_ROOT /opt/homebrew/opt/boost)
set(BOOST_INCLUDEDIR /opt/homebrew/include)
set(BOOST_LIBRARYDIR /opt/homebrew/lib)

# Find Boost libraries
find_package(Boost REQUIRED COMPONENTS system filesystem json)

# Include Boost and project directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

# Link to Boost libraries explicitly in case CMake needs guidance
link_directories(${Boost_LIBRARYDIR})

# Specify source files
set(SOURCES
    src/app/main.cpp
    src/controllers/CrudController.cpp
    src/utils/WebSocketBroadcaster.cpp
)

# Create executable
add_executable(CPP_CRUD_API ${SOURCES})

# Link Boost libraries
target_link_libraries(CPP_CRUD_API ${Boost_LIBRARIES})

