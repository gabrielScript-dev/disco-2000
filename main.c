#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct
{
    char artista[50];
    char album[50];
    char genero[20];
    int ano;
} Vinil;

typedef struct No
{
    Vinil dados;
    struct No* proximo;
} No;

typedef struct
{
    No* cabeca;
    int tamanho;
} ListaEncadeada;

No* criarNo(Vinil dados)
{
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL)
    {
        printf("Erro ao alocar memória para o nó.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->dados = dados;
    novoNo->proximo = NULL;
    return novoNo;
}

void inicializarLista(ListaEncadeada* lista)
{
    lista->cabeca = NULL;
    lista->tamanho = 0;
}

void inserirNo(ListaEncadeada* lista, Vinil dados)
{

    No* novoNo = criarNo(dados);
    novoNo->proximo = lista->cabeca;
    lista->cabeca = novoNo;
    lista->tamanho++;
}

No* buscarNo(ListaEncadeada* lista, char nomeArtista[50], char nomeAlbum[50])
{
    No* atual = lista->cabeca;

    while (atual != NULL)
    {
        if (strcmp(atual->dados.artista, nomeArtista) == 0 &&
                strcmp(atual->dados.album, nomeAlbum) == 0)
        {
            return atual; // Nó encontrado
        }
        atual = atual->proximo;
    }
    return NULL; // Nó não encontrado
}

void excluirNo(ListaEncadeada* lista, char nomeArtista[50], char nomeAlbum[50])
{
    No* atual = lista->cabeca;
    No* anterior = NULL;

    while (atual != NULL && (strcmp(atual->dados.artista, nomeArtista) != 0 ||
                             strcmp(atual->dados.album, nomeAlbum) != 0))
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != NULL)
    {
        if (anterior != NULL)
        {
            anterior->proximo = atual->proximo;
        }
        else
        {
            lista->cabeca = atual->proximo;
        }
        free(atual);
        lista->tamanho--;
        printf("Vinil excluído com sucesso!\n");
    }
    else
    {
        printf("Vinil não encontrado na lista.\n");
    }
}

void atualizarNo(ListaEncadeada* lista, char nomeArtista[50], char nomeAlbum[50], Vinil novosDados)
{

    No* noParaAtualizar = buscarNo(lista, nomeArtista, nomeAlbum);
    if (noParaAtualizar != NULL)
    {
        noParaAtualizar->dados = novosDados;
        printf("Dados do vinil atualizados com sucesso!\n");
    }
    else
    {
        printf("Vinil não encontrado na lista.\n");
    }
}


void imprimirVinil(No* vinil)
{
    printf("Artista: %s\n", vinil->dados.artista);
    printf("Álbum: %s\n", vinil->dados.album);
    printf("Gênero: %s\n", vinil->dados.genero);
    printf("Ano: %d\n", vinil->dados.ano);
}

void exibirLista(ListaEncadeada* lista)
{
    No* atual = lista->cabeca;
    while (atual != NULL)
    {
        printf("Artista: %s\n", atual->dados.artista);
        printf("Álbum: %s\n", atual->dados.album);
        printf("Gênero: %s\n", atual->dados.genero);
        printf("Ano: %d\n--\n\n", atual->dados.ano);
        atual = atual->proximo;
    }
}

void liberarLista(ListaEncadeada* lista)
{
    No* atual = lista->cabeca;
    No* proximo;

    while (atual != NULL)
    {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    lista->cabeca = NULL;
}

void gravarRegistro(ListaEncadeada* lista)
{
    FILE * registro = fopen("temp.bin", "ab");

    No* atual = lista->cabeca;
    while (atual != NULL)
    {

        fwrite(atual, sizeof(Vinil), 1, registro);
        atual = atual->proximo;
    }

    fclose(registro);
    remove("registro.bin");
    rename("temp.bin", "registro.bin");
}

void abrirRegistro(ListaEncadeada* lista)
{

    Vinil vinil;

    FILE * registro = fopen("registro.bin", "rb");

    if(registro != NULL)
    {

        while(fread(&vinil, sizeof(Vinil), 1, registro) == 1)
        {
            inserirNo(lista, vinil);
        }

    }

    fclose(registro);
}

void cabecalho()
{

    printf(" \t\t\t _____  _              ___   ___   ___   ___  \n");
    printf("\t\t\t|  __ \\(_)            |__ \\ / _ \\ / _ \\ / _ \\ \n");
    printf("\t\t\t| |  | |_ ___  ___ ___   ) | | | | | | | | | |\n");
    printf("\t\t\t| |  | | / __|/ __/ _ \\ / /| | | | | | | | | |\n");
    printf("\t\t\t| |__| | \\__ \\ (_| (_) / /_| |_| | |_| | |_| |\n");
    printf("\t\t\t|_____/|_|___/\\___\\___/____|\\___/ \\___/ \\___/ \n");
    printf("\t\t\t==============================================\n\n");

}

int main()
{

    setlocale(LC_ALL, "Portuguese_Brazil");
    system("mode con: lines=40 cols=100");



    ListaEncadeada listaVinis;
    inicializarLista(&listaVinis);

    abrirRegistro(&listaVinis);

    int opcao;
    char nomeArtista[50], nomeAlbum[50];
    Vinil novosDados;

    do
    {

        cabecalho();

        printf("\n\t\t\t\t\t\tMenu:\n\n");
        printf("\t\t\t\t\t1. Inserir Vinil\n");
        printf("\t\t\t\t\t2. Buscar Vinil\n");
        printf("\t\t\t\t\t3. Atualizar Vinil\n");
        printf("\t\t\t\t\t4. Excluir Vinil\n");
        printf("\t\t\t\t\t5. Exibir Lista\n");
        printf("\t\t\t\t\t0. Sair\n");

        fflush(stdin);
        printf("\n\n\t\t\t\t\tEscolha uma opção: ");
        scanf("%d", &opcao);


        system("cls");

        switch (opcao)
        {
        case 1:
            printf("Informe os dados do Vinil:\n");

            do {

                fflush(stdin);
                printf("Artista: ");
                gets(novosDados.artista);

                if(strlen(novosDados.artista) == 0) {
                    printf("Nome de artista inválido! Preencha o campo, por favor!\n\n");
                } else {
                    break;
                }
            } while(1);


            do {
                fflush(stdin);
                printf("Álbum: ");
                gets(novosDados.album);

                if(strlen(novosDados.artista) == 0) {
                    printf("Nome de álbum inválido! Preencha o campo, por favor!\n\n");
                } else {
                    break;
                }
            } while(1);


            fflush(stdin);
            printf("Gênero: ");
            gets(novosDados.genero);

            fflush(stdin);
            printf("Ano: ");
            scanf("%d", &novosDados.ano);

            inserirNo(&listaVinis, novosDados);

            break;
        case 2:
            printf("Tamanho: %d\n", listaVinis.tamanho);
            printf("Informe o Artista e o Álbum para buscar:\n");

            fflush(stdin);
            printf("Artista: ");
            gets(nomeArtista);

            fflush(stdin);
            printf("Álbum: ");
            gets(nomeAlbum);

            No* noBuscado = buscarNo(&listaVinis, nomeArtista, nomeAlbum);

            if (noBuscado != NULL)
            {
                printf("Vinil encontrado:\n");
                imprimirVinil(noBuscado);
            }
            else
            {
                printf("Vinil não encontrado na lista.\n");
            }

            break;
        case 3:
            printf("Informe o Artista e o Álbum para atualizar:\n");

            fflush(stdin);
            printf("Artista: ");
            gets(nomeArtista);

            fflush(stdin);
            printf("Álbum: ");
            gets(nomeAlbum);

            No* no = buscarNo(&listaVinis, nomeArtista, nomeAlbum);

            if(no != NULL)
            {

                system("cls");

                imprimirVinil(no);

                printf("Informe os novos dados do Vinil:\n");

                fflush(stdin);
                printf("Artista: ");
                gets(novosDados.artista);

                fflush(stdin);
                printf("Álbum: ");
                gets(novosDados.album);

                fflush(stdin);
                printf("Gênero: ");
                gets(novosDados.genero);

                fflush(stdin);
                printf("Ano: ");
                scanf("%d", &novosDados.ano);

                atualizarNo(&listaVinis, nomeArtista, nomeAlbum, novosDados);
            }
            break;
        case 4:
            printf("Informe o Artista e o Álbum para excluir:\n");

            fflush(stdin);
            printf("Artista: ");
            gets(nomeArtista);

            fflush(stdin);
            printf("Álbum: ");
            gets(nomeAlbum);

            excluirNo(&listaVinis, nomeArtista, nomeAlbum);
            break;
        case 5:
            printf("Lista de discos:\n\n");
            exibirLista(&listaVinis);
            system("pause");
            break;
        case 0:
            printf("Saindo do programa. Liberando memória...\n");
            system("pause");
            break;
        default:
            printf("\t\t\t\tOpção inválida. Tente novamente.\n\n");
            system("pause");
        }

        system("cls");
    }
    while (opcao != 0);

    // Liberar a memória alocada para a lista antes de encerrar o programa
    gravarRegistro(&listaVinis);
    liberarLista(&listaVinis);

    return 0;
}
