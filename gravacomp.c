/* Henrique Carvalho e Silva 2410365 3WB */
/* Joao Miguel Rodrigues 2411289 3WB */

#include "gravacomp.h"

int calculaIndice(int indiceAtual, int tamanhoCampo);
void codificaString(char* str, char* descritor, FILE* arquivo);
void codificaInt(int* num, char* descritor, FILE* arquivo);
void codificaU(unsigned int* num, char* descritor, FILE* arquivo);

int gravacomp(int nstructs, void* valores, char* descritor, FILE* arquivo)
{
	int i, j, indiceEstrutura = 0;
	char hasInt;

	fprintf(arquivo, "%uc", nstructs);
	for (i = 0; i < nstructs; i++)
	{
		j = 0;
		hasInt = 0;

		while (descritor[j] != '\0')
		{
			switch (descritor[j])
			{
			case 'i':
				indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(int));
				printf("%d %c\n", indiceEstrutura, 'i');
				codificaInt((char*)valores + indiceEstrutura, descritor + j, arquivo);
				indiceEstrutura += sizeof(int);
				hasInt = 1;
				break;
			case 'u':
				indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(unsigned));
				printf("%d %c\n", indiceEstrutura, 'u');
				codificaU((char*)valores + indiceEstrutura, descritor + j, arquivo);
				indiceEstrutura += sizeof(unsigned);
				hasInt = 1;
				break;
			case 's':
				printf("%d %c\n", indiceEstrutura, 's');
				codificaString((char*)valores + indiceEstrutura, descritor + j, arquivo);
				indiceEstrutura += (descritor[j + 1] - '0')*10 + descritor[j + 2] - '0';
				j += 2;
				break;
			}
			j++;
		}

		if(hasInt)
			indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(int));
	}
}

void mostracomp(FILE* arquivo)
{
	
}

int calculaIndice(int indiceAtual, int tamanhoCampo)
{
	int indiceResultado, modulo;

	modulo = indiceAtual % tamanhoCampo;
	if (modulo == 0)
	{
		return indiceAtual;
	}
	
	indiceAtual = indiceAtual + tamanhoCampo - modulo;

	return indiceAtual;
}

void codificaString(char* str, char* descritor, FILE* arquivo)
{
	unsigned char byteCabeca = 0x40, tamanhoStr = 0;

	while(str[tamanhoStr] != '\0')
	{
		tamanhoStr++;
	}

	if (descritor[3] == '\0')
	{
		byteCabeca += 0x80;
	}
	byteCabeca += tamanhoStr;

	fprintf(arquivo, "%c", byteCabeca);
	for (int i = 0; i < tamanhoStr; i++)
	{
		fprintf(arquivo, "%c", str[i]);
	}

	return;
}

void codificaInt(int* num, char* descritor, FILE* arquivo)
{
	unsigned char byteCabeca = 0x20, tamanhoInt = 0;
	int byteShift;

	if (descritor[1] == '\0')
	{
		byteCabeca += 0x80;
	}

	if (*num <= 0x7f && *num >= -0x80)
	{
		tamanhoInt = 1;
	}
	else if (*num <= 0x7fff && *num >= -0x8000)
	{
		tamanhoInt = 2;
	}
	else if (*num <= 0x7fffff && *num >= -0x800000)
	{
		tamanhoInt = 3;
	}
	else
	{
		tamanhoInt = 4;
	}

	byteCabeca += tamanhoInt;

	fprintf(arquivo, "%c", byteCabeca);
	printf("%x   ", byteCabeca);
	for (int i = 0; i < tamanhoInt; i++)
	{
		byteShift = *num >> (tamanhoInt - 1 - i) * 8;
		byteShift = byteShift & 0xFF;
		printf("%x ", byteShift);
		fprintf(arquivo, "%c", byteShift);
	}
	printf("\n");
}

void codificaU(unsigned int* num, char* descritor, FILE* arquivo)
{
	unsigned char byteCabeca = 0, tamanhoInt = 0;
	int byteShift;

	if (descritor[1] == '\0')
	{
		byteCabeca += 0x80;
	}

	if (*num <= 0xff)
	{
		tamanhoInt = 1;
	}
	else if (*num <= 0xffff)
	{
		tamanhoInt = 2;
	}
	else if (*num <= 0xffffff)
	{
		tamanhoInt = 3;
	}
	else
	{
		tamanhoInt = 4;
	}

	byteCabeca += tamanhoInt;

	fprintf(arquivo, "%c", byteCabeca);
	printf("%x   ", byteCabeca);
	for (int i = 0; i < tamanhoInt; i++)
	{
		byteShift = *num >> (tamanhoInt - 1 - i) * 8;
		byteShift = byteShift & 0xFF;
		printf("%x ", byteShift);
		fprintf(arquivo, "%c", byteShift);
	}
	printf("\n");
}
