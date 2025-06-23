#include "peqcomp.h"

#define DEBUG

void OnReturn(FILE* f, unsigned char codigo[]);

funcp peqcomp(FILE* f, unsigned char codigo[])
{
	unsigned char* pontAtual = codigo;
	char comando;

	comando = fgetc(f);
	while (comando != EOF)
	{
		switch (comando)
		{
		case 'r': 
		{
			OnReturn(f, pontAtual);
			pontAtual += 5;
			break;
		}
		default:
			break;
		}
		comando = fgetc(f);
	}

	return NULL;
}

void OnReturn(FILE* f, unsigned char codigo[])
{
	int valorRet;

	fscanf(f, "et $%d\n", &valorRet);
	#ifdef DEBUG
	printf("[DEBUG] ret %d\n", valorRet);
	#endif

	codigo[0] = 0xc2;
	for (int i = 0; i < 4; i++)
	{
		codigo[i+1] = valorRet & (0xFF << (8 * i));
	}
}
