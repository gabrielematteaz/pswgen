#include "cmd_line.h"

#include <stdio.h>

int mtt_extr_optv(int argc, char *argv[], int optc, struct mtt_opt_t *optv)
{
	if (argv == NULL || optv == NULL) return 0;

	char **av = argv + 1, **avc = argv + argc, *arg, *a, ac;
	struct mtt_opt_t *ov, *ovc = optv + optc;

	while (av < avc)
	{
		arg = *av;

		if (*arg == '-')
		{
			a = arg + 1, ac = *a;

			if (ac)
			{
				if (ac == '-') av++;
				else
				{
					do
					{
						ov = optv;

						while (ov < ovc)
						{
							if (ac == ov->alias)
							{
								int fs = ov->fs;

								if (fs & OPT_FS_OPTIONAL_ARG)
								{
									a++;
									ov->arg = *a ? a : NULL;
								}
								else if (fs & OPT_FS_REQUIRED_ARG)
								{
									a++;

									if (*a) ov->arg = a;
									else
									{
										av++;

										if (*av == NULL) goto error;

										ov->arg = *av;
									}
								}
								else
								{
									ov->arg = arg;

									break;
								}

								goto next_arg;
							}

							ov++;
						}

						a++;
						ac = *a;
					} while (ac);

				next_arg:
					av++;

					continue;
				}
			}
		}

		break;
	}

error:
	return av - argv;
}