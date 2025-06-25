#include "peqcomp.h"
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
    FILE* myfp;
    unsigned char codigo[100];
    funcp funcaoSBas;

    myfp = fopen("programa.txt", "r");
    if (myfp == NULL) 
    {
        perror("Falha na abertura do arquivo fonte");
        return 1;
    }

    funcaoSBas = peqcomp(myfp, codigo);

    int i = 1;
    int args[] = {0, 0, 0};
    while (i < argc)
    {
        args[i-1] = atoi(argv[i]);
        i++;
    }
    
    printf("%d\n", funcaoSBas(args[0], args[1], args[2]));
    fclose(myfp);
}