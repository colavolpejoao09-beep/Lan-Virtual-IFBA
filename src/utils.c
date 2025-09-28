#include "../include/utils.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_inteiro(const char *prompt) {
    int valor;
    printf("%s", prompt);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida! Digite um número: ");
        limpar_buffer();
    }
    limpar_buffer();
    return valor;
}

void ler_string(const char *prompt, char *buffer, int tamanho) {
    printf("%s", prompt);
    if (fgets(buffer, tamanho, stdin) != NULL) {
        // Remover quebra de linha se existir
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void pausar_sistema() {
    printf("\nPressione Enter para continuar...");
    getchar();
}

void gerar_relatorio_estatistico(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador) {
    if (!dispositivos || !gerenciador) {
        printf("Erro: Dados inválidos para relatório!\n");
        return;
    }
    
    printf("\n=== RELATÓRIO ESTATÍSTICO DA REDE ===\n");
    
    // Contar dispositivos por tipo
    int roteadores = contar_dispositivos_por_tipo(dispositivos, ROTEADOR);
    int switches = contar_dispositivos_por_tipo(dispositivos, SWITCH);
    int hosts = contar_dispositivos_por_tipo(dispositivos, HOST);
    
    printf("Total de dispositivos: %d\n", dispositivos->total);
    printf("  - Roteadores: %d\n", roteadores);
    printf("  - Switches: %d\n", switches);
    printf("  - Hosts: %d\n", hosts);
    
    // Dispositivo mais conectado
    int mais_conectado_id = encontrar_dispositivo_mais_conectado(dispositivos, gerenciador);
    if (mais_conectado_id != -1) {
        Dispositivo *mais_conectado = buscar_dispositivo(dispositivos, mais_conectado_id);
        int grau = obter_grau_conectividade(gerenciador, mais_conectado_id);
        printf("Dispositivo mais conectado: %s (%d) com %d conexões\n", 
               mais_conectado->nome, mais_conectado_id, grau);
    }
    
    // Grau de conectividade de cada dispositivo
    printf("\nGrau de conectividade por dispositivo:\n");
    Dispositivo *atual = dispositivos->primeiro;
    while (atual) {
        int grau = obter_grau_conectividade(gerenciador, atual->id);
        printf("  %s (%d): %d conexões\n", atual->nome, atual->id, grau);
        atual = atual->proximo;
    }
}

int contar_dispositivos_por_tipo(ListaDispositivos *dispositivos, TipoDispositivo tipo) {
    if (!dispositivos) return 0;
    
    int contador = 0;
    Dispositivo *atual = dispositivos->primeiro;
    while (atual) {
        if (atual->tipo == tipo) {
            contador++;
        }
        atual = atual->proximo;
    }
    return contador;
}

int encontrar_dispositivo_mais_conectado(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador) {
    if (!dispositivos || !gerenciador) return -1;
    
    int max_grau = -1;
    int dispositivo_id = -1;
    
    Dispositivo *atual = dispositivos->primeiro;
    while (atual) {
        int grau = obter_grau_conectividade(gerenciador, atual->id);
        if (grau > max_grau) {
            max_grau = grau;
            dispositivo_id = atual->id;
        }
        atual = atual->proximo;
    }
    
    return dispositivo_id;
}

int salvar_topologia(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador, const char *nome_arquivo) {
    if (!dispositivos || !gerenciador || !nome_arquivo) return 0;
    
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) return 0;
    
    // Salvar dispositivos
    fprintf(arquivo, "DISPOSITIVOS\n");
    Dispositivo *disp = dispositivos->primeiro;
    while (disp) {
        fprintf(arquivo, "%d,%s,%d,%d\n", disp->id, disp->nome, disp->tipo, disp->status);
        disp = disp->proximo;
    }
    
    // Salvar conexões
    fprintf(arquivo, "CONEXOES\n");
    ListaAdjacencia *adj = gerenciador->primeira;
    while (adj) {
        Conexao *conn = adj->primeira_conexao;
        while (conn) {
            if (adj->dispositivo_id < conn->dispositivo_destino_id) {
                // Salvar apenas uma direção para evitar duplicatas
                fprintf(arquivo, "%d,%d\n", adj->dispositivo_id, conn->dispositivo_destino_id);
            }
            conn = conn->proxima;
        }
        adj = adj->proxima;
    }
    
    fclose(arquivo);
    return 1;
}

int carregar_topologia(ListaDispositivos *dispositivos, GerenciadorConexoes *gerenciador, const char *nome_arquivo) {
    if (!dispositivos || !gerenciador || !nome_arquivo) return 0;
    
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return 0;
    
    char linha[256];
    int modo = 0; // 0 = dispositivos, 1 = conexões
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0; // Remover quebra de linha
        
        if (strcmp(linha, "DISPOSITIVOS") == 0) {
            modo = 0;
            continue;
        }
        if (strcmp(linha, "CONEXOES") == 0) {
            modo = 1;
            continue;
        }
        
        if (modo == 0) {
            // Carregar dispositivo
            int id, tipo, status;
            char nome[50];
            if (sscanf(linha, "%d,%49[^,],%d,%d", &id, nome, &tipo, &status) == 4) {
                adicionar_dispositivo(dispositivos, id, nome, (TipoDispositivo)tipo);
                if (status == INATIVO) {
                    alterar_status_dispositivo(dispositivos, id, INATIVO);
                }
            }
        } else if (modo == 1) {
            // Carregar conexão
            int origem, destino;
            if (sscanf(linha, "%d,%d", &origem, &destino) == 2) {
                adicionar_conexao(gerenciador, origem, destino);
            }
        }
    }
    
    fclose(arquivo);
    return 1;
}

int validar_ponteiro(void *ptr, const char *nome) {
    if (!ptr) {
        printf("Erro: %s é nulo!\n", nome);
        return 0;
    }
    return 1;
}

int validar_id_dispositivo(int id) {
    if (id < 0) {
        printf("Erro: ID do dispositivo deve ser positivo!\n");
        return 0;
    }
    return 1;
}