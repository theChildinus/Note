cmake_minimum_required(VERSION 3.3)

PROJECT(lex_yacc_ex)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(OUTPUT ${PROJECT_SOURCE_DIR}/output)

set(lexname "smv_lex")
set(yaccname "smv_yacc")

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)
BISON_TARGET(ex_parser src/${yaccname}.y ${OUTPUT}/${yaccname}.tab.cpp DEFINES_FILE ${OUTPUT}/yacc.tab.h)
FLEX_TARGET(ex_lexer src/${lexname}.l ${OUTPUT}/${lexname}.yy.cpp)
#
ADD_FLEX_BISON_DEPENDENCY(ex_lexer ex_parser)
#
include_directories(${OUTPUT})


add_executable(lex_yacc_ex
        ${BISON_ex_parser_OUTPUTS}
        ${FLEX_ex_lexer_OUTPUTS})

find_library(LEX_LIB lex)
target_link_libraries(lex_yacc_ex ${lex})

