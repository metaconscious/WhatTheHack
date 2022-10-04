add_executable(Injector main.cpp)

target_include_directories(Injector PRIVATE ${CMAKE_SOURCE_DIR}/utility/include)
target_link_libraries(Injector PRIVATE Utility)