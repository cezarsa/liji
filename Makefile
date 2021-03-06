CC ?= gcc

INCLUDE_DIR = .

CFLAGS += -I$(INCLUDE_DIR) -Wall -Werror -Wextra -Wno-unused-parameter
LDFLAGS += -lm

OBJ_DIR = obj
BIN_DIR = bin

DEPS = liji.h
OBJ = $(OBJ_DIR)/liji.o $(OBJ_DIR)/main.o

DEBUG = 1

ifndef DEBUG
	CFLAGS += -m64 -march=core2 -O3
else
	CFLAGS += -g
endif

test: build
	@$(BIN_DIR)/main

all: python build

build: $(BIN_DIR)/main

$(BIN_DIR)/main: $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c $(DEPS)
	mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main
	rm -rf ./python/build
	find ./python -name "*.so" -delete

python:
	cd ./python && PYTHONPATH=. python setup.py build_ext -i

python_dist: test
	cd ./python && PYTHONPATH=. python setup.py sdist upload

bench: python
	cd ./python && PYTHONPATH=. python liji/benchmark.py

.PHONY: clean test build python bench
