#include "mtt/cmd_line.h"
#include "mtt/str.h"
#include <Windows.h>
#include <bcrypt.h>

typedef NTSTATUS (*rngproc_t)(BCRYPT_ALG_HANDLE, PUCHAR, ULONG, ULONG);

int main(int argc, char *argv[])
{
	struct mtt_opt_t optv[] = {
		{ 'u', OPT_FS_REQUIRED_ARG, NULL },
		{ 'l', OPT_FS_REQUIRED_ARG, NULL },
		{ 'd', OPT_FS_REQUIRED_ARG, NULL },
		{ 's', OPT_FS_REQUIRED_ARG, NULL },
		{ 'c', OPT_FS_NO_ARG, NULL }
	};

	mtt_extr_optv(argc, argv, 5, optv);

	struct mtt_fmt_t optfmt = { '+', 0, ' ', 10, FMT_FS_LEFT_FILL, 0 };
	size_t ucasecount = optv[0].arg ? mtt_fstr_to_ival(optv[0].arg, NULL, optfmt) : 5, lcasecount = optv[1].arg ? mtt_fstr_to_ival(optv[1].arg, NULL, optfmt) : 5;
	size_t dgtcount = optv[2].arg ? mtt_fstr_to_ival(optv[2].arg, NULL, optfmt) : 5;
	size_t symcount = optv[3].arg ? mtt_fstr_to_ival(optv[3].arg, NULL, optfmt) : 5;
	size_t pswsize = ucasecount + lcasecount + dgtcount + symcount;
	HGLOBAL glmem = GlobalAlloc(GMEM_MOVEABLE, pswsize + 1);

	if (glmem == NULL) return GetLastError();

	HMODULE bcrypt = LoadLibraryA("bcrypt.dll");

	if (bcrypt == NULL)
	{
		DWORD error = GetLastError();

		GlobalFree(glmem);

		return error;
	}

	char *psw = GlobalLock(glmem), *p = psw, *ps = p + pswsize, syms[] = { ' ', '_', '-', ',', ';', ':', '!', '?', '.', '\'', '"', '(', ')', '[', ']', '{', '}', '@', '*', '/', '\\', '&', '#', '%', '`', '^', '+', '<', '=', '>', '|', '~', '$' };
	rngproc_t rngproc = (rngproc_t)GetProcAddress(bcrypt, "BCryptGenRandom");

	while (p < ps)
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
				*p = 'A' + num % 26;
				p++;
			}
		}
		else if (num == 1)
		{
			if (lcasecount)
			{
				lcasecount--;
				rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
				*p = 'a' + num % 26;
				p++;
			}
		}
		else if (num == 2)
		{
			if (dgtcount)
			{
				dgtcount--;
				rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
				*p = '0' + num % 26;
				p++;				
			}
		}
		else if (num == 3)
		{
			if (symcount)
			{
				symcount++;
				rngproc(NULL, (PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
				*p = syms[num % sizeof(syms)];
				p++;
			}
		}
	}

	FreeLibrary(bcrypt);
	*p = 0;

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	if (optv[4].arg)
	{
		BOOL res = OpenClipboard(GetConsoleWindow());

		GlobalUnlock(glmem);

		if (res)
		{
			EmptyClipboard();
			SetClipboardData(CF_TEXT, psw);
			CloseClipboard();
			WriteConsoleA(out, "Password copied to clipboard", 28, NULL, NULL);
		}
	}
	else
	{
		WriteConsoleA(out, "Password: ", 10, NULL, NULL);
		WriteConsoleA(out, psw, pswsize, NULL, NULL);
		GlobalUnlock(glmem);
	}

	GlobalFree(glmem);

	return 0;
}