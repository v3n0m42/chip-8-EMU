#ifndef KEYBOARD_HPP
# define KEYBOARD_HPP

# include "Iinput.hpp"

# define SAVECONFIG	sf::Keyboard::F5
# define SAVESTATE	sf::Keyboard::F1
# define LOADSTATE	sf::Keyboard::F2
# define SPEEDUP	sf::Keyboard::Add
# define SLOWDOWN	sf::Keyboard::Subtract
# define ESCAPE		sf::Keyboard::Escape
# define KEY_0		sf::Keyboard::A
# define KEY_1		sf::Keyboard::B
# define KEY_2		sf::Keyboard::Up
# define KEY_3		sf::Keyboard::C
# define KEY_4		sf::Keyboard::Left
# define KEY_5		sf::Keyboard::D
# define KEY_6		sf::Keyboard::Right
# define KEY_7		sf::Keyboard::E
# define KEY_8		sf::Keyboard::Down
# define KEY_9		sf::Keyboard::F
# define KEY_A		sf::Keyboard::G
# define KEY_B		sf::Keyboard::H
# define KEY_C		sf::Keyboard::I
# define KEY_D		sf::Keyboard::J
# define KEY_E		sf::Keyboard::K
# define KEY_F		sf::Keyboard::L

class	Keyboard : public Iinput
{
	public:
		Keyboard(void);
		~Keyboard(void);
		void	updateInput(void);
};

extern "C"
{
	Keyboard	*createInput(void);
	void		deleteInput(Keyboard *input);
}

#endif
