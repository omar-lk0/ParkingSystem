CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
SRC = src/main.cpp src/Application.cpp src/CommandHandler.cpp \
      src/HistoryRepository.cpp src/ParkingRecord.cpp src/ParkingZone.cpp \
      src/RomanianPlate.cpp src/Validators.cpp
TARGET = parking

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) help
