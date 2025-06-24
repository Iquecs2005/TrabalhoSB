#include "peqcomp.h"

int main(int argc, char* argv[]) {
    FILE* myfp;
    unsigned char codigo[100];
    funcp funcaoSBas;

    /* Abre o arquivo fonte */
    myfp = fopen("programa.txt", "r");
    if (myfp == NULL) 
    {
        perror("Falha na abertura do arquivo fonte");
        return 1;
    }
    /* compila a fun��o SBas */
    funcaoSBas = peqcomp(myfp, codigo);
    int i = 0;
    while (codigo[i] != 0xc3)
    {
        printf("%02x ", codigo[i]);
        i += 1;
    }
    printf("%02x\n", codigo[i]);
    printf("%d\n", funcaoSBas(1,1,2));
    fclose(myfp);
}