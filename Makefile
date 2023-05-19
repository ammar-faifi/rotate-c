.PHONY: redo clean debug all

ARG := 
CC ?= clang
SRC = $(wildcard src/*.c)
SRC += $(wildcard src/**/*.c)
SRC_C_H = src/**/*.c src/**/*.h
STRICT  = -Werror
CSTD = -std=gnu11
CSTD_LINT = --std=gnu17
DEBUG  = -g -DDEBUG -ggdb3 -pg -fsanitize=address	
BIN  = ./build/vr
CFLAGS := -Wall -Wextra -Wpedantic -ffast-math -Wno-unused 
CFLAGS += -finline-functions -fno-strict-aliasing -funroll-loops
CFLAGS += -march=native -mtune=native -Wwrite-strings -fno-exceptions
CFLAGS += -lm
#CFLAGS += -static


ANALYZE := 
ifeq ($(CC), gcc)
ANALYZE = -fanalyzer
CFLAGS += -Wcast-align=strict
else ifeq ($(CC), clang++)
#ANALYZE = -Xanalyzer
CFLAGS += -Wcast-align
else
CFLAGS += 
endif



all: format
	@cmake --build build


redo: gen build all
	@echo done

gen:
	@cmake -S . -B build -G Ninja

scan:
	@rm -r output
	@scan-build -o ./output $(CC) $(SRC) -o $(BIN) $(CFLAGS) $(DEBUG) $(CSTD) $(LIB)
	@scan-view ./output/

fast:
	$(CC) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(CSTD) $(LIB) -Ofast $(FLAG)

debug:
	$(CC) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB) $(FLAG)

debug_gdb: debug
	gdb --tui $(BIN)
hidden_debug:
	@$(CC) $(SRC) -o $(BIN) $(CFLAGS) $(ANALYZE) $(DEBUG) $(CSTD) $(LIB) -fsanitize=undefined



clean:
	@rm -r output
	@rm -r build


memcheck: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s $(BIN) ./main.vr

lint:
	@cppcheck  $(SRC_C_H) $(CSTD_LINT) --enable=all --check-config --quiet 

format:
	@clang-format -i src/**/*.c src/**/*.h

release:
	$(CC) $(SRC) -O2 -Ofast -o $(BIN) $(CFLAGS) $(CSTD) $(LIB) $(ANALYZE) -Werror $(STRICT) $(FLAG) -s 

release_min_size:
	$(CC) $(SRC) -O2 -Oz -o $(BIN) $(CFLAGS) $(CSTD) $(LIB) $(ANALYZE) -Werror $(STRICT) $(FLAG) -s -fvpt -ftree-loop-optimize

