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

	fprintf(arquivo, "%c", (unsigned char)nstructs);
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
				codificaInt((int*)((char*)valores + indiceEstrutura), descritor + j, arquivo);
				indiceEstrutura += sizeof(int);
				hasInt = 1;
				break;
			case 'u':
				indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(unsigned));
				codificaU((unsigned*)((char*)valores + indiceEstrutura), descritor + j, arquivo);
				indiceEstrutura += sizeof(unsigned);
				hasInt = 1;
				break;
			case 's':
				codificaString((char*)valores + indiceEstrutura, descritor + j, arquivo);
				indiceEstrutura += (descritor[j + 1] - '0') * 10 + descritor[j + 2] - '0';
				j += 2;
				break;
			}
			j++;
		}

		if (hasInt)
			indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(int));
	}
}

void mostracomp(FILE* arquivo)
{
	unsigned char nEstruturas = 0;
	unsigned char charLido;
	
	fscanf(arquivo, "%c", &nEstruturas);
	printf("Estruturas: %u\n\n", nEstruturas);

	for (int j = 0; j < nEstruturas; j++)
	{
		charLido = 0;

		while (!(charLido & 0x80))
		{
			fscanf(arquivo, "%c", &charLido);

			if (charLido & 0x40)
			{
				//Str
				int tamanhoStr = charLido & 0x3f;
				char caracterString;

				printf("(str) ");

				for (int i = 0; i < tamanhoStr; i++)
				{
					fscanf(arquivo, "%c", &caracterString);
					printf("%c", caracterString);
				}

				printf("\n");
			}
			else if (charLido & 0x20)
			{
				int tamanhoInt = charLido & 0x1f;
				unsigned char byteInt;
				int numTotal = 0;

				for (int i = 0; i < tamanhoInt; i++)
				{
					numTotal = numTotal << 8;
					fscanf(arquivo, "%c", &byteInt);
					numTotal += byteInt;
				}

				if (tamanhoInt != 4 && (numTotal >> (tamanhoInt - 1)*8) & 0x80)
				{
					numTotal += 0xffffffff << (tamanhoInt * 8);
				}
				printf("(int) %-11d (%08x)\n", numTotal, numTotal);
			}
			else
			{
				int tamanhoU = charLido & 0x1f;
				unsigned char byteInt;
				unsigned numTotal = 0;

				for (int i = 0; i < tamanhoU; i++)
				{
					numTotal = numTotal << 8;
					fscanf(arquivo, "%c", &byteInt);
					numTotal += byteInt;
				}

				printf("(uns) %-11u (%08x)\n", numTotal, numTotal);
			}
		}

		printf("\n");
	}
}

int calculaIndice(int indiceAtual, int tamanhoCampo)
{
	int modulo;

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

	while (str[tamanhoStr] != '\0')
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
	for (int i = 0; i < tamanhoInt; i++)
	{
		byteShift = *num >> (tamanhoInt - 1 - i) * 8;
		byteShift = byteShift & 0xFF;
		fprintf(arquivo, "%c", byteShift);
	}
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
	for (int i = 0; i < tamanhoInt; i++)
	{
		byteShift = *num >> (tamanhoInt - 1 - i) * 8;
		byteShift = byteShift & 0xFF;
		fprintf(arquivo, "%c", byteShift);
	}
}
