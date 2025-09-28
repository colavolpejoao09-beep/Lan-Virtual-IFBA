#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumeração para tipos de dispositivos
typedef enum {
    ROTEADOR,
    SWITCH,
    HOST
} TipoDispositivo;

// Enumeração para status do dispositivo
typedef enum {
    ATIVO,
    INATIVO
} StatusDispositivo;

// Estrutura para representar um dispositivo na rede
typedef struct Dispositivo {
    int id;
    char nome[50];
    TipoDispositivo tipo;
    StatusDispositivo status;
    struct Dispositivo *proximo;  // Ponteiro para próximo dispositivo na lista
} Dispositivo;

// Estrutura para lista de dispositivos
typedef struct {
    Dispositivo *primeiro;
    int total;
} ListaDispositivos;

// Funções para gerenciamento de dispositivos
ListaDispositivos* criar_lista_dispositivos();
int adicionar_dispositivo(ListaDispositivos *lista, int id, const char *nome, TipoDispositivo tipo);
Dispositivo* buscar_dispositivo(ListaDispositivos *lista, int id);
void listar_dispositivos(ListaDispositivos *lista);
void alterar_status_dispositivo(ListaDispositivos *lista, int id, StatusDispositivo status);
void liberar_lista_dispositivos(ListaDispositivos *lista);
const char* tipo_para_string(TipoDispositivo tipo);
const char* status_para_string(StatusDispositivo status);

#endif