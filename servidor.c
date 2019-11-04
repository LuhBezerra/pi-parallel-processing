#include <stdio.h>
#include <stdlib.h>

int numParcelas, numClientes, tipoProcessamento;

void inicio(){
    printf("Informe o numero de parcelas: ");
    scanf("%d", &numParcelas);
    printf("Informe o numero de clientes: ");
    scanf("%d", &numParcelas);
    printf("Escolha qual o tipo de processamento desejado: ");
    printf("\n\t1 -> Interativo");
    printf("\n\t2 -> Em lote");
    printf("\n>> ");
    scanf("%d", &numParcelas);
}

int main(void)
{
    inicio()
    return (0);
}