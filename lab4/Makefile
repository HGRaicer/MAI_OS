CC = gcc
CFLAGS = -fPIC -Wall
LDFLAGS = -shared
DLFLAGS = -ldl

# Directories
SRC_DIR = src
BUILD_DIR = build

# Targets
all: $(BUILD_DIR)/lib1.so $(BUILD_DIR)/lib2.so $(BUILD_DIR)/static $(BUILD_DIR)/dynamic

# Build first library
$(BUILD_DIR)/lib1.so: $(SRC_DIR)/lib1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lib1.c -o $(BUILD_DIR)/lib1.so

# Build second library
$(BUILD_DIR)/lib2.so: $(SRC_DIR)/lib2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lib2.c -o $(BUILD_DIR)/lib2.so

# Build program1 with compile-time linking
$(BUILD_DIR)/static: $(SRC_DIR)/static.c $(BUILD_DIR)/lib1.so $(BUILD_DIR)/lib2.so
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRC_DIR)/static.c -o $(BUILD_DIR)/static -L$(BUILD_DIR) -l1 -l2

# Build program2 with dynamic loading
$(BUILD_DIR)/dynamic: $(SRC_DIR)/dynamic.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRC_DIR)/dynamic.c -o $(BUILD_DIR)/dynamic $(DLFLAGS)

# Generate run scripts
$(BUILD_DIR)/run_static: $(BUILD_DIR)/static
	echo "#!/bin/bash" > $(BUILD_DIR)/run_static
	echo "export LD_LIBRARY_PATH=$(BUILD_DIR):\$LD_LIBRARY_PATH" >> $(BUILD_DIR)/run_static
	echo "$(BUILD_DIR)/static" >> $(BUILD_DIR)/run_static
	chmod +x $(BUILD_DIR)/run_static

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR)
