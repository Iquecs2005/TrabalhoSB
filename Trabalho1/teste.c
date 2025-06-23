#include "gravacomp.h"

struct s {
	char s1[4];
	int  i1, i2;
	unsigned int u1;
	char s2[26];
};

int main(void)
{
	struct s exemplo[5] = { {"abe", 0x80000000, 5, 0xffffffff, "olaMun"}, {"jh", 5, 23, 95, "ooMun"},
							{"abc", 5, 467, 27, "on"}, {"abb", 25, 16, 5, "Mun"}, 
							{"a", 62, 2, 56, "on132973tasdgs"}};

	FILE* arq = fopen("arquivo", "wb");

	gravacomp(5, &exemplo, "s04iius26", arq);

	fclose(arq);

	arq = fopen("arquivo", "rb");

	mostracomp(arq);
	
	fclose(arq);
}