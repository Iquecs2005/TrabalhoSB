/* Henrique Carvalho e Silva 2410365 3WB */
/* Joao Miguel Rodrigues 2411289 3WB */

#include "gravacomp.h"

int calculaIndice(int indiceAtual, int tamanhoCampo);
int codificaString(char* str, char* descritor, FILE* arquivo);
int codificaInt(int* num, char* descritor, FILE* arquivo);
int codificaU(unsigned int* num, char* descritor, FILE* arquivo);

int gravacomp(int nstructs, void* valores, char* descritor, FILE* arquivo)
{
	int i, j, indiceEstrutura = 0;
	char hasInt;

	int err;
	//grava no arquivo o numero de estruturas que serao gravadas
	err = fprintf(arquivo, "%c", (unsigned char)nstructs);
	//a funcao fprintf retorna um valor negativo em caso de erro entao nesse caso a funcao gravacomp retorna -1
	if (err < 0)
	{
		return -1;
	}
	//percorre as estruturas 
	for (i = 0; i < nstructs; i++)
	{
		j = 0;
		hasInt = 0;

		//percorre a string do descritor
		while (descritor[j] != '\0')
		{
			//realiza passos diferentes em cada um dos casos do tipo de dado na estrutura
			switch (descritor[j])
			{
			case 'i':
				indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(int));
				err = codificaInt((int*)((char*)valores + indiceEstrutura), descritor + j, arquivo);
				//a funcao fprintf retorna um valor negativo em caso de erro entao nesse caso a funcao gravacomp retorna -1
				if (err < 0)
				{
					return -1;
				}
				indiceEstrutura += sizeof(int);
				hasInt = 1;
				break;
			case 'u':
				indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(unsigned));
				err = codificaU((unsigned*)((char*)valores + indiceEstrutura), descritor + j, arquivo);
				//a funcao fprintf retorna um valor negativo em caso de erro entao nesse caso a funcao gravacomp retorna -1
				if (err < 0)
				{
					return -1;
				}
				indiceEstrutura += sizeof(unsigned);
				hasInt = 1;
				break;
			case 's':
				err = codificaString((char*)valores + indiceEstrutura, descritor + j, arquivo);
				//a funcao fprintf retorna um valor negativo em caso de erro entao nesse caso a funcao gravacomp retorna -1
				if (err < 0)
				{
					return -1;
				}
				indiceEstrutura += (descritor[j + 1] - '0') * 10 + descritor[j + 2] - '0';
				j += 2;
				break;
			}
			j++;
		}

		//se tiver lidando com um inteiro na estrutura, chama uma funcao de apoio para calcular o indice por conta do padding externo necessario
		if (hasInt)
			indiceEstrutura = calculaIndice(indiceEstrutura, sizeof(int));
	}
	return 0;
}

void mostracomp(FILE* arquivo)
{
	unsigned char nEstruturas = 0;
	unsigned char charLido;
	
	//le o primeiro caractere do arquivo binario que indica o numero de estruturas e mostra ele na tela
	fscanf(arquivo, "%c", &nEstruturas);
	printf("Estruturas: %u\n\n", nEstruturas);

	//percorre as estruturas do arquivo com base no numero total lido anteriormente
	for (int j = 0; j < nEstruturas; j++)
	{
		charLido = 0;

		//continua o loop ate o bit que indica se o campo estrutura eh o ultimo for 1 
		while (!(charLido & 0x80))
		{
			fscanf(arquivo, "%c", &charLido);

			//checa se o bit (ou os no caso de um inteiro) lido que define o tipo de dado e de string, int ou unsigned int
			if (charLido & 0x40)
			{
				//le os bytes que indicam o tamanho da string
				int tamanhoStr = charLido & 0x3f;
				char caracterString;

				printf("(str) ");

				//com base no tamanho lido, vai lendo os caracteres da string e mostrando na tela
				for (int i = 0; i < tamanhoStr; i++)
				{
					fscanf(arquivo, "%c", &caracterString);
					printf("%c", caracterString);
				}

				printf("\n");
			}
			else if (charLido & 0x20)
			{
				//le os bytes que indicam o tamanho do inteiro
				int tamanhoInt = charLido & 0x1f;
				unsigned char byteInt;
				int numTotal = 0;

				//com base no tamanho lido, le byte a byte e desloca um byte antes de ler o proximo para numTotal recriar o numero original
				for (int i = 0; i < tamanhoInt; i++)
				{
					numTotal = numTotal << 8;
					fscanf(arquivo, "%c", &byteInt);
					numTotal += byteInt;
				}

				//trata de numeros negativos com menos de 4 bytes
				if (tamanhoInt != 4 && (numTotal >> (tamanhoInt - 1)*8) & 0x80)
				{
					numTotal += 0xffffffff << (tamanhoInt * 8);
				}
				//exibe na tela o numero e seu valor em hexadecimal
				printf("(int) %-11d (%08x)\n", numTotal, numTotal);
			}
			else
			{
				//le os bytes que indicam o tamanho do inteiro sem sinal
				int tamanhoU = charLido & 0x1f;
				unsigned char byteInt;
				unsigned numTotal = 0;

				//com base no tamanho lido, le byte a byte e desloca um byte antes de ler o proximo para numTotal recriar o numero original
				for (int i = 0; i < tamanhoU; i++)
				{
					numTotal = numTotal << 8;
					fscanf(arquivo, "%c", &byteInt);
					numTotal += byteInt;
				}

				//exibe na tela o numero e seu valor em hexadecimal
				printf("(uns) %-11u (%08x)\n", numTotal, numTotal);
			}
		}

		printf("\n");
	}
}

//Calcula o indice da estrutura, respeitando o padding
int calculaIndice(int indiceAtual, int tamanhoCampo)
{
	int modulo;

	modulo = indiceAtual % tamanhoCampo;
	if (modulo == 0)
	{
		return indiceAtual;
	}

	//muda o indice para ajeitar o padding em caso de um inteiro (precisa iniciar em indice multiplo de 4, seu tamanho em bytes)
	indiceAtual = indiceAtual + tamanhoCampo - modulo;

	return indiceAtual;
}

int codificaString(char* str, char* descritor, FILE* arquivo)
{
	unsigned char byteCabeca = 0x40, tamanhoStr = 0;
	int err;

	while (str[tamanhoStr] != '\0')
	{
		tamanhoStr++;
	}

	if (descritor[3] == '\0')
	{
		byteCabeca += 0x80;
	}
	byteCabeca += tamanhoStr;

	err = fprintf(arquivo, "%c", byteCabeca);
	if (err < 0)
	{
		return -1;
	}
	for (int i = 0; i < tamanhoStr; i++)
	{
		err = fprintf(arquivo, "%c", str[i]);
		if (err < 0)
		{
			return -1;
		}
	}

	return 0;
}

int codificaInt(int* num, char* descritor, FILE* arquivo)
{
	unsigned char byteCabeca = 0x20, tamanhoInt = 0;
	int byteShift;
	int err;

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

	err = fprintf(arquivo, "%c", byteCabeca);
	if (err < 0)
	{
		return -1;
	}
	for (int i = 0; i < tamanhoInt; i++)
	{
		byteShift = *num >> (tamanhoInt - 1 - i) * 8;
		byteShift = byteShift & 0xFF;
		err = fprintf(arquivo, "%c", byteShift);
		if (err < 0)
		{
			return -1;
		}
	}
	return 0;
}

int codificaU(unsigned int* num, char* descritor, FILE* arquivo)
{
	unsigned char byteCabeca = 0, tamanhoInt = 0;
	int byteShift;
	int err;

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

	err = fprintf(arquivo, "%c", byteCabeca);
	if (err < 0)
	{
		return -1;
	}
	for (int i = 0; i < tamanhoInt; i++)
	{
		byteShift = *num >> (tamanhoInt - 1 - i) * 8;
		byteShift = byteShift & 0xFF;
		err = fprintf(arquivo, "%c", byteShift);
		if (err < 0)
		{
			return -1;
		}
	}
	return 0;
}
