#include <random>
#include <string>
#include <iostream>
#include "../mttlib/mttopt/mttopt.h"
#include "../mttlib/mttstr/mttstr.hpp"

class pswgen_t
{
	static constexpr char sym[] = "~`! @#$%^&*()_-+={[}]|\\:;\"'<,>.?/";

public:
	std::size_t ucasemin = 4, lcasemin = 4, digmin = 4, symmin = 4;

	pswgen_t()
	{

	}

	pswgen_t(std::size_t ucasemin, std::size_t lcasemin, std::size_t digmin, std::size_t symmin)
		: ucasemin(ucasemin), lcasemin(lcasemin), digmin(digmin), symmin(symmin)
	{

	}

	std::string generate() const
	{
		std::random_device rdev;
		std::mt19937_64 mtrng(rdev());
		std::uniform_int_distribution<std::size_t> csdis(0, 3);
		std::uniform_int_distribution<std::size_t> ulcasedis(0, 25);
		std::uniform_int_distribution<std::size_t> digdis(0, 9);
		std::uniform_int_distribution<std::size_t> symdis(0, sizeof(sym) - 1);
		std::size_t ucasec = ucasemin, lcasec = lcasemin, digc = digmin, symc = symmin;
		std::string psw(ucasemin + lcasemin + digmin + symmin + 1, 0);
		char *pswc = psw.data();

		while (1)
		{
			bool exit = true;
			std::size_t cstype = csdis(mtrng);

			if (ucasec)
			{
				if (cstype == 0)
				{
					ucasec--;
					*pswc = 'A' + ulcasedis(mtrng);
					pswc++;

					continue;
				}

				exit = false;
			}
			
			if (lcasec)
			{
				if (cstype == 1)
				{
					lcasec--;
					*pswc = 'a' + ulcasedis(mtrng);
					pswc++;

					continue;
				}

				exit = false;
			}
			
			if (digc)
			{
				if (cstype == 2)
				{
					digc--;
					*pswc = '0' + digdis(mtrng);
					pswc++;

					continue;
				}

				exit = false;
			}
			
			if (symc)
			{
				if (cstype == 3)
				{
					symc--;
					*pswc = sym[symdis(mtrng)];
					pswc++;
				}

				exit = false;
			}

			if (exit) break;
		}

		return psw;
	}
};

int main(int argc, char *argv[])
{
	mttopt_opt_t optv[] = {
		{ 'u', IGNORE_COPIES, MUST_HAVE_ARG, NOT_FOUND },
		{ 'l', IGNORE_COPIES, MUST_HAVE_ARG, NOT_FOUND },
		{ 'd', IGNORE_COPIES, MUST_HAVE_ARG, NOT_FOUND },
		{ 's', IGNORE_COPIES, MUST_HAVE_ARG, NOT_FOUND }
	};

	mttstr_Converter optfmt;

	mttopt_extract_optv(argc, argv, sizeof(optv) / sizeof(*optv), optv);

	pswgen_t pswgen(optv[0].status == FOUND ? optfmt.StringToValue(optv[0].arg, nullptr) : 4, optv[1].status == FOUND ? optfmt.StringToValue(optv[1].arg, nullptr) : 4, optv[2].status == FOUND ? optfmt.StringToValue(optv[2].arg, nullptr) : 4, optv[3].status == FOUND ? optfmt.StringToValue(optv[3].arg, nullptr) : 4);

	std::cout << pswgen.generate() << std::endl;

	return 0;
}