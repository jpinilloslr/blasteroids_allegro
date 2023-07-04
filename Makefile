CC := gcc
CFLAGS := -g -Wall -std=c17
LIBS := -lm -lallegro -lallegro_image -lallegro_font -lallegro_audio -lallegro_acodec -lallegro_primitives
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
DATA_DIR := data

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET := $(BIN_DIR)/blasteroids

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)/$(DATA_DIR)
	$(CC) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(DATA_DIR):
	cp -r $(DATA_DIR) $(BIN_DIR)/$(DATA_DIR)

clean:
	$(RM) -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

.PHONY: all clean
