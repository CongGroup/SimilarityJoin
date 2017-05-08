CC=g++
BIN=SimilarityJoinService_server
#TestAll
FLAGS=-Wall -g -fpermissive -std=c++0x
RM=-rm -f
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
COMMON_OBJ=$(filter-out SimilarityJoinService_server.o,$(OBJ))

#thrift
THRIFT_LINK=-lthrift
THRIFT_LIB=-L/usr/local/lib

#openssl
OPENSSL_LINK=-lcrypto

#caravel
CARAVEL_LINK=-lcaravel
CARAVEL_LIB=-L../Caravel
CARAVEL_INC=-I../Caravel

#combine the inc & lib
INC=$(OPENSSL_INC) $(THRIFT_INC) $(CARAVEL_INC)
LIB=$(CARAVEL_LINK) $(CARAVEL_LIB) $(THRIFT_LINK) $(THRIFT_LIB) $(OPENSSL_LINK) 


all: $(BIN)

%.d: %.cpp
	$(CC) -MM $(FLAGS) $(INC) $< -o $@

%.o: %.cpp %.d
	$(CC) -c $(FLAGS) $(INC) $< -o $@

SimilarityJoinService_server:$(OBJ)
	mkdir -p ../bin
	$(CC) $(FLAGS) -o ../bin/server $@.o $(COMMON_OBJ) $(LIB)

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) $(BIN)

-include $(SRC:.cpp=.d)
