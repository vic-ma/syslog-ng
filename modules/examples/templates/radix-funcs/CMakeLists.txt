add_library(radix-funcs STATIC
  radix-funcs.c
  radix-funcs.h
)

target_link_libraries(radix-funcs PUBLIC syslog-ng)
target_include_directories(radix-funcs
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

add_test_subdirectory(tests)
