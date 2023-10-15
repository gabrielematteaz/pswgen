#ifndef MTT_FMT_H
#define MTT_FMT_H

#include <stddef.h>

#define FMT_FILLING_LEFT 0x00
#define FMT_FILLING_INTERNAL 0x01
#define FMT_FILLING_RIGHT 0x03
#define FMT_FILLING_MASK 0x03

#define FMT_NULL_TERM 0x00
#define FMT_NULL_TERM_NONE 0x04
#define FMT_NULL_TERM_MASK 0x04

#define VALFMT_LTR_CASE_UNK 0x00
#define VALFMT_LTR_CASE_UPPER 0x08
#define VALFMT_LTR_CASE_LOWER 0x18
#define VALFMT_LTR_CASE_MASK 0x18

struct mtt_ival_fmt_t
{
	char plus;
	char minus;
	char fill;
	char base;
	char fs;
	size_t width;
};

size_t mtt_fstr_to_ival(const char *fstr, const char **end, struct mtt_ival_fmt_t fmt);

size_t mtt_ival_to_fstr(char *fstr, size_t ival, struct mtt_ival_fmt_t fmt);

#endif