#include "../include/dispositivo.h"

ListaDispositivos* criar_lista_dispositivos() {
    ListaDispositivos *lista = (ListaDispositivos*)malloc(sizeof(ListaDispositivos));
    if (lista) {
        lista->primeiro = NULL;
        lista->total = 0;
    }
    return lista;
}

int adicionar_dispositivo(ListaDispositivos *lista, int id, const char *nome, TipoDispositivo tipo) {
    if (!lista || !nome) return 0;
    
    // Verificar se ID já existe
    if (buscar_dispositivo(lista, id)) {
        printf("Erro: Dispositivo com ID %d já existe!\n", id);
        return 0;
    }
    
    Dispositivo *novo = (Dispositivo*)malloc(sizeof(Dispositivo));
    if (!novo) return 0;
    
    novo->id = id;
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->tipo = tipo;
    novo->status = ATIVO;
    novo->proximo = lista->primeiro;
    
    lista->primeiro = novo;
    lista->total++;
    
    return 1;
}

Dispositivo* buscar_dispositivo(ListaDispositivos *lista, int id) {
    if (!lista) return NULL;
    
    Dispositivo *atual = lista->primeiro;
    while (atual) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void listar_dispositivos(ListaDispositivos *lista) {
    if (!lista || !lista->primeiro) {
        printf("Nenhum dispositivo cadastrado.\n");
        return;
    }
    
    printf("\n--- DISPOSITIVOS CADASTRADOS ---\n");
    printf("ID\tNome\t\tTipo\t\tStatus\n");
    printf("------------------------------------------------\n");
    
    Dispositivo *atual = lista->primeiro;
    while (atual) {
        printf("%d\t%-15s\t%-10s\t%s\n", 
               atual->id, 
               atual->nome, 
               tipo_para_string(atual->tipo),
               status_para_string(atual->status));
        atual = atual->proximo;
    }
    printf("Total de dispositivos: %d\n", lista->total);
}

void alterar_status_dispositivo(ListaDispositivos *lista, int id, StatusDispositivo status) {
    Dispositivo *dispositivo = buscar_dispositivo(lista, id);
    if (dispositivo) {
        dispositivo->status = status;
        printf("Status do dispositivo %d alterado para %s\n", id, status_para_string(status));
    }
}

void liberar_lista_dispositivos(ListaDispositivos *lista) {
    if (!lista) return;
    
    Dispositivo *atual = lista->primeiro;
    while (atual) {
        Dispositivo *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(lista);
}

const char* tipo_para_string(TipoDispositivo tipo) {
    switch (tipo) {
        case ROTEADOR: return "Roteador";
        case SWITCH: return "Switch";
        case HOST: return "Host";
        default: return "Desconhecido";
    }
}

const char* status_para_string(StatusDispositivo status) {
    switch (status) {
        case ATIVO: return "Ativo";
        case INATIVO: return "Inativo";
        default: return "Desconhecido";
    }
}