#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include "IRender.hpp"
#include "Cpu.hpp"

void			dlErrorWrapper(void)
{
	std::cerr << "Error: " << dlerror() << std::endl;
	exit(-1);
}

Cpu::Cpu(Config *config) : active(true), _romName("save"), _nbClocks(4), _renderer(NULL), _rendererHandle(NULL), _RendererCreator(NULL), _deleteRenderer(NULL), _input(NULL),  _I(0), _nbJumps(0), _gameCounter(0), _soundCounter(0), _pc(512)
{
	unsigned short	i;

	for (i = 0; i < MEM_SIZE; i++)
		this->_memory[i] = 0;
	for (i = 0; i < 16; i++)
		this->_V[i] = 0;
	for (i = 0; i < 16; i++)
		this->_jump[i] = 0;
	//Register opcodes arrays for value and mask
	this->_mask[0]= 0x0000; this->_id[0]=0x0FFF;	/* 0NNN */
	this->_mask[1]= 0xFFFF; this->_id[1]=0x00E0;	/* 00E0 */
	this->_mask[2]= 0xFFFF; this->_id[2]=0x00EE;	/* 00EE */
	this->_mask[3]= 0xF000; this->_id[3]=0x1000;	/* 1NNN */
	this->_mask[4]= 0xF000; this->_id[4]=0x2000;	/* 2NNN */
	this->_mask[5]= 0xF000; this->_id[5]=0x3000;	/* 3XNN */
	this->_mask[6]= 0xF000; this->_id[6]=0x4000;	/* 4XNN */
	this->_mask[7]= 0xF00F; this->_id[7]=0x5000;	/* 5XY0 */
	this->_mask[8]= 0xF000; this->_id[8]=0x6000;	/* 6XNN */
	this->_mask[9]= 0xF000; this->_id[9]=0x7000;	/* 7XNN */
	this->_mask[10]= 0xF00F; this->_id[10]=0x8000;	/* 8XY0 */
	this->_mask[11]= 0xF00F; this->_id[11]=0x8001;	/* 8XY1 */
	this->_mask[12]= 0xF00F; this->_id[12]=0x8002;	/* 8XY2 */
	this->_mask[13]= 0xF00F; this->_id[13]=0x8003;	/* BXY3 */
	this->_mask[14]= 0xF00F; this->_id[14]=0x8004;	/* 8XY4 */
	this->_mask[15]= 0xF00F; this->_id[15]=0x8005;	/* 8XY5 */
	this->_mask[16]= 0xF00F; this->_id[16]=0x8006;	/* 8XY6 */
	this->_mask[17]= 0xF00F; this->_id[17]=0x8007;	/* 8XY7 */
	this->_mask[18]= 0xF00F; this->_id[18]=0x800E;	/* 8XYE */
	this->_mask[19]= 0xF00F; this->_id[19]=0x9000;	/* 9XY0 */
	this->_mask[20]= 0xF000; this->_id[20]=0xA000;	/* ANNN */
	this->_mask[21]= 0xF000; this->_id[21]=0xB000;	/* BNNN */
	this->_mask[22]= 0xF000; this->_id[22]=0xC000;	/* CXNN */
	this->_mask[23]= 0xF000; this->_id[23]=0xD000;	/* DXYN */
	this->_mask[24]= 0xF0FF; this->_id[24]=0xE09E;	/* EX9E */
	this->_mask[25]= 0xF0FF; this->_id[25]=0xE0A1;	/* EXA1 */
	this->_mask[26]= 0xF0FF; this->_id[26]=0xF007;	/* FX07 */
	this->_mask[27]= 0xF0FF; this->_id[27]=0xF00A;	/* FX0A */
	this->_mask[28]= 0xF0FF; this->_id[28]=0xF015;	/* FX15 */
	this->_mask[29]= 0xF0FF; this->_id[29]=0xF018;	/* FX18 */
	this->_mask[30]= 0xF0FF; this->_id[30]=0xF01E;	/* FX1E */
	this->_mask[31]= 0xF0FF; this->_id[31]=0xF029;	/* FX29 */
	this->_mask[32]= 0xF0FF; this->_id[32]=0xF033;	/* FX33 */
	this->_mask[33]= 0xF0FF; this->_id[33]=0xF055;	/* FX55 */
	this->_mask[34]= 0xF0FF; this->_id[34]=0xF065;	/* FX65 */
	//Register opcodes methods
	this->_opcodes_ptrs[0] = &Cpu::_0NNN;
	this->_opcodes_ptrs[1] = &Cpu::_00E0;
	this->_opcodes_ptrs[2] = &Cpu::_00EE;
	this->_opcodes_ptrs[3] = &Cpu::_1NNN;
	this->_opcodes_ptrs[4] = &Cpu::_2NNN;
	this->_opcodes_ptrs[5] = &Cpu::_3XNN;
	this->_opcodes_ptrs[6] = &Cpu::_4XNN;
	this->_opcodes_ptrs[7] = &Cpu::_5XY0;
	this->_opcodes_ptrs[8] = &Cpu::_6XNN;
	this->_opcodes_ptrs[9] = &Cpu::_7XNN;
	this->_opcodes_ptrs[10] = &Cpu::_8XY0;
	this->_opcodes_ptrs[11] = &Cpu::_8XY1;
	this->_opcodes_ptrs[12] = &Cpu::_8XY2;
	this->_opcodes_ptrs[13] = &Cpu::_8XY3;
	this->_opcodes_ptrs[14] = &Cpu::_8XY4;
	this->_opcodes_ptrs[15] = &Cpu::_8XY5;
	this->_opcodes_ptrs[16] = &Cpu::_8XY6;
	this->_opcodes_ptrs[17] = &Cpu::_8XY7;
	this->_opcodes_ptrs[18] = &Cpu::_8XYE;
	this->_opcodes_ptrs[19] = &Cpu::_9XY0;
	this->_opcodes_ptrs[20] = &Cpu::_ANNN;
	this->_opcodes_ptrs[21] = &Cpu::_BNNN;
	this->_opcodes_ptrs[22] = &Cpu::_CXNN;
	this->_opcodes_ptrs[23] = &Cpu::_DXYN;
	this->_opcodes_ptrs[24] = &Cpu::_EX9E;
	this->_opcodes_ptrs[25] = &Cpu::_EXA1;
	this->_opcodes_ptrs[26] = &Cpu::_FX07;
	this->_opcodes_ptrs[27] = &Cpu::_FX0A;
	this->_opcodes_ptrs[28] = &Cpu::_FX15;
	this->_opcodes_ptrs[29] = &Cpu::_FX18;
	this->_opcodes_ptrs[30] = &Cpu::_FX1E;
	this->_opcodes_ptrs[31] = &Cpu::_FX29;
	this->_opcodes_ptrs[32] = &Cpu::_FX33;
	this->_opcodes_ptrs[33] = &Cpu::_FX55;
	this->_opcodes_ptrs[34] = &Cpu::_FX65;
	//Register Chip-8 system Fonts
	this->_memory[0] = 0xF0; this->_memory[1] = 0x90; this->_memory[2] = 0x90; this->_memory[3] = 0x90; this->_memory[4] = 0xF0;
	this->_memory[5] = 0x20; this->_memory[6] = 0x60; this->_memory[7] = 0x20; this->_memory[8] = 0x20; this->_memory[9] = 0x70;
	this->_memory[10] = 0xF0; this->_memory[11] = 0x10; this->_memory[12] = 0xF0; this->_memory[13] = 0x80; this->_memory[14] = 0xF0;
	this->_memory[15] = 0xF0; this->_memory[16] = 0x10; this->_memory[17] = 0xF0; this->_memory[18] = 0x10; this->_memory[19] = 0xF0;
	this->_memory[20] = 0x90; this->_memory[21] = 0x90; this->_memory[22] = 0xF0; this->_memory[23] = 0x10; this->_memory[24] = 0x10;
	this->_memory[25] = 0xF0; this->_memory[26] = 0x80; this->_memory[27] = 0xF0; this->_memory[28] = 0x10; this->_memory[29] = 0xF0;
	this->_memory[30] = 0xF0; this->_memory[31] = 0x80; this->_memory[32] = 0xF0; this->_memory[33] = 0x90; this->_memory[34] = 0xF0;
	this->_memory[35] = 0xF0; this->_memory[36] = 0x10; this->_memory[37] = 0x20; this->_memory[38] = 0x40; this->_memory[39] = 0x40;
	this->_memory[40] = 0xF0; this->_memory[41] = 0x90; this->_memory[42] = 0xF0; this->_memory[43] = 0x90; this->_memory[44] = 0xF0;
	this->_memory[45] = 0xF0; this->_memory[46] = 0x90; this->_memory[47] = 0xF0; this->_memory[48] = 0x10; this->_memory[49] = 0xF0;
	this->_memory[50] = 0xF0; this->_memory[51] = 0x90; this->_memory[52] = 0xF0; this->_memory[53] = 0x90; this->_memory[54] = 0x90;
	this->_memory[55] = 0xE0; this->_memory[56] = 0x90; this->_memory[57] = 0xE0; this->_memory[58] = 0x90; this->_memory[59] = 0xE0;
	this->_memory[60] = 0xF0; this->_memory[61] = 0x80; this->_memory[62] = 0x80; this->_memory[63] = 0x80; this->_memory[64] = 0xF0;
	this->_memory[65] = 0xE0; this->_memory[66] = 0x90; this->_memory[67] = 0x90; this->_memory[68] = 0x90; this->_memory[69] = 0xE0;
	this->_memory[70] = 0xF0; this->_memory[71] = 0x80; this->_memory[72] = 0xF0; this->_memory[73] = 0x80; this->_memory[74] = 0xF0;
	this->_memory[75] = 0xF0; this->_memory[76] = 0x80; this->_memory[77] = 0xF0; this->_memory[78] = 0x80; this->_memory[79] = 0x80;
	//Load renderer plugin
	this->setRenderer(config->getValue("libGFX"));
	this->setInput(config->getValue("libINPUT"));
	this->_nbClocks = config->getUShort("CPU_CLOCKS", 4);
}

Cpu::~Cpu(void)
{
	//this->_deleteInput(this->_input);
	this->_deleteRenderer(this->_renderer);
}

void			Cpu::saveState(void) const
{
	FILE			*state = NULL;
	char			**vram = NULL;
	unsigned char	i;
	std::string		fileName(this->_romName + ".sav");

	state = fopen(fileName.c_str(), "wb");
	if (!state)
	{
		std::cerr << "Error while creating savestate file." << std::endl;
		return;
	}
	//Save RAM
	fwrite(&this->_memory[0], sizeof(unsigned char) * (MEM_SIZE), 1, state);
	//Save CPU
	fwrite(&this->_V[0], sizeof(unsigned char) * (NB_REGISTERS), 1, state);
	fwrite(&this->_I, sizeof(unsigned short), 1, state);
	fwrite(&this->_jump[0], sizeof(unsigned short) * (NB_JUMPS), 1, state);
	fwrite(&this->_nbJumps, sizeof(unsigned char), 1, state);
	fwrite(&this->_gameCounter, sizeof(unsigned char), 1, state);
	fwrite(&this->_soundCounter, sizeof(unsigned char), 1, state);
	fwrite(&this->_pc, sizeof(unsigned short), 1, state);
	fwrite(&this->_mask[0], sizeof(unsigned short) * (NB_OPCODES), 1, state);
	fwrite(&this->_id[0], sizeof(unsigned short) * (NB_OPCODES), 1, state);
	//Save screen
	vram = this->_renderer->getScreen();
	if (vram)
	{
		for (i = 0; i < 64; i++)
			fwrite(vram[i], sizeof(char) * 32, 1, state);
		for (i = 0; i < 64; i++)
			free(vram[i]);
		free(vram);
	}
	else
		std::cerr << "Error while saving screen." << std::endl;
	fclose(state);
	std::cout << "STATE SAVED." << std::endl;
}

void			Cpu::loadState(void)
{
	FILE			*state = NULL;
	char			**vram = NULL;
	unsigned char	i;
	std::string		fileName(this->_romName + ".sav");

	state = fopen(fileName.c_str(), "rb");
	if (!state)
	{
		std::cerr << "Error while opening savestate file." << std::endl;
		return;
	}
	this->_renderer->clearScreen();
	//Load RAM
	fread(&this->_memory[0], sizeof(unsigned char) * (MEM_SIZE), 1, state);
	//Load CPU
	fread(&this->_V[0], sizeof(unsigned char) * (NB_REGISTERS), 1, state);
	fread(&this->_I, sizeof(unsigned short), 1, state);
	fread(&this->_jump[0], sizeof(unsigned short) * (NB_JUMPS), 1, state);
	fread(&this->_nbJumps, sizeof(unsigned char), 1, state);
	fread(&this->_gameCounter, sizeof(unsigned char), 1, state);
	fread(&this->_soundCounter, sizeof(unsigned char), 1, state);
	fread(&this->_pc, sizeof(unsigned short), 1, state);
	fread(&this->_mask[0], sizeof(unsigned short) * (NB_OPCODES), 1, state);
	fread(&this->_id[0], sizeof(unsigned short) * (NB_OPCODES), 1, state);
	//Load screen
	vram = (char **)malloc(sizeof(char *) * 64);
	if (vram)
	{
		for (i = 0; i < 64; i++)
		{
			vram[i] = (char *)malloc(sizeof(char) * 32);
			if (!vram[i])
				std::cerr << "Error while setting a line of screen." << std::endl;
			fread(vram[i], sizeof(char) * 32, 1, state);
		}
		this->_renderer->setScreen(vram);
		for (i = 0; i < 64; i++)
			free(vram[i]);
		free(vram);
	}
	else
		std::cerr << "Error while loading screen." << std::endl;
	std::cout << "STATE LOADED." << std::endl;
	fclose(state);
}

void			Cpu::setRenderer(const std::string &fileName)
{
	if (this->_renderer)
	{
		std::cout << "Unloading previous renderer.. ";
		this->_deleteRenderer(this->_renderer);
		dlclose(this->_rendererHandle);
		std::cout << "done." << std::endl;
	}
	this->_rendererHandle = dlopen(fileName.c_str(), RTLD_LAZY | RTLD_LOCAL);
	if (!this->_rendererHandle)
		dlErrorWrapper();
	this->_RendererCreator = (IRender *(*)(void)) dlsym(this->_rendererHandle, "createRenderer");
	this->_deleteRenderer = (void(*)(IRender *)) dlsym(this->_rendererHandle, "deleteRenderer");
	if (!this->_RendererCreator || !this->_deleteRenderer)
		dlErrorWrapper();
	this->_renderer = this->_RendererCreator();
	std::cout << "Renderer set to " << fileName << std::endl;
}

void			Cpu::setInput(const std::string &fileName)
{
	if (this->_input)
	{
		std::cout << "Unloading previous input.. ";
		this->_deleteInput(this->_input);
		dlclose(this->_inputHandle);
		std::cout << "done." << std::endl;
	}
	this->_inputHandle = dlopen(fileName.c_str(), RTLD_LAZY | RTLD_LOCAL);
	if (!this->_inputHandle)
		dlErrorWrapper();
	this->_InputCreator = (Iinput *(*)(void)) dlsym(this->_inputHandle, "createInput");
	this->_deleteInput = (void(*)(Iinput *)) dlsym(this->_inputHandle, "deleteInput");
	if (!this->_InputCreator || !this->_deleteInput)
		dlErrorWrapper();
	this->_input = this->_InputCreator();
	std::cout << "Input set to " << fileName << std::endl;
}

void			Cpu::uncount(void)
{
	if (this->_gameCounter > 0)
		this->_gameCounter--;
	if (this->_soundCounter > 0)
		this->_soundCounter--;
}

unsigned short	Cpu::getOpcode(void) const
{
	return (this->_memory[this->_pc] << 8) + this->_memory[this->_pc + 1];
}

unsigned char	Cpu::getAction(unsigned short opcode) const
{
	unsigned char	action;

	for (action = 0; action < NB_OPCODES; action++)
		if ((this->_mask[action] & opcode) == this->_id[action])
			break;
	return action;
}

void			Cpu::execOpcode(unsigned short opcode)
{
	unsigned char	b4, b3, b2, b1;

	b3 = (opcode&(0x0F00)) >> 8; //on prend les 4 bits représentant X
	b2 = (opcode&(0x00F0)) >> 4; //idem pour Y
	b1 = (opcode&(0x000F));
	b4 = this->getAction(opcode);
	(this->*_opcodes_ptrs[b4])(b1, b2, b3);
	this->_pc += 2;
	if (this->_soundCounter != 0)
	{
		std::cout << "\a" << std::endl;
		this->_soundCounter = 0;
	}
}

void			Cpu::render(unsigned char b1, unsigned char b2, unsigned char b3)
{
	unsigned char	x = 0, y = 0, k = 0, codage = 0, j = 0, decalage = 0;

	this->_V[0xF] = 0;
	for (k = 0; k < b1; k++)
	{
		codage = this->_memory[this->_I + k];//on récupère le codage de la ligne à dessiner
		y = (this->_V[b2] + k) % 32;//on calcule l'ordonnée de la ligne à dessiner, on ne doit pas dépasser L
		for (j = 0, decalage = 7; j < 8; j++, decalage--)
		{
			x = (this->_V[b3] + j) % 64; //on calcule l'abscisse, on ne doit pas dépasser l
			if (((codage) & (0x1 << decalage)) != 0)//on récupère le bit correspondant
			{
				if (this->_renderer->getPixelColor(x, y) == WHITE)//le pixel était blanc
				{
					this->_renderer->setPixel(x, y, BLACK);
					this->_V[0xF] = 1; //il y a donc collusion
				}
				else
					this->_renderer->setPixel(x, y, WHITE);
			}
		}
	}
}

bool			Cpu::loadROM(const std::string &name)
{
	FILE	*rom = NULL;

	rom = fopen(name.c_str(), "rb");
	if (!rom)
	{
		std::cerr << "Error while opening ROM file." << std::endl;
		return false;
	}
	this->_romName = name;
	fread(&this->_memory[BASE_ADD], sizeof(unsigned char) * (MEM_SIZE - BASE_ADD), 1, rom);
	fclose(rom);
	return true;
}

void			Cpu::render(void) const
{
	if (this->_renderer)
		this->_renderer->updateScreen();
}

unsigned short	Cpu::getNbClocks(void) const
{
	return this->_nbClocks;
}

void			Cpu::setNbClocks(unsigned short nbClocks)
{
	this->_nbClocks = nbClocks;
}

Iinput			*Cpu::getInput(void) const
{
	return this->_input;
}

/*============================================================================*/
/*===========================  >>>OPCODES EXEC<<<  ===========================*/
/*============================================================================*/
void			Cpu::_0NNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	std::cerr << "Instruction 0NNN called: not implemented." << std::endl;
}

void			Cpu::_00E0(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_renderer->clearScreen();
}

void			Cpu::_00EE(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (this->_nbJumps > 0)
	{
		this->_nbJumps--;
		this->_pc = this->_jump[this->_nbJumps];
	}
}

void			Cpu::_1NNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	//on prend le nombre NNN (pour le saut)
	this->_pc = (b3 << 8) + (b2 << 4) + b1 - 2;
}

void			Cpu::_2NNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_jump[this->_nbJumps] = this->_pc; //on reste là où on était
	if (this->_nbJumps < 15)
		this->_nbJumps++;
	//sinon, on a effectué trop de sauts
	this->_pc = (b3 << 8) + (b2 << 4) + b1 - 2;
}

void			Cpu::_3XNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (this->_V[b3] == ((b2 << 4) + b1))
		this->_pc += 2;
}

void			Cpu::_4XNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (this->_V[b3] != ((b2 << 4) + b1))
		this->_pc += 2;
}

void			Cpu::_5XY0(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (this->_V[b3] == this->_V[b2])
		this->_pc += 2;
}

void			Cpu::_6XNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = (b2 << 4) + b1;
}

void			Cpu::_7XNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] += (b2 << 4) + b1;
}

void			Cpu::_8XY0(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = this->_V[b2];
}

void			Cpu::_8XY1(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = this->_V[b3] | this->_V[b2];
}

void			Cpu::_8XY2(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = this->_V[b3] & this->_V[b2];
}

void			Cpu::_8XY3(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = this->_V[b3] ^ this->_V[b2];
}

void			Cpu::_8XY4(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if ((this->_V[b3] + this->_V[b2]) > 0xFF)
		this->_V[0xF] = 1;
	else
		this->_V[0xF] = 0;
	this->_V[b3] += this->_V[b2];
}

void			Cpu::_8XY5(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if ((this->_V[b3] < this->_V[b2]))
		this->_V[0xF] = 0;
	else
		this->_V[0xF] = 1;
	this->_V[b3] -= this->_V[b2];
}

void			Cpu::_8XY6(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[0xF] = (this->_V[b3] & (0x01));
	this->_V[b3] = (this->_V[b3] >> 1);
}

void			Cpu::_8XY7(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if ((this->_V[b2] < this->_V[b3]))
		this->_V[0xF] = 0;
	else
		this->_V[0xF] = 1;
	this->_V[b3] = this->_V[b2] - this->_V[b3];
}

void			Cpu::_8XYE(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[0xF] = (this->_V[b3] >> 7);
	this->_V[b3] = (this->_V[b3] << 1);
}

void			Cpu::_9XY0(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (this->_V[b3] != this->_V[b2])
		this->_pc += 2;
}

void			Cpu::_ANNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_I = (b3 << 8) + (b2 << 4) + b1;
}

void			Cpu::_BNNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_I = (b3 << 8) + (b2 << 4) + b1 + this->_V[0];
	this->_pc -= 2;
}

void			Cpu::_CXNN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = (rand())%((b2 << 4) + b1 + 1);
}

void			Cpu::_DXYN(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->render(b1, b2, b3);
}

void			Cpu::_EX9E(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (this->_input->keys[this->_V[b3]])
		this->_pc += 2;
}

void			Cpu::_EXA1(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if (!this->_input->keys[this->_V[b3]])
		this->_pc += 2;
}

void			Cpu::_FX07(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_V[b3] = this->_gameCounter;
}

void			Cpu::_FX0A(unsigned char b1, unsigned char b2, unsigned char b3)
{
	unsigned char	i;

	std::cout << "Waiting KEY" << std::endl;
	this->active = false;
	if (!this->_input->newKey)
		return;
	this->_input->newKey = false;
	for (i = 0; i < 16; i++)
		if (this->_input->keys[i])
			this->_V[b3] = i;
	this->active = true;
}

void			Cpu::_FX15(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_gameCounter = this->_V[b3];
}

void			Cpu::_FX18(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_soundCounter = this->_V[b3];
}

void			Cpu::_FX1E(unsigned char b1, unsigned char b2, unsigned char b3)
{
	if ((this->_I + this->_V[b3]) > 0xFFF)
		this->_V[0xF] = 1;
	else
		this->_V[0xF] = 0;
	this->_I += this->_V[b3];
}

void			Cpu::_FX29(unsigned char b1, unsigned char b2, unsigned char b3)
{
	this->_I = 5 * this->_V[b3];
}

void			Cpu::_FX33(unsigned char b1, unsigned char b2, unsigned char b3)
{
	//Writing X00
	this->_memory[this->_I] = (this->_V[b3] - this->_V[b3] % 100) / 100;
	//Writing 0X0
	this->_memory[this->_I + 1] = (((this->_V[b3] - this->_V[b3] % 10) / 10) % 10);
	//Writing 00X
	this->_memory[this->_I + 2] = this->_V[b3] - this->_memory[this->_I] * 100 - this->_memory[this->_I + 1] * 10;
}

void			Cpu::_FX55(unsigned char b1, unsigned char b2, unsigned char b3)
{
	unsigned char	i;

	for (i = 0; i <= b3; i++)
		this->_memory[this->_I + i] = this->_V[i];
}

void			Cpu::_FX65(unsigned char b1, unsigned char b2, unsigned char b3)
{
	unsigned char	i;

	for (i = 0; i <= b3; i++)
		this->_V[i] = this->_memory[this->_I + i];
}
