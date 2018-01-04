cmake_minimum_required(VERSION 3.7)
project(linda_comm)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        parser/lambda_generator.cpp
        parser/lambda_generator.h
        parser/lexer.cpp
        parser/lexer.h
        parser/parser.h
        parser/parser_exceptions.h
        parser/source.h
        parser/stringSource.cpp
        parser/stringSource.h
        parser/template_parser.cpp
        parser/template_parser.h
        parser/token.cpp
        parser/token.h
        parser/value_parser.cpp
        parser/value_parser.h
        file_manager.cpp
        file_manager.h
        file_exceptions.h
        file_utils.h
        file_utils.cpp
        linda_base.h
        linda_communicator.cpp
        linda_communicator.h
        linda_template.cpp
        linda_template.h
        linda_value.cpp
        linda_value.h
        main.cpp)

add_executable(linda_comm ${SOURCE_FILES})
target_link_libraries(linda_comm rt)