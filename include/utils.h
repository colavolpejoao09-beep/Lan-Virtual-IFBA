#ifndef UTILS_H
#define UTILS_H

#include "dispositivo.h"
#include "conexao.h"

// Funções utilitárias
void limpar_buffer();
int ler_inteiro(const char *prompt);
void ler_string(const char *prompt, char *buffer, int tamanho);
void pausar_sistema();

// Funções de estatísticas
void gerar_relatorio_estatistico(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador);
int contar_dispositivos_por_tipo(ListaDispositivos *dispositivos, TipoDispositivo tipo);
int encontrar_dispositivo_mais_conectado(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador);

// Funções de persistência
int salvar_topologia(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador, const char *nome_arquivo);
int carregar_topologia(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador, const char *nome_arquivo);

// Funções de validação
int validar_ponteiro(void *ptr, const char *nome);
int validar_id_dispositivo(int id);

#endif