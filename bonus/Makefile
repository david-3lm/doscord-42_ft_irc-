NAME = botini

# Archivos fuente

SRC_DIR = src
OBJ_DIR = obj

SRC_FILES = main.cpp \
	Botini.cpp


HEADERS =

CXX = c++
#CXXFLAGS = -Wall -Werror -Wextra -Iinclude -std=c++98 
CXXFLAGS = -Iinclude -std=c++98 
RM = rm -f

# COLORES
COLOREND = \033[0m
GREEN = \033[1;32m
RED = \e[1;31m
YELLOW = \e[1;33m
BLUE = \033[0;34m
CIAN = \e[7;36m

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) -Linclude

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	@echo "$(BLUE)Limpio$(COLOREND)"

fclean: clean
	$(RM) $(NAME)
	@echo "$(BLUE)Limpio$(COLOREND)"

re: fclean all

exec:
	./$(NAME)

debug: CXXFLAGS += -g3 -O0
debug: re

val:debug
	valgrind ./$(NAME)

.PHONY: all clean fclean re exec norma debug 
