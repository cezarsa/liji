CC ?= gcc

INCLUDE_DIR = .

CFLAGS += -I$(INCLUDE_DIR) -Wall -Werror
LDFLAGS += -lm

OBJ_DIR = obj
BIN_DIR = bin

DEPS = liji.h
OBJ = $(OBJ_DIR)/liji.o $(OBJ_DIR)/main.o

ifndef DEBUG
	CFLAGS += -m64 -march=core2 -O3
else
	CFLAGS += -g
endif

run_main: $(BIN_DIR)/main
	@$(BIN_DIR)/main

$(BIN_DIR)/main: $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c $(DEPS)
	mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main

.PHONY: clean run_main
