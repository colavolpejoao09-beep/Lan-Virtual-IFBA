#ifndef CONEXAO_H
#define CONEXAO_H

#include "dispositivo.h"

// Estrutura para representar uma conexão entre dispositivos
typedef struct Conexao {
    int dispositivo_destino_id;
    struct Conexao *proxima;
} Conexao;

// Estrutura para lista de adjacência (conexões de um dispositivo)
typedef struct ListaAdjacencia {
    int dispositivo_id;
    Conexao *primeira_conexao;
    struct ListaAdjacencia *proxima;
} ListaAdjacencia;

// Estrutura principal para gerenciar todas as conexões
typedef struct {
    ListaAdjacencia *primeira;
} GerenciadorConexoes;

// Funções para gerenciamento de conexões
GerenciadorConexoes* criar_gerenciador_conexoes();
int adicionar_conexao(GerenciadorConexoes *gerenciador, int origem_id, int destino_id);
int adicionar_conexao_validado(GerenciadorConexoes *gerenciador, ListaDispositivos *dispositivos, int origem_id, int destino_id);
void listar_conexoes(GerenciadorConexoes *gerenciador, ListaDispositivos *dispositivos);
int verificar_conectividade_recursiva_seguro(GerenciadorConexoes *gerenciador, int origem_id, int destino_id, int *visitados_ids, int *visitados_count, int max_dispositivos);
int verificar_conectividade(GerenciadorConexoes *gerenciador, int origem_id, int destino_id, int total_dispositivos);
int obter_grau_conectividade(GerenciadorConexoes *gerenciador, int dispositivo_id);
void liberar_gerenciador_conexoes(GerenciadorConexoes *gerenciador);

#endif