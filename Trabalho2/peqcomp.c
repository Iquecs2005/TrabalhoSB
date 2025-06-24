#include "peqcomp.h"

#define DEBUG

void OnReturn(FILE* f, unsigned char codigo[]);
int OnVariable(FILE* f, unsigned char* codigo);
void OnEnter(unsigned char* codigo);
void WriteIntLittleEndien(unsigned char* codigo, unsigned int n);

funcp peqcomp(FILE* f, unsigned char codigo[])
{
	unsigned char* pontAtual = codigo;
	char comando;

	OnEnter(pontAtual);
	pontAtual += 4;

	comando = fgetc(f);
	while (comando != EOF)
	{
		switch (comando)
		{
		case 'r': 
		{
			OnReturn(f, pontAtual);
			pontAtual += 7;
			break;
		}
		case 'v':
		{
			pontAtual += OnVariable(f, pontAtual);
			break;
		}
		default:
			break;
		}
		comando = fgetc(f);
	}

	return (funcp)codigo;
}

void OnEnter(unsigned char* codigo)
{
	unsigned char codigoRetorno[] = {0x55, 0x48, 0x89, 0xe5};

	for (int i = 0; i < 4; i++)
	{
		codigo[i] = codigoRetorno[i];
	}
}

int OnVariable(FILE* f, unsigned char* codigo)
{
	unsigned char codigoRetorno[] = {0xb8, 0x00, 0x00, 0x00, 0x00, 0xc9, 0xc3};
	int nv1, nv2, indiceAtual = 0;
	char operando, vOuC;

	fscanf(f, "%d %c", &nv1, &operando);

	if (operando == ':')
	{
		//variable assignement

		fscanf(f, " %c%d", &operando, &nv2);

		if (operando == '$')
		{
			//variable assignement with a constant
			if (nv1 == 1)
			{
				//movl $const, %eax
				codigo[indiceAtual++] = 0xb9;
			}
			else
			{
				//movl $const, %r__d
				codigo[indiceAtual++] = 0x41;

				if (nv1 == 2)
				{
					//%r8d
					codigo[indiceAtual++] = 0xb8;
				}
				else if (nv1 == 3)
				{
					//%r9d
					codigo[indiceAtual++] = 0xb9;
				}
				else if (nv1 == 4)
				{
					//%r10d
					codigo[indiceAtual++] = 0xba;
				}
				else
				{
					//%r11d
					codigo[indiceAtual++] = 0xbb;
				}
			}
			
			//Writes constant
			WriteIntLittleEndien(codigo + indiceAtual, nv2);
			indiceAtual += 4;
		}
		else
		{

		}
	}

	return indiceAtual;
}

void OnReturn(FILE* f, unsigned char* codigo)
{
	char constante;
	int indexAtual = 0, valorRet;

	fscanf(f, "et %c%d\n", &constante, &valorRet);

	if (constante == '$')
	{
		//movl $const, %eax
		codigo[indexAtual++] = 0xb8;
		WriteIntLittleEndien(codigo + indexAtual, valorRet);
		indexAtual += 4;
	}
	else
	{
		if (valorRet == 1)
		{
			//movl %ecx, %eax
			codigo[indexAtual++] = 0x89;
			codigo[indexAtual++] = 0xc8;
		}
		else if (valorRet == 2)
		{
			//mov %r8d,%eax
			codigo[indexAtual++] = 0x44;
			codigo[indexAtual++] = 0x89;
			codigo[indexAtual++] = 0xc0;
		}
		else if (valorRet == 3)
		{
			//mov %r9d,%eax
			codigo[indexAtual++] = 0x44;
			codigo[indexAtual++] = 0x89;
			codigo[indexAtual++] = 0xc8;
		}
		else if (valorRet == 4)
		{
			//mov %r10d,%eax
			codigo[indexAtual++] = 0x44;
			codigo[indexAtual++] = 0x89;
			codigo[indexAtual++] = 0xd0;
		}
		else
		{
			//mov %r11d,%eax
			codigo[indexAtual++] = 0x44;
			codigo[indexAtual++] = 0x89;
			codigo[indexAtual++] = 0xd8;
		}
	}

	//leave
	codigo[indexAtual++] = 0xc9;
	//ret
	codigo[indexAtual++] = 0xc3; 
}

void WriteIntLittleEndien(unsigned char* codigo, unsigned int n)
{
	//Writes an integer into an array in Little Endien
	for (int i = 0; i < 4; i++)
	{
		codigo[i] = 0xff & (n >> (i * 8));
	}
}
