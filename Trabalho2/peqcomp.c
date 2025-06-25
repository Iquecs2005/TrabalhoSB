#include "peqcomp.h"
#include <stdlib.h>

#define DEBUG

typedef struct filaJump FilaJump;
struct filaJump
{
	unsigned char* ptn;
	int linha;
	FilaJump* prox;
};

FilaJump* listaJumps;

unsigned char vLinhasSBas[30];

unsigned char* pontAtual;

void OnEnter();
void OnVariable(FILE* f);
void Att(int nv1, char demarcador, int nv2);
void Expr(int vResultado, char def1, int v1, char def2, int v2, char operador);
void OnReturn(FILE* f);
void OnCondition(FILE* f);
void WriteIntLittleEndian(unsigned int n);
void Linkedicao(unsigned char codigo[]);
FilaJump* createList();
int appendList(unsigned char* ptn, int linha);
void freeList();

funcp peqcomp(FILE* f, unsigned char codigo[])
{
	int linha;
	char comando;
	
	listaJumps = createList();
	
	pontAtual = codigo;
	linha = 0;
	
	OnEnter();
	
	comando = fgetc(f);
	while (comando != EOF)
	{
		vLinhasSBas[linha] = (unsigned char)(pontAtual - codigo); 
		switch (comando)
		{
		case 'r': 
		{
			OnReturn(f);
			break;
		}
		case 'v':
		{
			OnVariable(f);
			break;
		}
		case 'i':
		{
			OnCondition(f);
			break;
		}
		default:
			break;
		}
		linha++;
		comando = fgetc(f);
	}

	Linkedicao(codigo);

	freeList();
	return (funcp)codigo;
}

void OnEnter()
{
	//TO DO: O valor Ox14 deve ser o valor correto do RA
	//push   %rbp
	//mov    %rsp,%rbp
   	//sub    $0x14,%rsp
	unsigned char codigoRetorno[] = {0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x14};

	for (int i = 0; i < 8; i++)
	{
		*pontAtual++ = codigoRetorno[i];
	}
}

void OnVariable(FILE* f)
{
	int nv1, nv2;
	char operando;

	fscanf(f, "%d %c ", &nv1, &operando);

	if (operando == ':')
	{
		//variable assignement
		fscanf(f, " %c%d\n", &operando, &nv2);
		Att(nv1, operando, nv2);
	}
	else if (operando == '=')
	{
		int nv3;
		char def1, def2;
		fscanf(f, " %c%d %c %c%d\n", &def1, &nv2, &operando, &def2, &nv3);
		Expr(nv1, def1, nv2, def2, nv3, operando);
	}
}

void Att(int nv1, char demarcador, int nv2)
{
	if (demarcador == '$')
	{
		//movl $nv2, -4*nv1(%rbp)
		*pontAtual++ = 0xc7;
		*pontAtual++ = 0x45;
		*pontAtual++ = -(4*nv1);
		WriteIntLittleEndian(nv2);
	}
	else if (demarcador == 'v')
	{
		//movl -4*nv2(rbp), %eax
		*pontAtual++ = 0x8b;
		*pontAtual++ = 0x45;
		*pontAtual++ = -(4*nv2);
		//movl %eax, -4*nv2(rbp)
		*pontAtual++ = 0x89;
		*pontAtual++ = 0x45;
		*pontAtual++ = -(4*nv1);
	}
	else
	{
		*pontAtual++ = 0x89;
		if (nv2 == 1)
		{
			//movl %edi,-nv1(%rbp)
			*pontAtual++ = 0x7d;
		}
		else if (nv2 == 2)
		{
			//movl %esi,-nv1(%rbp)
			*pontAtual++ = 0x75;
		}
		else
		{
			//movl %edx,-nv1(%rbp)
			*pontAtual++ = 0x55;
		}
		*pontAtual++ = -(4*nv1);
	}
}

void Expr(int vResultado, char def1, int v1, char def2, int v2, char operador)
{
	if (def1 == '$')
	{
		//movl $const, %eax
		*pontAtual++ = 0xb8;
		WriteIntLittleEndian(v1);
	}
	else
	{
		//movl -4*valorRet(rbp), %eax
		*pontAtual++ = 0x8b;
		*pontAtual++ = 0x45;
		*pontAtual++ = -(4*v1);
	}

	if (def2 == '$')
	{
		//movl $const, %ecx
		*pontAtual++ = 0xb9;
		WriteIntLittleEndian(v2);
	}
	else
	{
		//movl -4*valorRet(rbp), %ecx
		*pontAtual++ = 0x8b;
		*pontAtual++ = 0x4d;
		*pontAtual++ = -(4*v2);
	}
	
	switch (operador)
	{
		case '+':
			//addl %ecx, %eax
			*pontAtual++ = 0x01;
			*pontAtual++ = 0xc8;
			break;
		case '-':
			//subl %ecx, %eax
			*pontAtual++ = 0x29;
			*pontAtual++ = 0xc8;
			break;
		case '*':
			//imull %ecx, %eax
			*pontAtual++ = 0x0f;
			*pontAtual++ = 0xaf;
			*pontAtual++ = 0xc1;
			break;
	}

	//movl %eax,-4*vResultado(%rbp)
	*pontAtual++ = 0x89;
	*pontAtual++ = 0x45;
	*pontAtual++ = -(4*vResultado);
}

void OnReturn(FILE* f)
{
	char constante;
	int valorRet;

	fscanf(f, "et %c%d\n", &constante, &valorRet);

	//movl
	if (constante == '$')
	{
		//movl $const, %eax
		*pontAtual++ = 0xb8;
		WriteIntLittleEndian(valorRet);
	}
	else
	{
		//movl -4*valorRet(rbp), %eax
		*pontAtual++ = 0x8b;
		*pontAtual++ = 0x45;
		*pontAtual++ = -(4*valorRet);
	}

	//leave
	*pontAtual++  = 0xc9;
	//ret
	*pontAtual++  = 0xc3; 
}

void OnCondition(FILE* f)
{
	int nV, nLinha;

	fscanf(f, "flez v%d %d\n", &nV, &nLinha);

	//cmpl $0x0,-nV(%rbp)
	*pontAtual++  = 0x83;
	*pontAtual++  = 0x7d;
	*pontAtual++  = -(4*nV);
	*pontAtual++  = 0x00;
	// jle
	*pontAtual++  = 0x0f;
	*pontAtual++  = 0x8e;
	//offset
	appendList(pontAtual, nLinha);
	*pontAtual++ = 0x00;
	*pontAtual++ = 0x00;
	*pontAtual++ = 0x00;
	*pontAtual++ = 0x00;
	//*pontAtual++  = 0xcf;
}

void WriteIntLittleEndian(unsigned int n)
{
	//Writes an integer into an array in Little Endian
	for (int i = 0; i < 4; i++)
	{
		*pontAtual++ = 0xff & (n >> (i * 8));
	}
}

void Linkedicao(unsigned char codigo[])
{
	FilaJump* current = listaJumps;

	while (current != NULL)
	{
		//Calcula o offset
		//vLinhasSBas[current->linha - 1] representa o endereco de destino
		//(current->ptn + 4 - codigo) representa o endereco do jump
		unsigned int offset = vLinhasSBas[current->linha - 1] - (current->ptn + 4 - codigo);
		pontAtual = current->ptn;
		WriteIntLittleEndian(offset);
		current = current->prox;
	}
}

FilaJump* createList()
{
	return NULL;
}

int appendList(unsigned char* ptn, int linha)
{
	FilaJump* current = listaJumps, * prev = NULL, * newElement;

	while (current != NULL)
	{
		prev = current;
		current = current->prox;
	}

	newElement = (FilaJump*)malloc(sizeof(FilaJump));
	if (newElement == NULL)
	{
		return 1;
	}
	newElement->ptn = ptn;
	newElement->linha = linha;
	newElement->prox = NULL;

	if (prev != NULL)
	{
		prev->prox = newElement;
	}
	else
	{
		listaJumps = newElement;
	}

	return 0;
}

void freeList()
{
	FilaJump* current = listaJumps, * next = NULL;

	while (current != NULL)
	{
		next = current->prox;
		free(current);
		current = next;
	}
}