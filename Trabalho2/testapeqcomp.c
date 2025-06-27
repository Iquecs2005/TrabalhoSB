/* Henrique Carvalho e Silva 2410365 3WA */
/* Joao Miguel Rodrigues Montenegro da Franca 2411289 3WA */

#include "peqcomp.h"
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
    char* nomeArq;
    FILE* myfp;
    unsigned char codigo[100];
    funcp funcaoSBas;

    nomeArq = argv[1];
    myfp = fopen(nomeArq, "r");
    if (myfp == NULL) 
    {
        perror("Falha na abertura do arquivo fonte");
        return 1;
    }

    funcaoSBas = peqcomp(myfp, codigo);

    int i = 2;
    int args[] = {0, 0, 0};
    while (i < argc)
    {
        args[i-2] = atoi(argv[i]);
        i++;
    }
    
    printf("%d\n", funcaoSBas(args[0], args[1], args[2]));
    fclose(myfp);
}