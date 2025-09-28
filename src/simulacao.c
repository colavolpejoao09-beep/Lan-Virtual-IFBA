#include "../include/simulacao.h"
#include <time.h>

int simular_envio_pacote(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador, int origem_id, int destino_id, const char *dados) {
    if (!dispositivos || !gerenciador || !dados) return 0;
    
    Dispositivo *origem = buscar_dispositivo(dispositivos, origem_id);
    Dispositivo *destino = buscar_dispositivo(dispositivos, destino_id);
    
    if (!origem || !destino) {
        printf("Erro: Dispositivo origem ou destino não encontrado!\n");
        return 0;
    }
    
    if (origem->status == INATIVO) {
        char log_msg[512];
        snprintf(log_msg, sizeof(log_msg), "FALHA: Dispositivo origem %d (%s) está inativo", origem_id, origem->nome);
        registrar_falha(log_msg);
        printf("Erro: Dispositivo origem está inativo!\n");
        return 0;
    }
    
    if (destino->status == INATIVO) {
        char log_msg[512];
        snprintf(log_msg, sizeof(log_msg), "FALHA: Dispositivo destino %d (%s) está inativo", destino_id, destino->nome);
        registrar_falha(log_msg);
        printf("Erro: Dispositivo destino está inativo!\n");
        return 0;
    }
    
    // Verificar conectividade
    if (!verificar_conectividade(gerenciador, origem_id, destino_id, dispositivos->total)) {
        char log_msg[512];
        snprintf(log_msg, sizeof(log_msg), "FALHA: Sem conectividade entre %d (%s) e %d (%s)", 
                origem_id, origem->nome, destino_id, destino->nome);
        registrar_falha(log_msg);
        printf("Erro: Não há conectividade entre os dispositivos!\n");
        return 0;
    }
    
    // Simular encontrar caminho usando estrutura segura
    int *visitados_ids = (int*)malloc(dispositivos->total * sizeof(int));
    if (!visitados_ids) return 0;
    
    int visitados_count = 0;
    char caminho[512] = "";
    
    if (encontrar_caminho_recursivo_seguro(gerenciador, dispositivos, origem_id, destino_id, visitados_ids, &visitados_count, caminho, dispositivos->total)) {
        printf("\n=== SIMULAÇÃO DE ENVIO DE PACOTE ===\n");
        printf("Origem: %s (%d)\n", origem->nome, origem_id);
        printf("Destino: %s (%d)\n", destino->nome, destino_id);
        printf("Dados: %s\n", dados);
        printf("Caminho: %s\n", caminho);
        
        // Registrar no log
        char log_msg[1024];
        snprintf(log_msg, sizeof(log_msg), "SUCESSO: Pacote enviado de %d (%s) para %d (%s) - Caminho: %s - Dados: %s", 
                origem_id, origem->nome, destino_id, destino->nome, caminho, dados);
        registrar_log(log_msg);
        
        printf("Pacote enviado com sucesso!\n");
        free(visitados_ids);
        return 1;
    }
    
    free(visitados_ids);
    return 0;
}

int dispositivo_visitado_caminho(int *visitados_ids, int visitados_count, int id) {
    for (int i = 0; i < visitados_count; i++) {
        if (visitados_ids[i] == id) return 1;
    }
    return 0;
}

int encontrar_caminho_recursivo_seguro(GerenciadorConexoes *gerenciador, ListaDispositivos *dispositivos, int atual_id, int destino_id, int *visitados_ids, int *visitados_count, char *caminho, int total_dispositivos) {
    // Verificar limites do array antes de escrever
    if (*visitados_count >= total_dispositivos) {
        return 0; // Falha por overflow - interromper busca
    }
    
    visitados_ids[*visitados_count] = atual_id;
    (*visitados_count)++;
    
    // Adicionar dispositivo atual ao caminho (seguro)
    Dispositivo *atual = buscar_dispositivo(dispositivos, atual_id);
    if (atual) {
        int len_atual = strlen(caminho);
        int espaco_restante = 512 - len_atual - 1;
        
        if (len_atual > 0 && espaco_restante > 4) {
            strncat(caminho, " -> ", espaco_restante);
            espaco_restante -= 4;
        }
        
        if (espaco_restante > strlen(atual->nome)) {
            strncat(caminho, atual->nome, espaco_restante);
        }
    }
    
    if (atual_id == destino_id) {
        return 1;
    }
    
    // Verificar se dispositivo atual está ativo
    if (atual && atual->status == INATIVO) {
        return 0;
    }
    
    ListaAdjacencia *lista = gerenciador->primeira;
    while (lista) {
        if (lista->dispositivo_id == atual_id) {
            Conexao *conexao = lista->primeira_conexao;
            while (conexao) {
                int vizinho_id = conexao->dispositivo_destino_id;
                if (!dispositivo_visitado_caminho(visitados_ids, *visitados_count, vizinho_id)) {
                    char caminho_temp[512];
                    strncpy(caminho_temp, caminho, sizeof(caminho_temp) - 1);
                    caminho_temp[sizeof(caminho_temp) - 1] = '\0';
                    
                    int visitados_backup = *visitados_count;
                    
                    if (encontrar_caminho_recursivo_seguro(gerenciador, dispositivos, vizinho_id, destino_id, visitados_ids, visitados_count, caminho, total_dispositivos)) {
                        return 1;
                    }
                    
                    // Restaurar estado se não funcionou
                    *visitados_count = visitados_backup;
                    
                    strncpy(caminho, caminho_temp, 512 - 1);
                    caminho[511] = '\0';
                }
                conexao = conexao->proxima;
            }
            break;
        }
        lista = lista->proxima;
    }
    
    // Remover da lista de visitados (backtrack)
    if (*visitados_count > 0) {
        (*visitados_count)--;
    }
    return 0;
}

void registrar_log(const char *mensagem) {
    FILE *arquivo = fopen("log.txt", "a");
    if (!arquivo) return;
    
    time_t agora = time(NULL);
    char *timestamp = ctime(&agora);
    timestamp[strlen(timestamp) - 1] = '\0'; // Remover \n
    
    fprintf(arquivo, "[%s] %s\n", timestamp, mensagem);
    fclose(arquivo);
}

void registrar_falha(const char *mensagem) {
    FILE *arquivo = fopen("log.txt", "a");
    if (!arquivo) return;
    
    time_t agora = time(NULL);
    char *timestamp = ctime(&agora);
    timestamp[strlen(timestamp) - 1] = '\0'; // Remover \n
    
    fprintf(arquivo, "[%s] FALHA: %s\n", timestamp, mensagem);
    fclose(arquivo);
    
    printf("Falha registrada no log: %s\n", mensagem);
}