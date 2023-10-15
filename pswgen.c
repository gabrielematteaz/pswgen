#include "mtt/cmdline.h"
#include "mtt/fmt.h"
#include <Windows.h>
#include <bcrypt.h>

typedef NTSTATUS (*rngproc_t)(BCRYPT_ALG_HANDLE, PUCHAR, ULONG, ULONG);

int main(int argc, char *argv[])
{
	struct mtt_opt_t optv[] = {
		{ 'u', OPT_ARG_TYPE_REQUIRED, NULL },
		{ 'l', OPT_ARG_TYPE_REQUIRED, NULL },
		{ 'd', OPT_ARG_TYPE_REQUIRED, NULL },
		{ 's', OPT_ARG_TYPE_REQUIRED, NULL },
		{ 'c', OPT_ARG_TYPE_NONE, NULL }
	};

	struct mtt_ival_fmt_t optfmt = { '+', 0, ' ', 10, FMT_FILLING_LEFT, 0 };
	size_t ucasecount, lcasecount, dgtcount, symcount, pswsize;
	HGLOBAL glmem;
	int res;

	mtt_extr_optv(argc, argv, sizeof(optv) / sizeof(optv[0]), optv);
	ucasecount = optv[0].arg == NULL ? 5 : mtt_fstr_to_ival(optv[0].arg, NULL, optfmt);
	lcasecount = optv[1].arg == NULL ? 5 : mtt_fstr_to_ival(optv[1].arg, NULL, optfmt);
	dgtcount = optv[2].arg == NULL ? 5 : mtt_fstr_to_ival(optv[2].arg, NULL, optfmt);
	symcount = optv[3].arg == NULL ? 5 : mtt_fstr_to_ival(optv[3].arg, NULL, optfmt);
	pswsize = ucasecount + lcasecount + dgtcount + symcount;
	glmem = GlobalAlloc(GMEM_MOVEABLE, pswsize + 1);

	if (glmem == NULL)
	{	
		res = 1;
	}
	else
	{
		HMODULE bcrypt = LoadLibraryA("bcrypt.dll");

		if (bcrypt == NULL)
		{
			res = 2;
		}
		else
		{
			int i = 0;
			rngproc_t rngproc = (rngproc_t)GetProcAddress(bcrypt, "BCryptGenRandom");
			char *psw = GlobalLock(glmem), syms[] = { ' ', '_', '-', ',', ';', ':', '!', '?', '.', '\'', '"', '(', ')', '[', ']', '{', '}', '@', '*', '/', '\\', '&', '#', '%', '`', '^', '+', '<', '=', '>', '|', '~', '$' };
			HANDLE out;

			while (i < pswsize)
			{
				size_t num;

				rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
				num = num % 4;

				if (num == 0)
				{
					if (ucasecount)
					{
						ucasecount--;
						rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
						psw[i] = 'A' + num % 26;
						i++;
					}
				}
				else if (num == 1)
				{
					if (lcasecount)
					{
						lcasecount--;
						rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
						psw[i] = 'a' + num % 26;
						i++;
					}
				}
				else if (num == 2)
				{
					if (dgtcount)
					{
						dgtcount--;
						rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
						psw[i] = '0' + num % 26;
						i++;				
					}
				}
				else if (num == 3)
				{
					if (symcount)
					{
						symcount++;
						rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
						psw[i] = syms[num % sizeof(syms)];
						i++;
					}
				}
			}

			FreeLibrary(bcrypt);
			psw[i] = '\0';
			out = GetStdHandle(STD_OUTPUT_HANDLE);

			if (optv[4].arg == NULL)
			{
				const char str[] = "Password: ";

				WriteConsoleA(out, str, strlen(str), NULL, NULL);
				WriteConsoleA(out, psw, pswsize, NULL, NULL);
				GlobalUnlock(glmem);
			}
			else
			{
				BOOL res = OpenClipboard(GetConsoleWindow());

				GlobalUnlock(glmem);

				if (res)
				{
					const char str[] = "Password copied to clipboard";

					EmptyClipboard();
					SetClipboardData(CF_TEXT, psw);
					CloseClipboard();
					WriteConsoleA(out, str, strlen(str), NULL, NULL);
				}
			}

			res = 0;
		}

		GlobalFree(glmem);
	}

	return res;
}