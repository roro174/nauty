# --- Variables ---
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD
TARGET = bin/main
OBJS = obj/main.o obj/Graph.o obj/Partition.o obj/bsgs.o


all: dirs $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)


obj/%.o: %.cpp
	@mkdir -p obj   # <-- crée le dossier s'il n'existe pas
	$(CXX) $(CXXFLAGS) -c $< -o $@


dirs:
	@mkdir -p obj bin


-include obj/*.d

# --- Nettoyage ---
clean:
	rm -f $(OBJS) $(TARGET) obj/*.d
