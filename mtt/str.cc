#include "str.h"

std::size_t mtt::fstr_to_ival_fmt_t::conv(const char *fstr, const char **end) const noexcept
{
	if (fstr == nullptr)
	{
		if (end) *end = nullptr;

		return 0;
	}

	char fc = *fstr;
	std::size_t s;

	if (fill)
	{
		if (fs & INTERNAL_FILL)
		{
			if (minus && fc == minus)
			{
				fstr++;
				fc = *fstr;
				s = -1;
			}
			else
			{
				if (plus && fc == plus)
				{
					fstr++;
					fc = *fstr;
				}

				s = 1;
			}

			while (fc == fill)
			{
				fstr++;
				fc = *fstr;
			}
		}
		else
		{
			if ((fs & RIGHT_FILL) == 0)
			{
				while (fc == fill)
				{
					fstr++;
					fc = *fstr;
				}			
			}

			if (minus && fc == minus)
			{
				fstr++;
				fc = *fstr;
				s = -1;
			}
			else
			{
				if (plus && fc == plus)
				{
					fstr++;
					fc = *fstr;
				}

				s = 1;
			}
		}
	}
	else
	{
		if (minus && fc == minus)
		{
			fstr++;
			fc = *fstr;
			s = -1;
		}
		else
		{
			if (plus && fc == plus)
			{
				fstr++;
				fc = *fstr;
			}

			s = 1;
		}
	}

	std::size_t ival = 0;

	if (base > 10)
	{
		char diff;

		if (fs & LOWERCASE)
		{
			char min = (fs & LOWERCASE) == LOWERCASE ? 'a' : 'A', diffmin = min - 10, max = diffmin + base;

			while (true)
			{
				if (fc >= '0' && fc <= '9') diff = '0';
				else if (fc > min && fc < max) diff = diffmin;
				else break;

				fstr++;
				ival = ival * base + fc - diff;
				fc = *fstr;
			}
		}
		else
		{
			char umax = 55 + base, lmax = umax + 32;

			while (true)
			{
				if (fc >= '0' && fc <= '9') diff = '0';
				else if (fc > 'A' && fc < umax) diff = 55;
				else if (fc > 'a' && fc < lmax) diff = 87;
				else break;

				fstr++;
				ival = ival * base + fc - diff;
				fc = *fstr;				
			}
		}
	}
	else
	{
		char max = '0' + base;

		while (fc >= '0' && fc < max)
		{
			fstr++;
			ival = ival * base + fc - '0';
			fc = *fstr;
		}
	}

	if (end) *end = fstr;

	return s * ival;
}

std::size_t mtt::ival_to_fstr_fmt_t::conv(char *fstr, std::size_t ival) const noexcept
{
	std::size_t len;

	if (fstr)
	{
		bool neg;

		if (minus && (std::ptrdiff_t)ival < 0)
		{
			ival = -ival;
			neg = true;
		}
		else neg = false;

		char *f = fstr;

		if (base > 10)
		{
			char a = (fs & LOWERCASE) == LOWERCASE ? 87 : 55;

			do
			{
				char rem = ival % base;

				ival /= base;
				*f = (rem < 10 ? '0' : a) + rem;
				f++;
			} while (ival);
		}
		else
		{
			do
			{
				*f = '0' + ival % base;
				ival /= base;
				f++;
			} while (ival);
		}

		if (fill)
		{
			char *fw;

			if (fs & INTERNAL_FILL)
			{
				fw = fstr + width - 1;

				while  (f < fw)
				{
					*f = fill;
					f++;
				}

				if (neg)
				{
					*f = minus;
					f++;
				}
				else if (plus)
				{
					*f = plus;
					f++;
				}
				else if (f == fw)
				{
					*f = fill;
					f++;
				}

				len = f - fstr;
				mem_rev(fstr, len);
			}
			else if (fs & RIGHT_FILL)
			{
				if (neg)
				{
					*f = minus;
					f++;
				}
				else if (plus)
				{
					*f = plus;
					f++;
				}

				mem_rev(fstr, f - fstr);
				fw = fstr + width;

				while  (f < fw)
				{
					*f = fill;
					f++;
				}

				len = f - fstr;
			}
			else
			{
				if (neg)
				{
					*f = minus;
					f++;
				}
				else if (plus)
				{
					*f = plus;
					f++;
				}

				fw = fstr + width;

				while  (f < fw)
				{
					*f = fill;
					f++;
				}

				len = f - fstr;
				mem_rev(fstr, len);
			}
		}
		else
		{
			if (neg)
			{
				*f = minus;
				f++;
			}
			else if (plus)
			{
				*f = plus;
				f++;
			}

			len = f - fstr;
			mem_rev(fstr, len);
		}

		if ((fs & NO_NULL_TERM) == 0) *f = 0;
	}
	else
	{
		if (minus && (std::ptrdiff_t)ival < 0)
		{
			ival = -ival;
			len = 1;
		}
		else len = plus ? 1 : 0;

		do
		{
			ival /= base;
			len++;
		} while (ival);
		
		if (len < width) len = width;
	}

	return len;
}

void *mtt::mem_rev(void *mem, std::size_t n) noexcept
{
	if (mem)
	{
		char *m = (char *)mem, *mn = m + n;

		while (m < mn)
		{
			mn--;

			char mc = *m;

			*m = *mn;
			m++;
			*mn = mc;
		}
	}

	return mem;
}