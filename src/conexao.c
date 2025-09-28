#include "../include/conexao.h"

GerenciadorConexoes* criar_gerenciador_conexoes() {
    GerenciadorConexoes *gerenciador = (GerenciadorConexoes*)malloc(sizeof(GerenciadorConexoes));
    if (gerenciador) {
        gerenciador->primeira = NULL;
    }
    return gerenciador;
}

ListaAdjacencia* buscar_ou_criar_lista_adjacencia(GerenciadorConexoes *gerenciador, int dispositivo_id) {
    if (!gerenciador) return NULL;
    
    // Buscar lista existente
    ListaAdjacencia *atual = gerenciador->primeira;
    while (atual) {
        if (atual->dispositivo_id == dispositivo_id) {
            return atual;
        }
        atual = atual->proxima;
    }
    
    // Criar nova lista se não existir
    ListaAdjacencia *nova = (ListaAdjacencia*)malloc(sizeof(ListaAdjacencia));
    if (!nova) return NULL;
    
    nova->dispositivo_id = dispositivo_id;
    nova->primeira_conexao = NULL;
    nova->proxima = gerenciador->primeira;
    gerenciador->primeira = nova;
    
    return nova;
}

// Função original para uso interno (carregar arquivos, etc.)
int adicionar_conexao(GerenciadorConexoes *gerenciador, int origem_id, int destino_id) {
    if (!gerenciador || origem_id == destino_id) return 0;
    
    // Adicionar conexão bidirecional
    
    // Origem -> Destino
    ListaAdjacencia *lista_origem = buscar_ou_criar_lista_adjacencia(gerenciador, origem_id);
    if (!lista_origem) return 0;
    
    // Verificar se conexão já existe
    Conexao *conexao_atual = lista_origem->primeira_conexao;
    while (conexao_atual) {
        if (conexao_atual->dispositivo_destino_id == destino_id) {
            return 1; // Considerar sucesso se já existe
        }
        conexao_atual = conexao_atual->proxima;
    }
    
    Conexao *nova_conexao = (Conexao*)malloc(sizeof(Conexao));
    if (!nova_conexao) return 0;
    
    nova_conexao->dispositivo_destino_id = destino_id;
    nova_conexao->proxima = lista_origem->primeira_conexao;
    lista_origem->primeira_conexao = nova_conexao;
    
    // Destino -> Origem (conexão bidirecional)
    ListaAdjacencia *lista_destino = buscar_ou_criar_lista_adjacencia(gerenciador, destino_id);
    if (!lista_destino) return 0;
    
    nova_conexao = (Conexao*)malloc(sizeof(Conexao));
    if (!nova_conexao) {
        // Limpar a primeira conexão se a segunda falhar
        Conexao *primeiro = lista_origem->primeira_conexao;
        if (primeiro && primeiro->dispositivo_destino_id == destino_id) {
            lista_origem->primeira_conexao = primeiro->proxima;
            free(primeiro);
        }
        return 0;
    }
    
    nova_conexao->dispositivo_destino_id = origem_id;
    nova_conexao->proxima = lista_destino->primeira_conexao;
    lista_destino->primeira_conexao = nova_conexao;
    
    return 1;
}

// Função validada para uso na interface
int adicionar_conexao_validado(GerenciadorConexoes *gerenciador, ListaDispositivos *dispositivos, int origem_id, int destino_id) {
    if (!gerenciador || !dispositivos || origem_id == destino_id) return 0;
    
    // Verificar se ambos os dispositivos existem
    if (!buscar_dispositivo(dispositivos, origem_id) || !buscar_dispositivo(dispositivos, destino_id)) {
        return 0;
    }
    
    // Adicionar conexão bidirecional
    
    // Origem -> Destino
    ListaAdjacencia *lista_origem = buscar_ou_criar_lista_adjacencia(gerenciador, origem_id);
    if (!lista_origem) return 0;
    
    // Verificar se conexão já existe
    Conexao *conexao_atual = lista_origem->primeira_conexao;
    while (conexao_atual) {
        if (conexao_atual->dispositivo_destino_id == destino_id) {
            printf("Conexão entre %d e %d já existe!\n", origem_id, destino_id);
            return 1; // Considerar sucesso se já existe
        }
        conexao_atual = conexao_atual->proxima;
    }
    
    Conexao *nova_conexao = (Conexao*)malloc(sizeof(Conexao));
    if (!nova_conexao) return 0;
    
    nova_conexao->dispositivo_destino_id = destino_id;
    nova_conexao->proxima = lista_origem->primeira_conexao;
    lista_origem->primeira_conexao = nova_conexao;
    
    // Destino -> Origem (conexão bidirecional)
    ListaAdjacencia *lista_destino = buscar_ou_criar_lista_adjacencia(gerenciador, destino_id);
    if (!lista_destino) return 0;
    
    nova_conexao = (Conexao*)malloc(sizeof(Conexao));
    if (!nova_conexao) return 0;
    
    nova_conexao->dispositivo_destino_id = origem_id;
    nova_conexao->proxima = lista_destino->primeira_conexao;
    lista_destino->primeira_conexao = nova_conexao;
    
    return 1;
}

void listar_conexoes(GerenciadorConexoes *gerenciador, ListaDispositivos *dispositivos) {
    if (!gerenciador || !dispositivos) {
        printf("Nenhuma conexão cadastrada.\n");
        return;
    }
    
    printf("\n--- CONEXÕES DA REDE ---\n");
    
    ListaAdjacencia *atual = gerenciador->primeira;
    int tem_conexoes = 0;
    
    while (atual) {
        Dispositivo *disp_origem = buscar_dispositivo(dispositivos, atual->dispositivo_id);
        if (disp_origem) {
            Conexao *conexao = atual->primeira_conexao;
            while (conexao) {
                Dispositivo *disp_destino = buscar_dispositivo(dispositivos, conexao->dispositivo_destino_id);
                if (disp_destino && atual->dispositivo_id < conexao->dispositivo_destino_id) {
                    // Mostrar apenas uma direção para evitar duplicatas
                    printf("%s (%d) <--> %s (%d)\n", 
                           disp_origem->nome, atual->dispositivo_id,
                           disp_destino->nome, conexao->dispositivo_destino_id);
                    tem_conexoes = 1;
                }
                conexao = conexao->proxima;
            }
        }
        atual = atual->proxima;
    }
    
    if (!tem_conexoes) {
        printf("Nenhuma conexão cadastrada.\n");
    }
}

int dispositivo_ja_visitado(int *visitados_ids, int *visitados_count, int id) {
    for (int i = 0; i < *visitados_count; i++) {
        if (visitados_ids[i] == id) return 1;
    }
    return 0;
}

void marcar_como_visitado(int *visitados_ids, int *visitados_count, int id) {
    if (!dispositivo_ja_visitado(visitados_ids, visitados_count, id)) {
        visitados_ids[*visitados_count] = id;
        (*visitados_count)++;
    }
}

int verificar_conectividade_recursiva_seguro(GerenciadorConexoes *gerenciador, int origem_id, int destino_id, int *visitados_ids, int *visitados_count, int max_dispositivos) {
    if (origem_id == destino_id) return 1;
    
    // Verificar limites do array antes de escrever
    if (*visitados_count >= max_dispositivos) {
        return 0; // Falha por overflow - interromper busca
    }
    
    marcar_como_visitado(visitados_ids, visitados_count, origem_id);
    
    ListaAdjacencia *atual = gerenciador->primeira;
    while (atual) {
        if (atual->dispositivo_id == origem_id) {
            Conexao *conexao = atual->primeira_conexao;
            while (conexao) {
                int vizinho_id = conexao->dispositivo_destino_id;
                if (!dispositivo_ja_visitado(visitados_ids, visitados_count, vizinho_id)) {
                    if (verificar_conectividade_recursiva_seguro(gerenciador, vizinho_id, destino_id, visitados_ids, visitados_count, max_dispositivos)) {
                        return 1;
                    }
                }
                conexao = conexao->proxima;
            }
            break;
        }
        atual = atual->proxima;
    }
    
    return 0;
}

int verificar_conectividade(GerenciadorConexoes *gerenciador, int origem_id, int destino_id, int total_dispositivos) {
    if (!gerenciador) return 0;
    
    // Array seguro para armazenar IDs visitados (não indexado por ID)
    int *visitados_ids = (int*)malloc(total_dispositivos * sizeof(int));
    if (!visitados_ids) return 0;
    
    int visitados_count = 0;
    int resultado = verificar_conectividade_recursiva_seguro(gerenciador, origem_id, destino_id, visitados_ids, &visitados_count, total_dispositivos);
    
    free(visitados_ids);
    return resultado;
}

int obter_grau_conectividade(GerenciadorConexoes *gerenciador, int dispositivo_id) {
    if (!gerenciador) return 0;
    
    ListaAdjacencia *atual = gerenciador->primeira;
    while (atual) {
        if (atual->dispositivo_id == dispositivo_id) {
            int grau = 0;
            Conexao *conexao = atual->primeira_conexao;
            while (conexao) {
                grau++;
                conexao = conexao->proxima;
            }
            return grau;
        }
        atual = atual->proxima;
    }
    return 0;
}

void liberar_gerenciador_conexoes(GerenciadorConexoes *gerenciador) {
    if (!gerenciador) return;
    
    ListaAdjacencia *atual = gerenciador->primeira;
    while (atual) {
        Conexao *conexao = atual->primeira_conexao;
        while (conexao) {
            Conexao *temp_conexao = conexao;
            conexao = conexao->proxima;
            free(temp_conexao);
        }
        
        ListaAdjacencia *temp_lista = atual;
        atual = atual->proxima;
        free(temp_lista);
    }
    
    free(gerenciador);
}