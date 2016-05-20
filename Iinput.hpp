#ifndef IINPUT_H
# define IINPUT_H

class	Iinput
{
	public:
		virtual			~Iinput(void) {}
		virtual void	updateInput(void) = 0;

		bool			escape;
		bool			keys[16];
		bool			newKey;
		bool			speedup, slowdown;
		bool			savestate, loadstate;
		bool			saveConfig;
};

#endif
