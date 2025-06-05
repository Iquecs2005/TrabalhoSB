#include "peqcomp.h"

void OnReturn(FILE* f);

funcp peqcomp(FILE* f, unsigned char codigo[])
{
	char comando;

	comando = fgetc(f);
	while (comando != EOF)
	{
		switch (comando)
		{
		case 'r': 
		{
			OnReturn(f);
			break;
		}
		default:
			break;
		}
		comando = fgetc(f);
	}
}

void OnReturn(FILE* f)
{
	int valorRet;

	fscanf(f, "et $%d", &valorRet);
	printf("ret %d", valorRet);
}