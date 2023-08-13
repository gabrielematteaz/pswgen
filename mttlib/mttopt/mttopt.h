#ifndef MTTOPT_H
#define MTTOPT_H

#include <stdint.h>
#include <stddef.h>

struct mttopt_opt_t
{
	uint8_t shrt;
	uint8_t flags;
	uint8_t found;
	const char *arg;
};

#define OPT_FLAGS_OVERWRITE_ON_COPY 0
#define OPT_FLAGS_IGNORE_COPIES 1
#define OPT_FLAGS_EXIT_ON_COPY 2
#define OPT_FLAGS_HAS_NO_ARG 0
#define OPT_FLAGS_CAN_HAVE_ARG 4
#define OPT_FLAGS_MUST_HAVE_ARG 12

int mttopt_extr_optv(int argc, char *argv[], int optc, struct mttopt_opt_t *optv);

#endif