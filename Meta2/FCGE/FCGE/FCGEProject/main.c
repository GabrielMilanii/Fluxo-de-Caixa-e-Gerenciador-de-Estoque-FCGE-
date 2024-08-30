#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define NOME_ARQUIVO "fcge.bin"
#define MAX_ITENS 1024
#define MAX_TRANSACOES 1024

// ESTRUTURA QUE REGISTRA OS ITENS
typedef struct
{
    char nome[256];
    float valor_Item;
    float custoFabricacao;
    int qtd_Item;
} Item;

// ESTRUTURA QUE REGISTRA TRANSA��ES FINANCEIRAS
typedef struct
{
    float saldo;
    float valor;
    char descricao[100];
    int mes;
} transacao;

// DEFINI��O DE VARI�VEIS GLOBAIS
int contagem_estoque = 0;
Item estoque[MAX_ITENS];
transacao transacoes[MAX_TRANSACOES];
int contagem_transacao = 0;
float saldo_atual = 0;

// FUN��O PARA SALVAR DADOS
void salvarDados()
{
    FILE *arquivo = fopen(NOME_ARQUIVO, "wb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }

    fwrite(&contagem_estoque, sizeof(int), 1, arquivo);
    fwrite(estoque, sizeof(Item), contagem_estoque, arquivo);

    fwrite(&contagem_transacao, sizeof(int), 1, arquivo);
    fwrite(transacoes, sizeof(transacao), contagem_transacao, arquivo);

    fwrite(&saldo_atual, sizeof(float), 1, arquivo);

    fclose(arquivo);
}

// FUN��O PARA CARREGAR DADOS
void carregarDados()
{
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }

    fread(&contagem_estoque, sizeof(int), 1, arquivo);
    fread(estoque, sizeof(Item), contagem_estoque, arquivo);

    fread(&contagem_transacao, sizeof(int), 1, arquivo);
    fread(transacoes, sizeof(transacao), contagem_transacao, arquivo);

    fread(&saldo_atual, sizeof(float), 1, arquivo);

    fclose(arquivo);
}

// FUN��O PARA ADICIONAR ITENS AO ESTOQUE
void adicionarItem()
{
//CASO O ESTOQUE ESTEJA CHEIO IMPRIME TAL MENSAGEM
    system("CLS");
    if (contagem_estoque >= MAX_ITENS)
    {
        printf("Estoque cheio!\n");
        return;
    }
//ADICIONA O ITEM
    Item novoItem;
    printf("Digite o nome do Item: ");
    scanf(" %[^\n]", novoItem.nome);
    printf("Digite o valor do Item: ");
    scanf("%f", &novoItem.valor_Item);
    printf("Digite seu custo de Fabrica��o: ");
    scanf("%f", &novoItem.custoFabricacao);
    printf("Digite a quantidade de itens que ser�o registrados: ");
    scanf("%d", &novoItem.qtd_Item);

    estoque[contagem_estoque] = novoItem;
    contagem_estoque++;
    printf("\nSucesso ao adicionar item!\n");

    salvarDados();  // Salva os dados ap�s adicionar item

    system("pause");
}

// FUN��O PARA REMOVER ALGUM ITEM DO ESTOQUE
void removerItem()
{
//PERGUNTA O NOME DO ITEM QUE DESEJA REMOVER
    system("CLS");
    char nome[256];
    printf("Digite o nome do item que deseja remover: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < contagem_estoque; i++)
    {
        if (strcmp(estoque[i].nome, nome) == 0)
        {
            for (int j = i; j < contagem_estoque - 1; j++)
            {
                estoque[j] = estoque[j + 1];
            }
            contagem_estoque--;
            printf("Item removido com sucesso!\n");
            salvarDados();  //SALVA OS DADOS AP�S REMOVER O ITEM
            system("pause");
            return;
        }
    }
    printf("Item n�o encontrado!\n");

    system("pause");
}

// FUN��O QUE LISTA OS ITENS REGISTRADOS

void listarItem() {
//IMPRIME OS ITENS REGISTRADOS

    system("CLS");
    printf("Itens no estoque: \n");
    printf("Valor do Item  |  Custo de Fabrica��o  |  Quantidade de Itens  |   Lucro   |  Nome do Produto\n");
    for (int i = 0; i < contagem_estoque; i++) {
        float lucro = estoque[i].valor_Item - estoque[i].custoFabricacao;

        printf("R$%-12.2f | R$%-19.2f | %-21d | R$%-7.2f | %s\n",
               estoque[i].valor_Item, estoque[i].custoFabricacao, estoque[i].qtd_Item, lucro, estoque[i].nome);
    }

    system("pause");
}


// FUN��O QUE REGISTRAR� A ENTRADA E SA�DA DE DINHEIRO
void adicionar_transacao()
{
//ADICIONA A TRANSA��O

    system("CLS");
    transacao novaTransacao;
    printf("Digite o valor da transa��o(Use + para adicionar e - para subtrair): ");
    scanf("%f", &novaTransacao.valor);
    printf("Digite o m�s da transa��o: ");
    scanf("%d", &novaTransacao.mes);
    printf("Digite a descri��o da transa��o: ");
    scanf(" %[^\n]", novaTransacao.descricao);

    novaTransacao.saldo = saldo_atual + novaTransacao.valor;
    saldo_atual = novaTransacao.saldo;//ATUALIZA O SALDO

    transacoes[contagem_transacao] = novaTransacao;
    contagem_transacao++;

    salvarDados();  //SALVA OS DADOS

    system("pause");
}

void remover_transacao()
{
    system("CLS");
    char nome[256];
    printf("Digite a descri��o da transa��o que deseja remover: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < contagem_transacao; i++)
    {
        if (strcmp(transacoes[i].descricao, nome) == 0)
        {
            for (int j = i; j < contagem_transacao - 1; j++)
            {
                transacoes[j] = transacoes[j + 1];
            }
            contagem_transacao--;
            printf("Transa��o removida com sucesso!\n");
            salvarDados();  // Salva os dados ap�s remover item
            system("pause");
            return;

        }
    }
    printf("Transa��o n�o encontrada!\n");

    system("pause");
}

// FUN��O PARA EXIBIR O HIST�RICO DE TRANSA��ES COM BASE NO M�S

void historicoTransacoes()
{
//PERGUNTA O M�S DO HIST�RICO QUE DESEJA VER

    system("CLS");
    int mes;
    printf("Digite de qual m�s deseja ver o hist�rico de transa��o: ");
    scanf("%d", &mes);

//IMPRIME O HIST�RICO DE TRANSA��O DO MES SELECIONADO

    printf("Hist�rico de transa��o: %d", mes);
    printf("\nValor da transa��o | Descri��o da transa��o\n");
    for (int i = 0; i < contagem_transacao; i++)
    {
        if (transacoes[i].mes == mes)
        {
            printf("R$%-18.2f %-10s\n", transacoes[i].valor, transacoes[i].descricao);
        }
    }

    system("pause");
}

// FUN��O PARA VER O SALDO

void verSaldo()
{
    system("CLS");
    printf("Seu saldo atual: R$%.2f\n", saldo_atual);

    system("pause");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    int opcao;

// CARREGA OS DADOS ANTES DE TUDO

    carregarDados();

//FA�A A EXIBI��O DO MENU
    do
    {


        printf("MENU\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item do estoque\n");
        printf("3. Mostrar itens no estoque\n");
        printf("4. Adicionar movimenta��o financeira\n");
        printf("5. Remover movimenta��o financeira\n");
        printf("6. Hist�rico de transa��es\n");
        printf("7. Saldo atual\n");
        printf("8. Sair\n");

        printf("Escolha uma op��o acima: ");
        scanf("%d", &opcao);

//V� A OP��O E EXIBE A FUN��O ESCOLHIDA

        switch (opcao)
        {
        case 1:
            adicionarItem();
            break;
        case 2:
            removerItem();
            break;
        case 3:
            listarItem();
            break;
        case 4:
            adicionar_transacao();
            break;
        case 5:
            remover_transacao();
            break;
        case 6:
            historicoTransacoes();
            break;
        case 7:
            verSaldo();
        case 8:
            printf("Saindo!\n");
            break;
        default:
            printf("Op��o inv�lida... Tente novamente.\n");
            break;
        }

    }
//ENQUANTO A OP��O ESCOLHIDA FOR DIFERENTE DE 8
    while (opcao != 8);

    return 0;
}
