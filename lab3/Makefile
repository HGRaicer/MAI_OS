CC = gcc
CFLAGS = -Wall -g
BUILD_DIR = build
SRC_DIR = src

# Цель по умолчанию
all: $(BUILD_DIR)/parent $(BUILD_DIR)/child

# Правило для компиляции parent
$(BUILD_DIR)/parent: $(SRC_DIR)/parent.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Правило для компиляции child
$(BUILD_DIR)/child: $(SRC_DIR)/child.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Правило для запуска программы
run: all
	echo "$(NUMBERS)" | valgrind ./$(BUILD_DIR)/parent $(FILE)

# Правило для очистки
clean:
	rm -rf $(BUILD_DIR)

