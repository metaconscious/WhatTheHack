add_library(WhatTheHackIn MODULE dllmain.cpp)

target_include_directories(WhatTheHackIn PRIVATE ${CMAKE_SOURCE_DIR}/utility/include)
target_link_libraries(WhatTheHackIn PRIVATE Utility)