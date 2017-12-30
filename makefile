CXX=g++
CFLAGS=-Wall -std=c++14

DEPS=token.h lexer.h source.h parser_exceptions.h parser.h file_manager.h lambda_generator.h linda_base.h value_parser.h linda_template.h linda_value.h template_parser.h stringSource.h
OBJ=main.o parser/token.o parser/lexer.o parser/lambda_generator.o parser/value_parser.o linda_template.o parser/template_parser.o linda_value.o linda_communicator.o parser/stringSource.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) 

lindaCommunicator: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
	rm parser/*.o
