# Makefile para o TP3 de AEDS 3

# Compilador e flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude/
LDFLAGS :=

# Diretorios
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := include

# Executável
TARGET := $(BIN_DIR)/tp3.out

# Arquivos fonte e objeto
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Regra principal: criar o executável
all: $(TARGET)

# Regra para linkar o executável
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Regra para compilar os fontes em objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Regra para limpar os arquivos gerados
clean:
	@echo "Limpando arquivos gerados..."
	@rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Phony targets
.PHONY: all clean