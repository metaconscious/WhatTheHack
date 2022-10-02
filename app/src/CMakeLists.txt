add_executable(WhatTheHackEX EXCLUDE_FROM_ALL main.cpp)

target_include_directories(WhatTheHackEX PRIVATE ${CMAKE_SOURCE_DIR}/utility/include)
target_link_libraries(WhatTheHackEX PRIVATE Utility)