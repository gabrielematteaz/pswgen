#include "str.h"

void *mtt_mem_rev(void *mem, size_t n)
{
	if (mem)
	{
		char *m = mem, *mn = m + n, mc;

		while (m < mn)
		{
			mn--;
			mc = *m;
			*m = *mn;
			m++;
			*mn = mc;
		}
	}

	return mem;
}

size_t mtt_fstr_to_ival(const char *fstr, const char **end, struct mtt_fmt_t fmt)
{
	if (fstr == NULL || fmt.base < 2 || fmt.base > 36)
	{
		if (end) *end = NULL;

		return 0;
	}

	char fc = *fstr;
	size_t sign;

	if (fmt.fill)
	{
		if (fmt.fs & FMT_FS_INTERNAL_FILL)
		{
			if (fmt.minus && fc == fmt.minus)
			{
				fstr++;
				fc = *fstr;
				sign = -1;
			}
			else
			{
				if (fmt.plus && fc == fmt.plus)
				{
					fstr++;
					fc = *fstr;
				}

				sign = 1;
			}

			while (fc == fmt.fill)
			{
				fstr++;
				fc = *fstr;
			}
		}
		else
		{
			if ((fmt.fs & FMT_FS_RIGHT_FILL) == 0)
			{
				while (fc == fmt.fill)
				{
					fstr++;
					fc = *fstr;
				}
			}

			if (fmt.minus && fc == fmt.minus)
			{
				fstr++;
				fc = *fstr;
				sign = -1;
			}
			else
			{
				if (fmt.plus && fc == fmt.plus)
				{
					fstr++;
					fc = *fstr;
				}

				sign = 1;
			}
		}
	}
	else
	{
		if (fmt.minus && fc == fmt.minus)
		{
			fstr++;
			fc = *fstr;
			sign = -1;
		}
		else
		{
			if (fmt.plus && fc == fmt.plus)
			{
				fstr++;
				fc = *fstr;
			}

			sign = 1;
		}
	}

	size_t ival = 0;

	if (fmt.base > 10)
	{
		char ltrcase = fmt.fs & FMT_FS_LOWERCASE, diff;

		if (ltrcase)
		{
			char min = ltrcase == FMT_FS_LOWERCASE ? 'a' : 'A', diffmin = min - 10, max = diffmin + fmt.base;

			while (1)
			{
				if (fc >= '0' && fc <= '9') diff = '0';
				else if (fc >= min && fc < max) diff = diffmin;
				else break;

				fstr++;
				ival = ival * fmt.base + fc - diff;
				fc = *fstr;
			}
		}
		else
		{
			char umax = 55 + fmt.base, lmax = umax + 32;

			while (1)
			{
				if (fc >= '0' && fc <= '9') diff = '0';
				else if (fc >= 'A' && fc < umax) diff = 55;
				else if (fc >= 'a' && fc < lmax) diff = 87;
				else break;

				fstr++;
				ival = ival * fmt.base + fc - diff;
				fc = *fstr;
			}
		}
	}
	else
	{
		char max = '0' + fmt.base;

		while (fc >= '0' && fc < max)
		{
			fstr++;
			ival = ival * fmt.base + fc - '0';
			fc = *fstr;
		}
	}

	if (end) *end = fstr;

	return sign * ival;
}

size_t mtt_ival_to_fstr(char *fstr, size_t ival, struct mtt_fmt_t fmt)
{
	if (fmt.base < 2 || fmt.base > 36) return 0;

	size_t len;

	if (fstr)
	{
		char neg;

		if (fmt.minus && (ptrdiff_t)ival < 0)
		{
			ival = -ival;
			neg = 1;
		}
		else neg = 0;

		char *f = fstr;

		if (fmt.base > 10)
		{
			char a = (fmt.fs & FMT_FS_LOWERCASE) == FMT_FS_LOWERCASE ? 87 : 55, rem;

			do
			{
				rem = ival % fmt.base;
				ival /= fmt.base;
				*f = (rem < 10 ? '0' : a) + rem;
				f++;
			} while (ival);
		}
		else
		{
			do
			{
				*f = '0' + ival % fmt.base;
				ival /= fmt.base;
				f++;
			} while (ival);
		}

		if (fmt.fill)
		{
			char *fw;

			if (fmt.fs & FMT_FS_INTERNAL_FILL)
			{
				fw = fstr + fmt.width - 1;

				while (f < fw)
				{
					*f = fmt.fill;
					f++;
				}

				if (neg)
				{
					*f = fmt.minus;
					f++;
				}
				else if (fmt.plus)
				{
					*f = fmt.plus;
					f++;
				}
				else if (f == fw)
				{
					*f = fmt.fill;
					f++;
				}

				len = f - fstr;
				mtt_mem_rev(fstr, len);
			}
			else if (fmt.fs & FMT_FS_RIGHT_FILL)
			{
				if (neg)
				{
					*f = fmt.minus;
					f++;
				}
				else if (fmt.plus)
				{
					*f = fmt.plus;
					f++;
				}

				mtt_mem_rev(fstr, f - fstr);
				fw = fstr + fmt.width;

				while (f < fw)
				{
					*f = fmt.fill;
					f++;
				}

				len = f - fstr;
			}
			else
			{
				if (neg)
				{
					*f = fmt.minus;
					f++;
				}
				else if (fmt.plus)
				{
					*f = fmt.plus;
					f++;
				}

				fw = fstr + fmt.width;

				while (f < fw)
				{
					*f = fmt.fill;
					f++;
				}

				len = f - fstr;
				mtt_mem_rev(fstr, len);
			}
		}
		else
		{
			if (neg)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}

			len = f - fstr;
			mtt_mem_rev(fstr, len);
		}

		if ((fmt.fs & FMT_FS_NO_NULL_TERM) == 0) *f = 0;
	}
	else
	{
		if (fmt.minus && (ptrdiff_t)ival < 0)
		{
			ival = -ival;
			len = 1;
		}
		else len = fmt.plus ? 1 : 0;

		do
		{
			ival /= fmt.base;
			len++;
		} while (ival);

		if (len < fmt.width) len = fmt.width;
	}

	return len;
}