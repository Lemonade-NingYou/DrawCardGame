CC = clang
CFLAGS = -Wall -Wextra -std=c99 -I.
LDFLAGS = 
TARGET = gacha_system
SRC_DIR = .
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
HEADERS = $(wildcard $(SRC_DIR)/*.h)

.PHONY: all clean

all: $(OBJ_DIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Dependencies
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/card.h $(SRC_DIR)/card_pool.h \
                   $(SRC_DIR)/user_data.h $(SRC_DIR)/gacha.h $(SRC_DIR)/file_io.h
$(OBJ_DIR)/card.o: $(SRC_DIR)/card.c $(SRC_DIR)/card.h
$(OBJ_DIR)/card_pool.o: $(SRC_DIR)/card_pool.c $(SRC_DIR)/card_pool.h $(SRC_DIR)/card.h
$(OBJ_DIR)/user_data.o: $(SRC_DIR)/user_data.c $(SRC_DIR)/user_data.h $(SRC_DIR)/card.h
$(OBJ_DIR)/gacha.o: $(SRC_DIR)/gacha.c $(SRC_DIR)/gacha.h $(SRC_DIR)/card.h \
                    $(SRC_DIR)/card_pool.h $(SRC_DIR)/user_data.h
$(OBJ_DIR)/file_io.o: $(SRC_DIR)/file_io.c $(SRC_DIR)/file_io.h $(SRC_DIR)/card.h \
                      $(SRC_DIR)/user_data.h $(SRC_DIR)/card_pool.h