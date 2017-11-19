#-------------------------------------------------------------
#Assume-se uma distribuição Linux como sistema operacional padrão
#-------------------------------------------------------------

COMPILER = g++
#comando para remover pastas
RMDIR = rm -rdf
#comando para remover arquivos
RM = rm -f

#Flags para geração automática de dependências
DEP_FLAGS = -M -MT $(BIN_PATH)/$(*F).o -MP -MF $@
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm -lpthread

#Se o gcc não reconhecer a flag -fdiagnostics-color basta retirar ela
# FLAGS= -std=c++11 -Wall -pedantic -Wextra -fmax-errors=5 -Wno-unused-parameter -fdiagnostics-color -static-libgcc -static-libstdc++ -Werror=init-self
# FLAGS= -std=c++11 -Wall -pedantic -Wextra -fmax-errors=5 -Wno-unused-parameter -fdiagnostics-color -Werror=init-self
FLAGS= -std=c++11 -Wall -pedantic -Wextra -fmax-errors=5 -Wno-unused-parameter -Werror=init-self
DFLAGS = -ggdb -O0

INC_PATH = include
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

#Uma lista de arquivos por extensão:
CPP_FILES = $(wildcard $(SRC_PATH)/*.cpp)
INC_FILES := $(wildcard $(INC_PATH)/*.h)
FILE_NAMES = $(sort $(notdir $(CPP_FILES:.cpp=)) $(notdir $(INC_FILES:.h=)))
DEP_FILES = $(addprefix $(DEP_PATH)/,$(addsuffix .d,$(FILE_NAMES)))
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))

EXTRA_INC_PATH =


#Nome do executável
EXEC = JOGO

#-------------------------------------------------------------
#Caso o sistema seja windows
#-------------------------------------------------------------
ifeq ($(OS),Windows_NT)
#comando para remover um diretório recursivamente
RMDIR= rd /s /q
#comando para deletar um único arquivo
RM = del /q

#path da SDL
SDL_PATHS = C:/SDL2/x86_64-w64-mingw32 C:/Tools/msys64/mingw64
EXTRA_INC_PATH += $(addsuffix /include,$(SDL_PATHS))
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

INC_PATHS = -I$(INC_PATH) $(addprefix -I,$(EXTRA_INC_PATH)) -IFowardList

.PRECIOUS: $(DEP_FILES)

$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp $(INC_PATH)/%.h | folders
	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS)
$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp | folders
	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS)
$(DEP_PATH)/%.d: $(SRC_PATH)/%.h | folders
	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp $(DEP_PATH)/%.d | folders
	$(COMPILER) $(INC_PATHS) $< -c $(FLAGS) -o $@

clean:
	-$(RMDIR) $(DEP_PATH)
	-$(RMDIR) $(BIN_PATH)

folders:
ifeq ($(OS), Windows_NT)
	@if NOT exist $(DEP_PATH) ( mkdir $(DEP_PATH) )
	@if NOT exist $(BIN_PATH) ( mkdir $(BIN_PATH) )
	@if NOT exist $(INC_PATH) ( mkdir $(INC_PATH) )
	@if NOT exist $(SRC_PATH) ( mkdir $(SRC_PATH) )
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH) $(INC_PATH) $(SRC_PATH)
endif

.PHONY: fdebug clean frelease folders

#regra pra debug
print-% : ; @echo $* = $($*)

fdebug: FLAGS += $(DFLAGS)
fdebug: objects

frelease: FLAGS += -O3 -mtune=native
frelease: objects

objects: $(OBJ_FILES)
	echo "Arquivos objeto do jogo compilados."

unused: FLAGS -= -Wno-unused-parameter

-include $$(DEP_FILES)

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
