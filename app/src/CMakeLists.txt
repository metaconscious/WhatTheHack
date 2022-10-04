add_executable(WhatTheHackEx main.cpp)

target_include_directories(WhatTheHackEx PRIVATE ${CMAKE_SOURCE_DIR}/utility/include)
target_link_libraries(WhatTheHackEx PRIVATE Utility)