Target =			chip8EMU
Sources =			Cpu.cpp				\
					libGFXSFML.cpp		\
					Config.cpp			\
					main.cpp

LIBGFXSFMLTarget =	libGFXSFML.so
LIBGFXSFMLSources =	libGFXSFML.cpp		\
					Config.cpp

LIBINPUTKBTarget =	libKB.so
LIBINPUTKBSources =	Keyboard.cpp

OBJ = $(Sources:.cpp=.o)
GFXSFMLOBJ = $(LIBGFXSFMLSources:.cpp=.o)
INPUTKBOBJ = $(LIBINPUTKBSources:.cpp=.o)
CPPFLAGS =	-Wall -Werror -g
SFMLLIBS =	-lsfml-graphics		\
			-lsfml-system		\
			-lsfml-window

all: $(LIBGFXSFMLTarget) $(LIBINPUTKBTarget) $(Target)

$(Target): $(OBJ)
	@clang++ $^ -o $@ $(SFMLLIBS) -ldl
	@echo "chip8EMU ready"

%.o: %.cpp
	@echo "chip8EMU: compiling " $<
	@clang++ $(CPPFLAGS) $< -o $@ -c -fPIC

$(LIBGFXSFMLTarget): $(GFXSFMLOBJ)
	@clang++ $^ -o $@ -shared -fPIC $(SFMLLIBS)
	@echo "libGFXSFML ready"

$(LIBINPUTKBTarget): $(INPUTKBOBJ)
	@clang++ $^ -o $@ -shared -fPIC $(SFMLLIBS)
	@echo "libINPUTKB ready"

clean:
	@$(RM) -f $(OBJ)
	@$(RM) -f $(GFXSFMLOBJ)
	@$(RM) -f $(INPUTKBOBJ)

fclean: clean
	@$(RM) $(Target)
	@$(RM) $(LIBGFXSFMLTarget)
	@$(RM) $(LIBINPUTKBTarget)

re: fclean all

.PHONY: all clean fclean re
