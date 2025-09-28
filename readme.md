# **Projeto: Sistema de Gerenciamento de LAN Virtual**

Este projeto implementa um sistema de console em C para simular e gerenciar a topologia de uma rede local (LAN) virtual. Ele permite cadastrar diferentes tipos de dispositivos (roteadores, switches, hosts), criar conexões entre eles, simular o envio de pacotes, verificar a conectividade e gerar relatórios estatísticos sobre a rede.

## **Como Compilar e Executar**

O projeto utiliza um makefile para simplificar a compilação. Certifique-se de ter o gcc e o make instalados.

1. **Compilar o projeto:**  
   make all

   Isso irá compilar todos os arquivos-fonte e gerar o executável main.  
2. **Executar o programa:**  
   make run

   Ou diretamente:  
   ./main

3. **Limpar arquivos compilados:**  
   make clean

## **Estrutura dos Arquivos**

* main.c: Contém a função main e a interface do menu principal para interação com o usuário.  
* src/dispositivo.c: Implementação das funções para criar, gerenciar e manipular os dispositivos da rede.  
* include/dispositivo.h: Declaração das estruturas (Dispositivo, ListaDispositivos) e dos protótipos das funções do módulo de dispositivos.  
* src/conexao.c: Implementação das funções para gerenciar as conexões, formando a topologia da rede (grafo).  
* include/conexao.h: Declaração das estruturas (Conexao, ListaAdjacencia) e protótipos do módulo de conexões.  
* src/simulacao.c: Implementação da lógica de simulação de envio de pacotes e registro de logs.  
* include/simulacao.h: Declaração das funções e estruturas relacionadas à simulação.  
* src/utils.c: Implementação de funções utilitárias para entrada/saída, estatísticas, persistência de dados e validações.  
* include/utils.h: Protótipos das funções utilitárias.

## **Documentação das Funções**

### **Módulo dispositivo.h**

Gerencia todas as operações relacionadas aos dispositivos da rede.

#### **ListaDispositivos\* criar\_lista\_dispositivos()**

* **Descrição:** Aloca memória e inicializa uma nova lista encadeada de dispositivos.  
* **Retorno:** Ponteiro para a ListaDispositivos criada ou NULL em caso de falha de alocação.

#### **int adicionar\_dispositivo(ListaDispositivos \*lista, int id, const char \*nome, TipoDispositivo tipo)**

* **Descrição:** Adiciona um novo dispositivo à lista, verificando se o ID já existe.  
* **Parâmetros:**  
  * lista: Ponteiro para a lista de dispositivos.  
  * id: ID único para o novo dispositivo.  
  * nome: Nome do dispositivo.  
  * tipo: Tipo do dispositivo (ROTEADOR, SWITCH ou HOST).  
* **Retorno:** 1 em caso de sucesso, 0 em caso de falha (ID duplicado, falha de alocação).

#### **Dispositivo\* buscar\_dispositivo(ListaDispositivos \*lista, int id)**

* **Descrição:** Procura por um dispositivo na lista com base no seu ID.  
* **Parâmetros:**  
  * lista: Ponteiro para a lista de dispositivos.  
  * id: O ID do dispositivo a ser buscado.  
* **Retorno:** Ponteiro para o Dispositivo encontrado ou NULL se não existir.

#### **void listar\_dispositivos(ListaDispositivos \*lista)**

* **Descrição:** Imprime na tela uma lista formatada de todos os dispositivos cadastrados.  
* **Parâmetros:**  
  * lista: Ponteiro para a lista de dispositivos.

#### **void alterar\_status\_dispositivo(ListaDispositivos \*lista, int id, StatusDispositivo status)**

* **Descrição:** Altera o status (ATIVO ou INATIVO) de um dispositivo específico.  
* **Parâmetros:**  
  * lista: Ponteiro para a lista de dispositivos.  
  * id: ID do dispositivo a ser modificado.  
  * status: O novo status para o dispositivo.

#### **void liberar\_lista\_dispositivos(ListaDispositivos \*lista)**

* **Descrição:** Libera toda a memória alocada para a lista de dispositivos e seus nós.  
* **Parâmetros:**  
  * lista: Ponteiro para a lista a ser liberada.

#### **const char\* tipo\_para\_string(TipoDispositivo tipo)**

* **Descrição:** Converte um valor do enum TipoDispositivo para sua representação em string.  
* **Retorno:** String correspondente ao tipo.

#### **const char\* status\_para\_string(StatusDispositivo status)**

* **Descrição:** Converte um valor do enum StatusDispositivo para sua representação em string.  
* **Retorno:** String correspondente ao status.

### **Módulo conexao.h**

Gerencia a topologia da rede, tratando as conexões como um grafo.

#### **GerenciadorConexoes\* criar\_gerenciador\_conexoes()**

* **Descrição:** Aloca memória e inicializa o gerenciador de conexões.  
* **Retorno:** Ponteiro para o GerenciadorConexoes criado ou NULL em caso de falha.

#### **int adicionar\_conexao\_validado(GerenciadorConexoes \*gerenciador, ListaDispositivos \*dispositivos, int origem\_id, int destino\_id)**

* **Descrição:** Cria uma conexão bidirecional entre dois dispositivos, validando se ambos existem na lista de dispositivos.  
* **Parâmetros:**  
  * gerenciador: Ponteiro para o gerenciador de conexões.  
  * dispositivos: Ponteiro para a lista de dispositivos (para validação).  
  * origem\_id: ID do dispositivo de origem.  
  * destino\_id: ID do dispositivo de destino.  
* **Retorno:** 1 em caso de sucesso, 0 em caso de falha.

#### **void listar\_conexoes(GerenciadorConexoes \*gerenciador, ListaDispositivos \*dispositivos)**

* **Descrição:** Imprime na tela todas as conexões existentes na rede de forma legível.  
* **Parâmetros:**  
  * gerenciador: Ponteiro para o gerenciador de conexões.  
  * dispositivos: Ponteiro para a lista de dispositivos para obter os nomes.

#### **int verificar\_conectividade(GerenciadorConexoes \*gerenciador, int origem\_id, int destino\_id, int total\_dispositivos)**

* **Descrição:** Verifica se existe um caminho entre dois dispositivos usando um algoritmo de busca em profundidade (DFS).  
* **Parâmetros:**  
  * gerenciador: Ponteiro para o gerenciador de conexões.  
  * origem\_id: ID do dispositivo de partida.  
  * destino\_id: ID do dispositivo de chegada.  
  * total\_dispositivos: Número total de dispositivos para alocação segura do array de visitados.  
* **Retorno:** 1 se houver conectividade, 0 caso contrário.

#### **int obter\_grau\_conectividade(GerenciadorConexoes \*gerenciador, int dispositivo\_id)**

* **Descrição:** Retorna o número de conexões diretas (grau) de um dispositivo.  
* **Parâmetros:**  
  * gerenciador: Ponteiro para o gerenciador de conexões.  
  * dispositivo\_id: ID do dispositivo a ser verificado.  
* **Retorno:** O número de conexões do dispositivo.

#### **void liberar\_gerenciador\_conexoes(GerenciadorConexoes \*gerenciador)**

* **Descrição:** Libera toda a memória associada ao gerenciador de conexões.  
* **Parâmetros:**  
  * gerenciador: Ponteiro para o gerenciador a ser liberado.

### **Módulo simulacao.h**

Responsável pela lógica de simulação e registro de eventos.

#### **int simular\_envio\_pacote(ListaDispositivos \*dispositivos, GerenciadorConexoes \*gerenciador, int origem\_id, int destino\_id, const char \*dados)**

* **Descrição:** Simula o envio de um pacote de um dispositivo de origem para um de destino. Verifica se os dispositivos estão ativos e se há um caminho entre eles, e então encontra e exibe o caminho.  
* **Parâmetros:**  
  * dispositivos: Ponteiro para a lista de dispositivos.  
  * gerenciador: Ponteiro para o gerenciador de conexões.  
  * origem\_id: ID do dispositivo remetente.  
  * destino\_id: ID do dispositivo destinatário.  
  * dados: Conteúdo do pacote a ser enviado.  
* **Retorno:** 1 se o envio for bem-sucedido, 0 em caso de falha.

#### **void registrar\_log(const char \*mensagem)**

* **Descrição:** Registra uma mensagem de evento (geralmente de sucesso) no arquivo log.txt com data e hora.  
* **Parâmetros:**  
  * mensagem: A mensagem a ser registrada.

#### **void registrar\_falha(const char \*mensagem)**

* **Descrição:** Registra uma mensagem de falha no arquivo log.txt com data e hora.  
* **Parâmetros:**  
  * mensagem: A mensagem de falha a ser registrada.

### **Módulo utils.h**

Agrupa funções auxiliares para diversas finalidades.

#### **void limpar\_buffer()**

* **Descrição:** Limpa o buffer de entrada do teclado (stdin) para evitar leituras incorretas.

#### **int ler\_inteiro(const char \*prompt)**

* **Descrição:** Exibe uma mensagem e lê um número inteiro do usuário com tratamento de erro.  
* **Retorno:** O inteiro lido com sucesso.

#### **void ler\_string(const char \*prompt, char \*buffer, int tamanho)**

* **Descrição:** Exibe uma mensagem e lê uma string do usuário de forma segura.

#### **void pausar\_sistema()**

* **Descrição:** Pausa a execução do programa e aguarda o usuário pressionar Enter.

#### **void gerar\_relatorio\_estatistico(ListaDispositivos \*dispositivos, GerenciadorConexoes \*gerenciador)**

* **Descrição:** Exibe um relatório completo com estatísticas da rede, como total de dispositivos por tipo e grau de conectividade.

#### **int contar\_dispositivos\_por\_tipo(ListaDispositivos \*dispositivos, TipoDispositivo tipo)**

* **Descrição:** Conta o número de dispositivos de um tipo específico.  
* **Retorno:** A quantidade de dispositivos do tipo especificado.

#### **int encontrar\_dispositivo\_mais\_conectado(ListaDispositivos \*dispositivos, GerenciadorConexoes \*gerenciador)**

* **Descrição:** Encontra o ID do dispositivo com o maior número de conexões.  
* **Retorno:** O ID do dispositivo mais conectado ou \-1 se não houver dispositivos.

#### **int salvar\_topologia(ListaDispositivos \*dispositivos, GerenciadorConexoes \*gerenciador, const char \*nome\_arquivo)**

* **Descrição:** Salva o estado atual da rede (dispositivos e conexões) em um arquivo de texto.  
* **Retorno:** 1 em caso de sucesso, 0 em caso de falha.

#### **int carregar\_topologia(ListaDispositivos \*dispositivos, GerenciadorConexoes \*gerenciador, const char \*nome\_arquivo)**

* **Descrição:** Carrega uma topologia de rede a partir de um arquivo, recriando os dispositivos e conexões.  
* **Retorno:** 1 em caso de sucesso, 0 em caso de falha.