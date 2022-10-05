add_library(WhatTheHackIn MODULE dllmain.cpp hack.cpp)

target_include_directories(WhatTheHackIn PRIVATE
        ${CMAKE_SOURCE_DIR}/utility/include
        ${CMAKE_SOURCE_DIR}/game/include
        )
target_link_libraries(WhatTheHackIn PRIVATE Utility)