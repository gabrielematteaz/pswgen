#include "../include/mttopt.h"

int mttopt_extr_optv(int argc, char *argv[], int optc, struct mttopt_opt_t *optv)
{
	if (argv == NULL || optv == NULL) return 0;

	char **av = argv + 1, **avc = argv + argc;
	struct mttopt_opt_t *ovc = optv + optc;
	
	while (av < avc)
	{
		char *arg = *av;

		if (*arg == '-')
		{
			char *a = arg + 1;

			if (*a == '-')
			{
				av++;

				break;
			}

			char ac = *a;

			while (ac)
			{
				struct mttopt_opt_t *ov = optv;

				while (ov < ovc)
				{
					if (ac == ov->shrt)
					{
						int fs = ov->fs;

						if (ov->found)
						{
							if (fs & OPT_FS_IGNORE_COPIES)
							{
								if (fs & OPT_FS_CAN_HAVE_ARG)
								{
									if (fs & OPT_FS_MUST_HAVE_ARG)
									{
										a++;

										if (*a == 0) av++;
									}

									goto next;
								}

								break;
							}
							else if (fs & OPT_FS_EXIT_ON_COPY)
							{
								av++;

								if (fs & OPT_FS_MUST_HAVE_ARG)
								{
									a++;

									if (*a == 0) av++;
								}

								goto exit;
							}
						}

						ov->found = 1;

						if (fs & OPT_FS_CAN_HAVE_ARG)
						{
							a++;

							if ((fs & OPT_FS_MUST_HAVE_ARG) == 12)
							{
								if (*a) ov->arg = a;
								else
								{
									av++;
									ov->arg = *av;
								}
							}
							else ov->arg = *a ? a : NULL;

							goto next;
						}

						ov->arg = NULL;

						break;
					}

					ov++;
				}

				a++;
				ac = *a;
			}
		}
		else break;

	next:
		av++;
	}

exit:
	return av - argv;
}