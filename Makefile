CC=g++
BLOSSOM_DIR=/home/damaris/git/IJCAI-19/GBFS/Local_Search_IJCAI_18.01.19_Código/Include/blossom5/
MEMORY_DIR=/home/damaris/git/IJCAI-19/GBFS/Local_Search_IJCAI_18.01.19_Código/Include/memory/
CFLAGS=-O3 -fpermissive -std=gnu++11 
INCLUDES=-I$(BLOSSOM_DIR) -I$(MEMORY_DIR)
SOURCES=$(wildcard Source/*.cpp) $(wildcard Include/blossom5/*.cpp) $(wildcard Include/memory/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all:LocalSearch 

LocalSearch:$(OBJECTS) 
	$(CC) $(OBJECTS) -o bin/$@  $(LDFLAGS)

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@ 
.cc.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	
clean:
	rm -f $(OBJECTS)
	
