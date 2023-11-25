#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int linha;
    int coluna;
    int valor;
} ElementoMatriz;

// Estrutura para representar a matriz esparsa
typedef struct {
    int linhas;
    int colunas;
    int qtdElementos;
    ElementoMatriz *elementos;
} MatrizEsparsa;

MatrizEsparsa lerMatriz(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    MatrizEsparsa matriz;

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }


    fscanf(arquivo, "%d %d %d", &matriz.linhas, &matriz.colunas, &matriz.qtdElementos);


    matriz.elementos = (ElementoMatriz *)malloc(matriz.qtdElementos * sizeof(ElementoMatriz));

    for (int i = 0; i < matriz.qtdElementos; i++) {
        fscanf(arquivo, "%d %d %d", &matriz.elementos[i].linha, &matriz.elementos[i].coluna, &matriz.elementos[i].valor);
    }

    fclose(arquivo);
    return matriz;
}

// Função para somar duas matrizes esparsas
MatrizEsparsa somarMatrizes(MatrizEsparsa matrizA, MatrizEsparsa matrizB) {
    MatrizEsparsa matrizC;
    int tamanhoMax = matrizA.qtdElementos + matrizB.qtdElementos; 

    // Aloca espaço para os elementos da matriz soma
    matrizC.elementos = (ElementoMatriz *)malloc(tamanhoMax * sizeof(ElementoMatriz));

    int i = 0, j = 0, k = 0;

    while (i < matrizA.qtdElementos && j < matrizB.qtdElementos) {
        if (matrizA.elementos[i].linha < matrizB.elementos[j].linha ||
            (matrizA.elementos[i].linha == matrizB.elementos[j].linha && matrizA.elementos[i].coluna < matrizB.elementos[j].coluna)) {
            matrizC.elementos[k++] = matrizA.elementos[i++];
        } else if (matrizA.elementos[i].linha > matrizB.elementos[j].linha ||
                   (matrizA.elementos[i].linha == matrizB.elementos[j].linha && matrizA.elementos[i].coluna > matrizB.elementos[j].coluna)) {
            matrizC.elementos[k++] = matrizB.elementos[j++];
        } else {
            matrizC.elementos[k] = matrizA.elementos[i];
            matrizC.elementos[k++].valor += matrizB.elementos[j++].valor;
        }
    }

    
    while (i < matrizA.qtdElementos) {
        matrizC.elementos[k++] = matrizA.elementos[i++];
    }

    
    while (j < matrizB.qtdElementos) {
        matrizC.elementos[k++] = matrizB.elementos[j++];
    }

    matrizC.linhas = matrizA.linhas;
    matrizC.colunas = matrizA.colunas;
    matrizC.qtdElementos = k;

    return matrizC;
}

MatrizEsparsa multiplicarMatrizes(MatrizEsparsa matrizA, MatrizEsparsa matrizB) {
    MatrizEsparsa matrizD;
    int tamanhoMax = matrizA.qtdElementos * matrizB.qtdElementos; 

    // Aloca espaço para os elementos da matriz multiplicação
    matrizD.elementos = (ElementoMatriz *)malloc(tamanhoMax * sizeof(ElementoMatriz));

    int k = 0;

    for (int i = 0; i < matrizA.qtdElementos; i++) {
        for (int j = 0; j < matrizB.qtdElementos; j++) {
            if (matrizA.elementos[i].coluna == matrizB.elementos[j].linha) {
                // Multiplica os valores correspondentes
                matrizD.elementos[k].linha = matrizA.elementos[i].linha;
                matrizD.elementos[k].coluna = matrizB.elementos[j].coluna;
                matrizD.elementos[k].valor = matrizA.elementos[i].valor * matrizB.elementos[j].valor;
                k++;
            }
        }
    }

    // Atualiza o número de linhas, colunas e elementos
    matrizD.linhas = matrizA.linhas;
    matrizD.colunas = matrizB.colunas;
    matrizD.qtdElementos = k;

    return matrizD;
}

// Função para exibir as matriz
void exibirMatriz(MatrizEsparsa matriz) {
    int k = 0;

    for (int i = 0; i < matriz.linhas; i++) {
        for (int j = 0; j < matriz.colunas; j++) {
            if (k < matriz.qtdElementos && matriz.elementos[k].linha == i && matriz.elementos[k].coluna == j) {
                printf("%d ", matriz.elementos[k++].valor);
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

int main() {
    
    MatrizEsparsa matrizA = lerMatriz("matrizA.txt");
    MatrizEsparsa matrizB = lerMatriz("matrizB.txt");

    MatrizEsparsa matrizC = somarMatrizes(matrizA, matrizB);

    MatrizEsparsa matrizD = multiplicarMatrizes(matrizA, matrizB);


    printf("Matriz A:\n");
    exibirMatriz(matrizA);

    printf("\nMatriz B:\n");
    exibirMatriz(matrizB);

    printf("\nMatriz C (A + B):\n");
    exibirMatriz(matrizC);

    printf("\nMatriz D (A * B):\n");
    exibirMatriz(matrizD);

    free(matrizA.elementos);
    free(matrizB.elementos);
    free(matrizC.elementos);
    free(matrizD.elementos);

    return 0;
}
