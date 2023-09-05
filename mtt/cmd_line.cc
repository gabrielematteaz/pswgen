#include "cmd_line.h"

int mtt::extr_optv(int argc, char *argv[], int optc, opt_t *optv) noexcept
{
	if (argv == nullptr || optv == nullptr) return 0;

	char **av = argv + 1, **avc = argv + argc;
	opt_t *ovc = optv + optc;

	while (av < avc)
	{
		char *arg = *av;

		if (*arg == '-')
		{
			char *a = arg + 1, ac = *a;

			if (ac)
			{
				if (ac == '-') av++;
				else
				{
					while (ac)
					{
						opt_t *ov = optv;

						while (ov < ovc)
						{
							if (ac == ov->get_alias())
							{
								int fs = ov->fs;

								if (fs == opt_t::OPTIONAL_ARG)
								{
									a++;
									ov->arg = *a ? a : nullptr;
								}
								else if (fs == opt_t::REQUIRED_ARG)
								{
									a++;

									if (*a) ov->arg = a;
									else
									{
										av++;

										if (*av == nullptr) goto exit;

										ov->arg = *av;
									}
								}
								else
								{
									ov->arg = arg;

									break;
								}

								goto next;
							}

							ov++;
						}

						a++;
						ac = *a;
					}

				next:
					av++;

					continue;
				}
			}
		}

		break;
	}

exit:
	return av - argv;
}