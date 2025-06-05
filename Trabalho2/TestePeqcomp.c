#include "peqcomp.h"

int main(int argc, char* argv[]) {
    FILE* myfp;
    unsigned char codigo[100];
    funcp funcaoSBas;
    int res;

    /* Abre o arquivo fonte */
    myfp = fopen("programa.txt", "r");
    if (myfp == NULL) 
    {
        perror("Falha na abertura do arquivo fonte");
        exit(1);
    }
    /* compila a função SBas */
    funcaoSBas = peqcomp(myfp, codigo);
    fclose(myfp);
}