#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void	invalid(void)
{
	printf("invalid input\n");
	exit(1);
}

int					main(void)
{
	char			buff[32];
	unsigned int	n;

	if (!(n = read(0, buff, 31)))
		invalid();
	buff[n - 1] = 0;
	if (strlen(buff) < 5)
		invalid();
	for (n = 0; buff[n]; ++n)
	{
		if ((buff[n] < 0x61) || (0x7a < buff[n]))
			invalid();
	}
	for (n = 0; n < (strlen(buff) - 1); ++n)
	{
		if (buff[n] != (buff[n+1] + 1))
			invalid();
	}
	printf("yeah you got it!\n");
	return 0;
}

