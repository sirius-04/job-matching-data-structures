CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./models -I./controllers -I./utils -I./LinkedList -I./LinkedList/JobLinkedList -I./LinkedList/ResumeLinkedList

SRC = $(wildcard *.cpp) \
			$(wildcard Array/**/*.cpp) \
      $(wildcard LinkedList/**/*.cpp) \
      $(wildcard CircularLinkedList/**/*.cpp) \
      $(wildcard JobMatching/*.cpp) \
      $(wildcard models/**/*.cpp) \
      $(wildcard utils/**/*.cpp)

OBJ = $(SRC:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(subst /,\\,$(OBJ)) $(TARGET).exe 2>nul || exit 0
else
	rm -f $(OBJ) $(TARGET)
endif

