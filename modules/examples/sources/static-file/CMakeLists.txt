add_library(static-file STATIC
  static-file-parser.c
  static-file-parser.h
  static-file.c
  static-file.h
  ${CMAKE_CURRENT_BINARY_DIR}/static-file-grammar.c
  ${CMAKE_CURRENT_BINARY_DIR}/static-file-grammar.h
)

target_link_libraries(static-file PUBLIC syslog-ng)
target_include_directories(static-file
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

generate_y_from_ym(modules/examples/sources/static-file/static-file-grammar)
bison_target(MsgGeneratorGrammar
    ${CMAKE_CURRENT_BINARY_DIR}/static-file-grammar.y
    ${CMAKE_CURRENT_BINARY_DIR}/static-file-grammar.c
    COMPILE_FLAGS ${BISON_FLAGS})
