#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <map>

class	Config
{
	public:
		Config(const std::string &cfgFile);
		Config(void);
		~Config(void);
		std::string								getValue(const std::string &name);
		void									setUShort(const std::string &name, unsigned short value);
		unsigned short							getUShort(const std::string &name, unsigned short defValue);
		std::string								clearString(std::string str) const;
		void									save(void) const;

	private:
		void									_loadFile(const std::string &cfgFile);
		std::map<std::string, std::string>		_data;
};

#endif
