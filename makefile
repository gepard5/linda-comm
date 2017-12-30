CXX=g++
CFLAGS=-Wall -std=c++14


DEPS=token.h lexer.h source.h parser_exceptions.h parser.h file_manager.h lambda_generator.h linda_base.h linda_parser.h linda_template.h linda_value.h template_parser.h stringSource.h
OBJ=main.o token.o lexer.o lambda_generator.o linda_parser.o linda_template.o template_parser.o linda_value.o linda_communicator.o stringSource.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) 

lindaCommunicator: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
