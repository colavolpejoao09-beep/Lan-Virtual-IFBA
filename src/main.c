#include <stdio.h>
#include "../include/dispositivo.h"
#include "../include/conexao.h"
#include "../include/simulacao.h"
#include "../include/utils.h"

void exibir_menu() {
    printf("\n=== SISTEMA DE GERENCIAMENTO DE LAN VIRTUAL ===\n");
    printf("1. Cadastrar Dispositivo\n");
    printf("2. Listar Dispositivos\n");
    printf("3. Adicionar Conexão\n");
    printf("4. Listar Conexões\n");
    printf("5. Simular Envio de Pacote\n");
    printf("6. Verificar Conectividade\n");
    printf("7. Alterar Status de Dispositivo\n");
    printf("8. Gerar Relatório Estatístico\n");
    printf("9. Salvar Topologia\n");
    printf("10. Carregar Topologia\n");
    printf("0. Sair\n");
    printf("===============================================\n");
}

int main() {
    printf("Iniciando Sistema de Gerenciamento de LAN Virtual...\n");
    
    // Inicializar estruturas principais
    ListaDispositivos *dispositivos = criar_lista_dispositivos();
    GerenciadorConexoes *gerenciador = criar_gerenciador_conexoes();
    
    if (!dispositivos || !gerenciador) {
        printf("Erro: Falha ao inicializar o sistema!\n");
        return 1;
    }
    
    printf("Sistema inicializado com sucesso!\n");
    
    int opcao;
    do {
        exibir_menu();
        opcao = ler_inteiro("Escolha uma opção: ");
        
        switch (opcao) {
            case 1: {
                printf("\n--- CADASTRAR DISPOSITIVO ---\n");
                int id = ler_inteiro("ID do dispositivo: ");
                char nome[50];
                ler_string("Nome do dispositivo: ", nome, sizeof(nome));
                
                printf("Tipos disponíveis:\n");
                printf("0 - Roteador\n1 - Switch\n2 - Host\n");
                int tipo = ler_inteiro("Tipo do dispositivo: ");
                
                if (tipo < 0 || tipo > 2) {
                    printf("Tipo inválido!\n");
                    break;
                }
                
                if (adicionar_dispositivo(dispositivos, id, nome, (TipoDispositivo)tipo)) {
                    printf("Dispositivo cadastrado com sucesso!\n");
                } else {
                    printf("Erro ao cadastrar dispositivo!\n");
                }
                break;
            }
            case 2:
                printf("\n--- LISTA DE DISPOSITIVOS ---\n");
                listar_dispositivos(dispositivos);
                break;
                
            case 3: {
                printf("\n--- ADICIONAR CONEXÃO ---\n");
                int origem = ler_inteiro("ID do dispositivo origem: ");
                int destino = ler_inteiro("ID do dispositivo destino: ");
                
                // Validar se os dispositivos existem
                if (!buscar_dispositivo(dispositivos, origem)) {
                    printf("Erro: Dispositivo origem %d não encontrado!\n", origem);
                    break;
                }
                
                if (!buscar_dispositivo(dispositivos, destino)) {
                    printf("Erro: Dispositivo destino %d não encontrado!\n", destino);
                    break;
                }
                
                if (origem == destino) {
                    printf("Erro: Não é possível conectar um dispositivo a ele mesmo!\n");
                    break;
                }
                
                if (adicionar_conexao_validado(gerenciador, dispositivos, origem, destino)) {
                    printf("Conexão adicionada com sucesso!\n");
                } else {
                    printf("Erro ao adicionar conexão!\n");
                }
                break;
            }
            case 4:
                printf("\n--- LISTA DE CONEXÕES ---\n");
                listar_conexoes(gerenciador, dispositivos);
                break;
                
            case 5: {
                printf("\n--- SIMULAR ENVIO DE PACOTE ---\n");
                int origem = ler_inteiro("ID do dispositivo origem: ");
                int destino = ler_inteiro("ID do dispositivo destino: ");
                
                // Validar se os dispositivos existem
                if (!buscar_dispositivo(dispositivos, origem)) {
                    printf("Erro: Dispositivo origem %d não encontrado!\n", origem);
                    break;
                }
                
                if (!buscar_dispositivo(dispositivos, destino)) {
                    printf("Erro: Dispositivo destino %d não encontrado!\n", destino);
                    break;
                }
                
                char dados[256];
                ler_string("Dados do pacote: ", dados, sizeof(dados));
                
                simular_envio_pacote(dispositivos, gerenciador, origem, destino, dados);
                break;
            }
            case 6: {
                printf("\n--- VERIFICAR CONECTIVIDADE ---\n");
                int origem = ler_inteiro("ID do dispositivo origem: ");
                int destino = ler_inteiro("ID do dispositivo destino: ");
                
                // Validar se os dispositivos existem
                if (!buscar_dispositivo(dispositivos, origem)) {
                    printf("Erro: Dispositivo origem %d não encontrado!\n", origem);
                    break;
                }
                
                if (!buscar_dispositivo(dispositivos, destino)) {
                    printf("Erro: Dispositivo destino %d não encontrado!\n", destino);
                    break;
                }
                
                int conectado = verificar_conectividade(gerenciador, origem, destino, dispositivos->total);
                if (conectado) {
                    printf("Os dispositivos %d e %d ESTÃO conectados!\n", origem, destino);
                } else {
                    printf("Os dispositivos %d e %d NÃO estão conectados!\n", origem, destino);
                }
                break;
            }
            case 7: {
                printf("\n--- ALTERAR STATUS DE DISPOSITIVO ---\n");
                int id = ler_inteiro("ID do dispositivo: ");
                
                if (!buscar_dispositivo(dispositivos, id)) {
                    printf("Erro: Dispositivo %d não encontrado!\n", id);
                    break;
                }
                
                printf("Status:\n0 - Ativo\n1 - Inativo\n");
                int status = ler_inteiro("Novo status: ");
                
                if (status < 0 || status > 1) {
                    printf("Status inválido!\n");
                    break;
                }
                
                alterar_status_dispositivo(dispositivos, id, (StatusDispositivo)status);
                break;
            }
            case 8:
                printf("\n--- RELATÓRIO ESTATÍSTICO ---\n");
                gerar_relatorio_estatistico(dispositivos, gerenciador);
                break;
                
            case 9: {
                printf("\n--- SALVAR TOPOLOGIA ---\n");
                char arquivo[100];
                ler_string("Nome do arquivo: ", arquivo, sizeof(arquivo));
                
                if (salvar_topologia(dispositivos, gerenciador, arquivo)) {
                    printf("Topologia salva com sucesso!\n");
                } else {
                    printf("Erro ao salvar topologia!\n");
                }
                break;
            }
            case 10: {
                printf("\n--- CARREGAR TOPOLOGIA ---\n");
                char arquivo[100];
                ler_string("Nome do arquivo: ", arquivo, sizeof(arquivo));
                
                if (carregar_topologia(dispositivos, gerenciador, arquivo)) {
                    printf("Topologia carregada com sucesso!\n");
                } else {
                    printf("Erro ao carregar topologia!\n");
                }
                break;
            }
            case 0:
                printf("Encerrando sistema...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
        
        if (opcao != 0) {
            pausar_sistema();
        }
        
    } while (opcao != 0);
    
    // Liberar memória
    liberar_lista_dispositivos(dispositivos);
    liberar_gerenciador_conexoes(gerenciador);
    
    printf("Sistema encerrado com sucesso!\n");
    return 0;
}