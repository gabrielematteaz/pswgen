#include "../mttlib/mttopt/mttopt.h"
#include "../mttlib/mttstr/mttstr.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct mttopt_opt_t optv[] = {
		{ 'u', OPT_FLAGS_MUST_HAVE_ARG, 0 },
		{ 'l', OPT_FLAGS_MUST_HAVE_ARG, 0 },
		{ 'n', OPT_FLAGS_MUST_HAVE_ARG, 0 },
		{ 's', OPT_FLAGS_MUST_HAVE_ARG, 0 }
	};

	struct mttstr_fmt_t optfmt = { 10, '+', '-', ' ', FMT_FLAGS_LEFT_FILL };
	size_t ucase, lcase, num, sym, pswsize;
	char *psw, *p, *ps, syms[] = { ' ', '_', '-', ',', ';', ':', '!', '?', '.', '\'', '"', '(', ')', '[', ']', '{', '}', '@', '*', '/', '\\', '&', '#', '%', '`', '^', '+', '<', '=', '>', '|', '~', '$' };

	mttopt_extr_optv(argc, argv, 4, optv);
	ucase = optv[0].found ? mttstr_fstr_to_ival(optv[0].arg, NULL, optfmt) : 4;
	lcase = optv[1].found ? mttstr_fstr_to_ival(optv[1].arg, NULL, optfmt) : 4;
	num = optv[2].found ? mttstr_fstr_to_ival(optv[2].arg, NULL, optfmt) : 4;
	sym = optv[3].found ? mttstr_fstr_to_ival(optv[3].arg, NULL, optfmt) : 4;
	pswsize = ucase + lcase + num + sym;
	psw = malloc(pswsize + 1);

	if (psw)
	{
		p = psw;
		ps = p + pswsize;
		srand(time(NULL));

		while (1)
		{
			switch (rand() % 4)
			{
			case 0:
				if (ucase)
				{
					ucase--;
					*p = 'A' + rand() % 26;
					p++;
				}

				break;
			case 1:
				if (lcase)
				{
					lcase--;
					*p = 'a' + rand() % 26;
					p++;
				}

				break;
			case 2:
				if (num)
				{
					num--;
					*p = '0' + rand() % 10;
					p++;
				}

				break;
			case 3:
				if (sym)
				{
					sym--;
					*p = syms[rand() % sizeof(syms)];
					p++;
				}

				break;
			}

			if (p == ps) break;
		}

		*p = 0;
		puts(psw);
		free(psw);
	}

	return 0;
}