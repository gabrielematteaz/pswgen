#ifndef MTT_STR_H
#define MTT_STR_H

#include <stddef.h>

#define FMT_FS_LEFT_FILL 0x00
#define FMT_FS_INTERNAL_FILL 0x01
#define FMT_FS_RIGHT_FILL 0x02

#define FMT_FS_UNK_CASE 0x00
#define FMT_FS_UPPERCASE 0x04
#define FMT_FS_LOWERCASE 0x0C

#define FMT_FS_NO_NULL_TERM 0x10

struct mtt_fmt_t
{
	char plus;
	char minus;
	char fill;
	char base;
	int fs;
	size_t width;
};

void *mtt_mem_rev(void *mem, size_t n);

size_t mtt_fstr_to_ival(const char *fstr, const char **end, struct mtt_fmt_t fmt);

size_t mtt_ival_to_fstr(char *fstr, size_t ival, struct mtt_fmt_t fmt);

#endif