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
	@g++ $^ -o $@ $(SFMLLIBS) -ldl
	@echo "\033[1;32mchip8EMU ready !!\033[00m"

%.o: %.cpp
	@echo "\033[1;30mchip8EMU: compiling \033[00m" $<
	@g++ $(CPPFLAGS) $< -o $@ -c -fPIC

$(LIBGFXSFMLTarget): $(GFXSFMLOBJ)
	@g++ $^ -o $@ -shared -fPIC $(SFMLLIBS)
	@echo "\033[1;32mlibGFXSFML ready !!\033[00m"

$(LIBINPUTKBTarget): $(INPUTKBOBJ)
	@g++ $^ -o $@ -shared -fPIC $(SFMLLIBS)
	@echo "\033[1;32mlibINPUTKB ready !!\033[00m"

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
