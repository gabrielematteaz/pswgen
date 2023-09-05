#include "mtt/cmd_line.h"
#include "mtt/str.h"
#include <Windows.h>
#include <ctime>
#include <iostream>

int main(int argc, char *argv[])
{
	std::srand(std::time(nullptr));

	mtt::opt_t optv[] = {
		{ 'u', mtt::opt_t::REQUIRED_ARG },
		{ 'l', mtt::opt_t::REQUIRED_ARG },
		{ 'd', mtt::opt_t::REQUIRED_ARG },
		{ 's', mtt::opt_t::REQUIRED_ARG },
		{ 'c', mtt::opt_t::NO_ARG }
	};

	mtt::extr_optv(argc, argv, 5, optv);

	mtt::fstr_to_ival_fmt_t opt('+', 0, ' ', 10, opt.LEFT_FILL);
	std::size_t u = optv[0].arg ? opt.conv(optv[0].arg) : 5;
	std::size_t l = optv[1].arg ? opt.conv(optv[1].arg) : 5;
	std::size_t d = optv[2].arg ? opt.conv(optv[2].arg) : 5;
	std::size_t s = optv[3].arg ? opt.conv(optv[3].arg) : 5;
	std::size_t size = u + l + d + s;
	HGLOBAL pswmem = GlobalAlloc(GMEM_MOVEABLE, size + 1);

	if (pswmem == NULL) return GetLastError();

	char *psw = (char *)GlobalLock(pswmem);
	char *p = psw, *ps = p + size;
	char syms[] = { ' ', '_', '-', ',', ';', ':', '!', '?', '.', '\'', '"', '(', ')', '[', ']', '{', '}', '@', '*', '/', '\\', '&', '#', '%', '`', '^', '+', '<', '=', '>', '|', '~', '$' };

	while (p < ps)
	{
		std::size_t cs = std::rand() % 4;

		if (cs == 0)
		{
			if (u)
			{
				*p = 'A' + std::rand() % 26;
				p++;
			}
		}
		else if (cs == 1)
		{
			if (l)
			{
				*p = 'a' + std::rand() % 26;
				p++;
			}
		}
		else if (cs == 2)
		{
			if (d)
			{
				*p = '0' + std::rand() % 9;
				p++;
			}
		}
		else if (cs == 3)
		{
			if (s)
			{
				*p = syms[std::rand() % sizeof(syms)];
				p++;
			}
		}
	}

	*p = 0;
	GlobalUnlock(pswmem);

	if (optv[4].arg)
	{
		OpenClipboard(GetConsoleWindow());
		EmptyClipboard();
		SetClipboardData(CF_TEXT, pswmem);
		CloseClipboard();
	}
	else std::cout << psw << std::endl;

	GlobalFree(pswmem);

	return 0;
}