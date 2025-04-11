CC = sdcc
CFLAGS = -mmcs51 --model-large --std-c99 -I./include \
		--xram-loc 0xF000 --xram-size 0x1000 \
		--code-loc 0x0000 --code-size 0x8000 \
		--iram-size 0x0100
LDFLAGS = -mmcs51 --model-large --std-c99 -I./include \
		--xram-loc 0xF000 --xram-size 0x1000 \
		--code-loc 0x0000 --code-size 0x8000 \
		--iram-size 0x0100

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.rel)
TARGET = $(BIN_DIR)/cc2510_flash

.PHONY: all clean

all: $(TARGET).hex

$(TARGET).hex: $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET).ihx $^
	packihx $(TARGET).ihx > $(TARGET).hex
	@rm -f $(TARGET).ihx $(TARGET).lk $(TARGET).map $(TARGET).mem $(TARGET).sym

$(OBJ_DIR)/%.rel: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.rel $(OBJ_DIR)/*.asm $(OBJ_DIR)/*.lst $(OBJ_DIR)/*.sym
	rm -f $(BIN_DIR)/*.ihx $(BIN_DIR)/*.lk $(BIN_DIR)/*.map $(BIN_DIR)/*.mem $(BIN_DIR)/*.sym $(BIN_DIR)/*.hex