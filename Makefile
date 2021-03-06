OBJS = lib/game/src/main.cpp lib/engine/src/run.cpp lib/engine/src/collision.cpp lib/engine/src/render.cpp lib/engine/src/texture.cpp lib/engine/src/error.cpp lib/engine/src/log.cpp lib/game/src/menu.cpp lib/game/src/hud.cpp lib/game/src/input.cpp lib/game/src/character.cpp lib/game/src/tetromino.cpp 

CC = g++

COMPILER_FLAGS = -w -Iinclude

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

OBJ_NAME = build/tetris-man

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
