INC = -I. \
      -I./Array \
      -I./LinkedList \
      -I./JobMatching \
      -I./models \
      -I./utils

SRC = $(wildcard *.cpp) \
			$(wildcard Array/**/*.cpp) \
      $(wildcard LinkedList/**/*.cpp) \
      $(wildcard JobMatching/*.cpp) \
      $(wildcard models/**/*.cpp) \
      $(wildcard utils/**/*.cpp)

CXX = g++
CXXFLAGS = -std=c++17 -Wall $(INC)
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

