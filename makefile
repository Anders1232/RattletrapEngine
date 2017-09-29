#-------------------------------------------------------------
#Assume-se uma distribuição Linux como sistema operacional padrão
#-------------------------------------------------------------

COMPILER = g++
#comando para remover pastas
RMDIR = rm -rf
#comando para remover arquivos
RM = rm -f

#Flags para geração automática de dependências
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$.d
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

#Se o gcc não reconhecer a flag -fdiagnostics-color basta retirar ela
# FLAGS= -std=c++11 -Wall -pedantic -Wextra -fmax-errors=5 -Wno-unused-parameter -fdiagnostics-color -static-libgcc -static-libstdc++ -Werror=init-self
#FLAGS= -std=c++11 -Wall -pedantic -Wextra -fmax-errors=5 -Wno-unused-parameter -fdiagnostics-color -Werror=init-self
FLAGS = -std=c++11 -Wall -pedantic -Wextra -fmax-errors=5 -Wno-unused-parameter  -Werror=init-self
DFLAGS = -ggdb -O0

INC_PATH = -Iinclude
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

#Uma lista de arquivos por extensão:
CPP_FILES= $(wildcard $(SRC_PATH)/*.cpp)
OBJ_FILES= $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES = $(wildcard $(DEP_PATH)/*.d)

#Nome do executável
EXEC = JOGO

#-------------------------------------------------------------
#Caso o sistema seja windows
#-------------------------------------------------------------
ifeq ($(OS),Windows_NT)
#comando para remover um diretório recursivamente
RMDIR= rd /s /q
#comando para deletar um único arquivo
RM = del

#path da SDL
SDL_PATH = C:/SDL2/x86_64-w64-mingw32
INC_PATH += -I$(SDL_PATH)/include/SDL2
FLAGS += -mwindows
DFLAGS += -mconsole
LIBS := -lmingw32 -lSDL2main $(LIBS)

#Nome do executável
EXEC := $(EXEC).exe

else
UNAME_S := $(shell uname -s)

#-------------------------------------------------------------
#Caso o sistema seja Mac
#-------------------------------------------------------------

ifeq ($(UNAME_S), Darwin)

LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf

endif
endif

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(COMPILER) -o $@ $^ $(LIBS)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp

ifeq ($(OS), Windows_NT)
	@if not exist $(DEP_PATH) @ mkdir $(DEP_PATH)
	@if not exist $(BIN_PATH) @ mkdir $(BIN_PATH)
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH)
endif

	$(COMPILER) $(DEP_FLAGS) -c -o $@ $< $(INC_PATH) $(FLAGS)

-include $(DEP_FILES)

clean:
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC)

.PHONY: debug clean release again
#regra pra debug
print-% : ; @echo $* = $($*)

debug: FLAGS += $(DFLAGS)
debug: all

fdebug: FLAGS += $(DFLAGS)
fdebug: objects

release: FLAGS += -O3 -mtune=native
release: all

frelease: FLAGS += -O3 -mtune=native
frelease: objects

objects: $(OBJ_FILES)
	echo "Arquivos objeto da engine compilados."

doc: dclean
	doxygen Doxyfile

dclean:
	$(RMDIR) docs
	$(RM) doxygen_sqlite3.db

help:
	@echo.
	@echo Available targets:
	@echo - release:  Builds the release version (default target)
	@echo - debug:    Builds the debug version
	@echo - doc:      Clean and generate Doxygen documentation
	@echo - dclean:   Clean Doxygen documentation
	@echo - profile:  Builds a version to use with gprof (not implemented)
	@echo - coverage: Builds a version to use with gcov (not implemented)
	@echo - help:     Shows this help
	@echo.











