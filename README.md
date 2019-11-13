# Cálculo do número PI em C
Neste projeto é feito o cálculo do número PI usando threads e sockets.
Dispomos de dois modos que são: 1 -> Interativo, 2 -> Lote.

## Getting Started

Abaixo as instruções para executar a aplicação.

### Pré-requisito

Sistema linux.

### Compilação

Servidor:
```
gcc server.c -o server -lpthread -lm
```

Cliente(s):
```
gcc client.c -o client -lpthread -lm
```

### Execução

Servidor:
```
./server <parcelas> <clientes> <modo>
```

Cliente(s):
```
./client
```
