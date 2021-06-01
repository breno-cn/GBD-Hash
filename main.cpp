#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <time.h>

using namespace std;

// utilize um tamanho de histograma adequado para a quantidade de buckets para os conjuntos de dados fornecidos
#define TAM_HISTOGRAMA 10000
#define TAM_BUCKET 200
#define SEED_FIXA 7

// vetor que armazenara histograma de contagem
int histograma[TAM_HISTOGRAMA];

int getHash(char *str) {
    unsigned int hash = 0;
    int c;
    int length = strlen(str);
    srand(SEED_FIXA);

    for (int i = 0; i < length; i++) {
        c = str[i];
        hash = (c + (hash << 7) + (hash << 13) - hash) % TAM_HISTOGRAMA;
    }

    if (histograma[hash] >= TAM_BUCKET) {
        while (true) {
            int incremento = rand() % TAM_HISTOGRAMA; 
            
            if (histograma[(hash + incremento) % TAM_HISTOGRAMA] < TAM_BUCKET) {
                hash += incremento;
                break;
            } 
        }
    }

    return hash % TAM_HISTOGRAMA;
}

int main() {
    FILE *f = fopen("dblp-1m.txt","rb");
    if (f == NULL) {
        printf("Sinto muito, nao consegui abrir arquivo de dados.");
        return 0;
    }

    char linha[1024];


    for (int i = 0; i < TAM_HISTOGRAMA; i++)
        histograma[i] = 0;

    int contagem = 0;
    while (true) {
        // ler linha
        fgets(linha,1024,f);
        if (feof(f))
            break;

        // computar hash e atualizar contagem no histograma
        int hash = getHash(linha);
        histograma[hash]++;

        // para fornecer algum feedback para usuario nao pensar que programa travou, imprimir um caractere a cada 1000 elementos
        contagem++; if (contagem % 1000 == 0) { printf("."); fflush(stdout); }
    }
    fclose(f);

    // criar grafico de contagem do hash
    FILE *grafico = fopen("grafico.plt","wb");
    fprintf(grafico,"# executar esse arquivo com GNUPLOT\n");
    fprintf(grafico,"# http://www.gnuplot.info/download.html\n");
    fprintf(grafico,"# no linux: gnuplot grafico.plt\n");
    fprintf(grafico,"# no windows: wgnuplot grafico.plt\n");
    fprintf(grafico,"set title \"Histograma\"\n");
    fprintf(grafico,"set encoding iso_8859_1\n");
    fprintf(grafico,"set xlabel \"Hash\"\n");
    fprintf(grafico,"set ylabel \"Quantidade\"\n");
    fprintf(grafico,"plot '-' title 'Quantidade' with linespoints linewidth 2 linetype 1 pointtype 1\n");
    for (int i = 0; i < TAM_HISTOGRAMA; i++)
        fprintf(grafico,"%d %d\n",i,histograma[i]);
    fprintf(grafico,"end\n");
    fprintf(grafico,"pause -1\n");
    fclose(grafico);

    printf("\n\nThat's all, folks!\n\n");

    return 0;
}
