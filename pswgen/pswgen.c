#include "../mttopt/include/mttopt.h"
#include "../mttstr/include/mttstr.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct mttopt_opt_t optv[] = {
		{ 'u', OPT_FS_MUST_HAVE_ARG, 0 },
		{ 'l', OPT_FS_MUST_HAVE_ARG, 0 },
		{ 'n', OPT_FS_MUST_HAVE_ARG, 0 },
		{ 's', OPT_FS_MUST_HAVE_ARG, 0 }
	};

	struct mttstr_fmt_t opt = { '+', 0, ' ', 10, FMT_FS_LEFT_FILL };

	mttopt_extr_optv(argc, argv, 4, optv);

	size_t ucase = optv[0].found ? mttstr_fstr_to_ival(optv[0].arg, NULL, opt) : 4;
	size_t lcase = optv[1].found ? mttstr_fstr_to_ival(optv[1].arg, NULL, opt) : 4;
	size_t num = optv[2].found ? mttstr_fstr_to_ival(optv[2].arg, NULL, opt) : 4;
	size_t sym = optv[3].found ? mttstr_fstr_to_ival(optv[3].arg, NULL, opt) : 4;
	size_t pswsize = ucase + lcase + num + sym;
	char *psw = malloc(pswsize + 1);

	if (psw == NULL) return 1;

	char *p = psw, *ps = p + pswsize;
	char syms[] = { ' ', '_', '-', ',', ';', ':', '!', '?', '.', '\'', '"', '(', ')', '[', ']', '{', '}', '@', '*', '/', '\\', '&', '#', '%', '`', '^', '+', '<', '=', '>', '|', '~', '$' };

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

	return 0;
}