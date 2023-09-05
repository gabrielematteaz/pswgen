#ifndef MTT_STR_H
#define MTT_STR_H

#include <stdexcept>

namespace mtt
{
	class fstr_to_ival_fmt_t
	{
	public:
		static constexpr int LEFT_FILL = 0x00;
		static constexpr int INTERNAL_FILL = 0x01;
		static constexpr int RIGHT_FILL = 0x02;

		static constexpr int UNK_CASE = 0x00;
		static constexpr int UPPERCASE = 0x04;
		static constexpr int LOWERCASE = 0x0C;

		char plus;
		char minus;
		char fill;
	
	protected:
		char base;
	
	public:
		int fs;

		fstr_to_ival_fmt_t() noexcept
			: plus('+'), minus('-'), fill(' '), base(10), fs(LEFT_FILL)
		{

		}

		fstr_to_ival_fmt_t(char plus, char minus, char fill, char base, int fs)
			: plus(plus), minus(minus), fill(fill), base(base < 2 || base > 36 ? throw std::invalid_argument("base") : base), fs(fs)
		{

		}

		char get_base() const noexcept
		{
			return base;
		}

		bool set_base(char base) noexcept
		{
			if (base < 2 || base > 36) return false;

			this->base = base;

			return true;
		}

		std::size_t conv(const char *fstr, const char **end = nullptr) const noexcept;
	};

	class ival_to_fstr_fmt_t : public fstr_to_ival_fmt_t
	{
	public:
		static constexpr int NO_NULL_TERM = 0x10;

		std::size_t width;

		ival_to_fstr_fmt_t() noexcept
			: fstr_to_ival_fmt_t(0, '-', ' ', 10, LEFT_FILL), width(0)
		{

		}

		ival_to_fstr_fmt_t(char plus, char minus, char fill, char base, int fs, std::size_t width)
			: fstr_to_ival_fmt_t(plus, minus, fill, base, fs), width(width)
		{

		}

		std::size_t conv(char *fstr, std::size_t ival) const noexcept;
	};

	void *mem_rev(void *mem, std::size_t n) noexcept;
}

#endif