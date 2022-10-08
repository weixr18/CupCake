CC:= g++
INCLUDE:= -I./include
LIBS:= -lstdc++
CXXFLAGS:= -std=c++11 -g
DIR_SRC:= ./src
DIR_OBJ:= ./obj
DIR_OBJ_WIN:= .\obj
TARGET:= main.exe
OBJECTS := utils.o ll1_table.o mtree.o cup_scanner.o cup_parser.o cup_analyser.o cup_generator.o main.o

OBJECTS := $(addprefix $(DIR_OBJ)/,$(OBJECTS))

all: $(TARGET)

$(shell mkdir obj)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp   
	$(CC) -c $(CXXFLAGS) $(INCLUDE) $< -o $@
  
.PHONY : clean
clean:   
	-del $(DIR_OBJ_WIN)\*.o
	-del $(TARGET) 
	rmdir obj
