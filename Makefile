TARGET = spi_ili9341
BUILD_DIR = build
SRC_DIR = src
INC_DIR = inc

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
DEPS = $(wildcard $(INC_DIR)/*.h)
CFLAGS = $(patsubst %, -I%, $(INC_DIR))

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	ln -sf $(BUILD_DIR)/$(TARGET) test

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean cleanall

clean:
	rm -rf $(BUILD_DIR)

cleanall:
	rm -rf $(BUILD_DIR)