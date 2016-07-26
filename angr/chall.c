#include <stdlib.h>
#include <stdio.h>

#define	START		11
#define NBBLOCKS	9
#define LEN			16

//char*	url = "http://www.insomnihack.ch/3e01460b5548c25403f8ec432594029ca38b6b7c6133d4e7f6aeae4807db4e3db2ca7ab34f485c7f283b9cf0f67e83488218d2d2c39c896af7ff30490c960756/";
char* url = "uggc://jjj.vafbzavunpx.pu/3r01460o5548p25403s8rp432594029pn38o6o7p6133q4r7s6nrnr4807qo4r3qo2pn7no34s485p7s283o9ps0s67r83488218q2q2p39p896ns7ss30490p960756/";

// original blake2 value = 0xd5517861, 0x3463f133, 0xd89c1593, 0xe122f552, 0x4c45153d, 0xb3d2f039, 0x89667ef0, 0x5fcc7302, 0x6ad88400
int		hashes[9] = {
 0xd551784b, 0x3463f133, 0xd89c15a9, 0xe122f552, 0x4c45153d, 0xb3d2f078, 0x89667ef0, 0x5fcc7302, 0x6ad88400
};

int		magic[12] = {
105,
111,
117,
114,
113,
115,
111,
111,
105,
108,
117
};

//137
int			calc_offset(int nothing)
{
	int		tab[5][5];
	int		line[5];
	int		col[5];
	int		i;
	int		j;
	int		res = 0;
	int		nbr = 0;

	tab[0][0] = 47;
	tab[0][1] = 23;
	tab[0][2] = 24;
	tab[0][3] = 37;
	tab[0][4] = 42;
	tab[1][0] = 34;
	tab[1][1] = 10;
	tab[1][2] = 11;
	tab[1][3] = 24;
	tab[1][4] = 29;
	tab[2][0] = 39;
	tab[2][1] = 15;
	tab[2][2] = 16;
	tab[2][3] = 29;
	tab[2][4] = 34;
	tab[3][0] = 46;
	tab[3][1] = 22;
	tab[3][2] = 23;
	tab[3][3] = 36;
	tab[3][4] = 41;
	tab[4][0] = 33;
	tab[4][1] = 9;
	tab[4][2] = 10;
	tab[4][3] = 23;
	tab[4][4] = 28;
	for (i = 0; i < 5; ++i)
	{
		col[i] = 0;
		line[i] = 0;
	}
	srand((unsigned)time(NULL));
	for (nbr = 0; nbr < 5; ++nbr)
	{
		do
		{
			i = rand() % 5;
		} while (line[i]);
		line[i] = 1;
		do
		{
			j = rand() % 5;
		} while (col[j]);
		col[j] = 1;
		res += tab[i][j];
	}
	return res;
}

static char		salad(char c, int n )
{
	char		caps = c & 32;

    c &= ~caps;
    return (((c >= 'A') && (c <= 'Z') ? ((c - 'A' + n) % 26 + 'A') : c) | caps);
}

// solution: $ ./chall `python -c "print 'insomnihack*A\x0b\x16\x99'"`  
int					main(int argc, char** argv)
{
	unsigned int	i = 0;
	int				offset = 0;
	int*			result = NULL;

	if (argc < 2)
		exit (51);
	for (i = 0; i < 11; ++i)
	{
		if (((char)(argv[1][i] + i)) != magic[i])
			exit (42);
	}
	if (argv[1][START] != 42)
		exit (42);
	if (((argv[(START * (strlen(argv[1]) - LEN)) + 1][START + 2] * 2) != argv[1][START + 3]) ||
		((argv[1][START + 2] + 87) != 'b'))
		exit (42);
	offset = calc_offset(strlen(argv[1]));
	if ((argv[1][START + 4] + offset)&0xff != 34)
		exit (42);

	//printf("%08x\n", hashes[0] ^ argv[1][START]); // 0xd5517861 ^ '*' = 0xd551784b
	//printf("%08x\n", hashes[LEN - strlen(argv[1]) + 2] - argv[1][START + 3]); // 0xd89c1593 + 0x16 = 0xd89c15a9
	//printf("%08x\n", hashes[5] ^ 'A'); // 0xb3d2f039 ^ 'A' = 0xb3d2f078
	result = malloc(9 * sizeof (int));
	result[0] = hashes[0] ^ argv[1][START];
	result[1] = 0x3463f133;
	result[2] = hashes[LEN - strlen(argv[1]) + 2] - argv[1][START + 3];
	result[3] = 0xe122f552;
	result[4] = 0x4c45153d;
	result[5] = hashes[5] ^ argv[1][START + 1];
	result[6] = 0x89667ef0;
	result[7] = 0x5fcc7302;
	result[8] = 0x6ad884;
	// here they should check the content of the allocated memory to get the Blake2 value (just remove the last two 0 to get a correct length)
	printf("flag is : 4-%08x%08x%08x%08x%08x%08x%08x%08x%6x\n",result[0],result[1],result[2],result[3],result[4],result[5],result[6],result[7],result[8]);
	printf("Now go to :\n");
	for (i = 0; i < strlen(url); ++i)
		printf("%c", salad(url[i], 13));
	return 0;
}
