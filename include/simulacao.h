#ifndef SIMULACAO_H
#define SIMULACAO_H

#include "dispositivo.h"
#include "conexao.h"

// Estrutura para representar um pacote na rede
typedef struct {
    int origem_id;
    int destino_id;
    char dados[256];
    int hops;
    char caminho[512];
} Pacote;

// Funções para simulação de tráfego
int simular_envio_pacote(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador, int origem_id, int destino_id, const char *dados);
void registrar_log(const char *mensagem);
void registrar_falha(const char *mensagem);
int encontrar_caminho_recursivo_seguro(GerenciadorConexoes *gerenciador, ListaDispositivos *dispositivos, int atual_id, int destino_id, int *visitados_ids, int *visitados_count, char *caminho, int total_dispositivos);

#endif