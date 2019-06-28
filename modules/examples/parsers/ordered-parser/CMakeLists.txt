add_library(ordered-parser STATIC
  ordered-parser-parser.c
  ordered-parser-parser.h
  ordered-parser.c
  ordered-parser.h
  ${CMAKE_CURRENT_BINARY_DIR}/ordered-parser-grammar.c
  ${CMAKE_CURRENT_BINARY_DIR}/ordered-parser-grammar.h
)

target_link_libraries(ordered-parser PUBLIC syslog-ng)
target_include_directories(ordered-parser
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

generate_y_from_ym(modules/examples/parsers/ordered-parser/ordered-parser-grammar)
bison_target(MsgGeneratorGrammar
    ${CMAKE_CURRENT_BINARY_DIR}/ordered-parser-grammar.y
    ${CMAKE_CURRENT_BINARY_DIR}/ordered-parser-grammar.c
    COMPILE_FLAGS ${BISON_FLAGS})
