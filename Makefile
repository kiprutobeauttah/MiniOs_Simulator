CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/main.cpp src/process.cpp src/scheduler.cpp src/memory.cpp src/gui.cpp
TARGET = MiniOS-GUI-Simulator

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
