#ifndef MTT_CMD_LINE_H
#define MTT_CMD_LINE_H

#include <stdexcept>

namespace mtt
{
	class opt_t
	{
	public:
		static constexpr int NO_ARG = 0x00;
		static constexpr int OPTIONAL_ARG = 0x01;
		static constexpr int REQUIRED_ARG = 0x02;

	private:
		char alias;
	
	public:
		int fs;
		const char *arg;
	
		opt_t(char alias, int fs, const char *arg = nullptr)
			: alias(alias ? alias : throw std::invalid_argument("alias")), fs(fs), arg(arg)
		{

		}

		char get_alias() const noexcept
		{
			return alias;
		}

		bool set_alias(char alias) noexcept
		{
			if (alias == '\0') return false;

			this->alias = alias;

			return true;
		}
	};

	int extr_optv(int argc, char *argv[], int optc, opt_t *optv) noexcept;
}

#endif