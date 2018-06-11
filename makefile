CC = g++
CFLAG = -O0 -std=c++11
SRC = .
BIN = .
BUILD = build
DIRGUARD=@mkdir -p $(@D)
TARGET = main
SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(SOURCES:$(SRC)/%.cpp=$(BUILD)/%.o)

rm = rm -fR

all: build

build: $(OBJECTS)
	@$(CC) $(CFLAG) $(OBJECTS) -o $(BIN)/$(TARGET)
	@echo "\033[0;32mLinking \""$(TARGET)"\" complete!\033[0;0m"
	@$(BIN)/$(TARGET)

$(OBJECTS): $(BUILD)/%.o : $(SRC)/%.cpp
	@$(DIRGUARD)
	@$(CC) $(CFLAG) -c $< -o $@
	@echo "\033[0;32mCompiled \""$<"\" successfully!\033[0;0m"

.PHONE: clean
clean:
	@$(rm) $(BUILD)/*.o
