CC = gcc
CFLAGS = -fPIC -Wall
LDFLAGS = -shared
DLFLAGS = -ldl

# Directories
SRC_DIR = src
BUILD_DIR = build

# Targets
all: $(BUILD_DIR)/lib1.so $(BUILD_DIR)/lib2.so $(BUILD_DIR)/lib1.a  $(BUILD_DIR)/static $(BUILD_DIR)/dynamic

# Build first static library
$(BUILD_DIR)/lib1.a: $(SRC_DIR)/lib1.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/lib1.c -o $(BUILD_DIR)/lib1.o
	ar rcs $(BUILD_DIR)/lib1.a $(BUILD_DIR)/lib1.o

# Build second static library
#$(BUILD_DIR)/lib2.a: $(SRC_DIR)/lib2.c
#	mkdir -p $(BUILD_DIR)
#	$(CC) -c $(SRC_DIR)/lib2.c -o $(BUILD_DIR)/lib2.o
#	ar rcs $(BUILD_DIR)/lib2.a $(BUILD_DIR)/lib2.o

# Build first shared library
$(BUILD_DIR)/lib1.so: $(SRC_DIR)/lib1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lib1.c -o $(BUILD_DIR)/lib1.so

# Build second shared library	
$(BUILD_DIR)/lib2.so: $(SRC_DIR)/lib2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lib2.c -o $(BUILD_DIR)/lib2.so

# Build program1 with compile-time linking to static libraries
$(BUILD_DIR)/static: $(SRC_DIR)/static.c $(BUILD_DIR)/lib1.a 
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRC_DIR)/static.c -o $(BUILD_DIR)/static $(BUILD_DIR)/lib1.a #$(BUILD_DIR)/lib2.a 

# Build program2 with dynamic loading
$(BUILD_DIR)/dynamic: $(SRC_DIR)/dynamic.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRC_DIR)/dynamic.c -o $(BUILD_DIR)/dynamic $(DLFLAGS)


# Clean up generated files
clean:
	rm -rf $(BUILD_DIR)
