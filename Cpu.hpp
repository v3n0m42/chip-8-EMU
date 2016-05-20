#ifndef CPU_HPP
# define CPU_HPP

# include <string>
# include "Config.hpp"
# include "Iinput.hpp"
# include "IRender.hpp"

# define MEM_SIZE			4096
# define BASE_ADD			512
# define NB_REGISTERS		16
# define NB_JUMPS			16
# define NB_OPCODES			35

class	Cpu
{
	public:
		typedef void (Cpu::*opc_ptr)(unsigned char b1, unsigned char b2, unsigned char b3);
		Cpu(Config *config);
		~Cpu(void);
		void				uncount(void);
		unsigned short		getOpcode(void) const;
		unsigned char		getAction(unsigned short opcode) const;
		void				execOpcode(unsigned short opcode);
		void				setRenderer(const std::string &fileName);
		void				setInput(const std::string &fileName);
		void				render(unsigned char b1, unsigned char b2, unsigned char b3);
		bool				loadROM(const std::string &file);
		void				saveState(void) const;
		void				loadState(void);
		void				render(void) const;
		unsigned short		getNbClocks(void) const;
		void				setNbClocks(unsigned short nbClocks);
		Iinput				*getInput(void) const;
		bool				active;

	private:
		std::string			_romName;
		unsigned short		_nbClocks;
		//Config				*_config;
		IRender				*_renderer;
		void				*_rendererHandle, *_inputHandle;
		IRender				*(*_RendererCreator)(void);
		void				(*_deleteRenderer)(IRender *);
		Iinput				*_input;
		Iinput				*(*_InputCreator)(void);
		void				(*_deleteInput)(Iinput *);
		unsigned char		_memory[MEM_SIZE];
		unsigned char		_V[NB_REGISTERS];	//Registers
		unsigned short		_I;					//Men add
		unsigned short		_jump[NB_JUMPS];	//Mem jumps
		unsigned char		_nbJumps;
		unsigned char		_gameCounter, _soundCounter;
		unsigned short		_pc;
		unsigned short		_mask[NB_OPCODES];
		unsigned short		_id[NB_OPCODES];
		opc_ptr				_opcodes_ptrs[NB_OPCODES];
		/*==================================================*/
		/*===============>>> OPCODES EXEC <<<===============*/
		/*==================================================*/
		void				_0NNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_00E0(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_00EE(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_1NNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_2NNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_3XNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_4XNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_5XY0(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_6XNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_7XNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY0(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY1(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY2(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY3(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY4(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY5(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY6(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XY7(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_8XYE(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_9XY0(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_ANNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_BNNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_CXNN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_DXYN(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_EX9E(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_EXA1(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX07(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX0A(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX15(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX18(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX1E(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX29(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX33(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX55(unsigned char b1, unsigned char b2, unsigned char b3);
		void				_FX65(unsigned char b1, unsigned char b2, unsigned char b3);
};

#endif
