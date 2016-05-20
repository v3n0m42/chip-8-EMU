#include <iostream>
#include <SFML/Graphics.hpp>
#include "Cpu.hpp"
#include "Keyboard.hpp"
#include "Config.hpp"

int		main(int ac, char **av)
{
	Config				config;
	Cpu					cpu(&config);
	Iinput				*input = cpu.getInput();
	unsigned short		cpuClocksPerSecond = cpu.getNbClocks();

	if (ac == 1)
	{
		std::cerr << "Usage: <chi8EMU> romfile" << std::endl;
		delete input;
		return 42;
	}
	if (!cpu.loadROM(av[1]))
	{
		delete input;
		return 42;
	}
	while (!input->escape)
	{
		input->updateInput();
		if (cpu.active)
		{
			//Change emulation speed
			if (input->speedup)
			{
				cpuClocksPerSecond++;
				cpu.setNbClocks(cpuClocksPerSecond);
				config.setUShort("CPU_CLOCKS", cpuClocksPerSecond);
				std::cout << "SPEED: " << cpuClocksPerSecond << " clocks/sec." << std::endl;
			}
			else if (input->slowdown)
			{
				cpuClocksPerSecond--;
				cpu.setNbClocks(cpuClocksPerSecond);
				config.setUShort("CPU_CLOCKS", cpuClocksPerSecond);
				std::cout << "SPEED: " << cpuClocksPerSecond << " clocks/sec." << std::endl;
			}
			//Save/Load state
			if (input->savestate)
				cpu.saveState();
			if (input->loadstate)
				cpu.loadState();
			//Save config
			if (input->saveConfig)
				config.save();
			cpu.execOpcode(cpu.getOpcode());
			cpu.render();
			cpu.uncount();
		}
		sf::sleep(sf::milliseconds(1000 / cpuClocksPerSecond));
	}
	delete input;
	return 0;
}
