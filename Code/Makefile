# --- Variables ---
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD
TARGET = bin/main
OBJS = obj/main.o obj/Graph.o obj/Partition.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)


obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include obj/*.d

clean:
	rm -f $(OBJS) $(TARGET)
