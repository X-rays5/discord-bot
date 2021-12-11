cmake_minimum_required(VERSION 3.20)
project(discord_cpp_bot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(websocketpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.40 COMPONENTS random REQUIRED)

add_subdirectory(vendor/cpr)
add_subdirectory(vendor/fmt)

add_executable(discord_cpp_bot src/main.cpp src/bot/client/client.hpp src/bot/connection/heartbeat.hpp src/bot/connection/websocket.hpp src/bot/threadpool/threadpool.hpp src/bot/datatypes.hpp src/bot/connection/heartbeat.cpp src/bot/connection/websocket.cpp src/bot/events/event_handler.hpp src/bot/events/message_create.hpp src/bot/events/command_handler.hpp src/bot/api/channel.hpp src/bot/api/api.hpp src/bot/api/user.hpp src/bot/threadpool/threadpool.cpp src/bot/client/client.cpp src/bot/events/message_create.cpp src/bot/events/event_handler.cpp src/bot/events/command_handler.cpp src/main.hpp src/bot/commands/commands.cpp src/bot/commands/commands.hpp)

target_include_directories(discord_cpp_bot PRIVATE vendor/rapidjson/include)

target_link_libraries(discord_cpp_bot PRIVATE websocketpp::websocketpp)
target_link_libraries(discord_cpp_bot PRIVATE fmt::fmt)
target_link_libraries(discord_cpp_bot PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(discord_cpp_bot PRIVATE ${Boost_LIBRARIES})

target_link_libraries(discord_cpp_bot PRIVATE cpr)
