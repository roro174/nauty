# --- Variables ---
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = main
OBJS = main.o Graph.o Partition.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)


main.o: main.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c Graph.cpp

Partition.o : Partition.cpp Partition.hpp
	$(CXX) $(CXXFLAGS) -c Partition.cpp


clean:
	rm -f $(OBJS) $(TARGET)
