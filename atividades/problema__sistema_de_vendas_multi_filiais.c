#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILIAIS 50
#define MAX_PRODUTOS 100
#define TAM_STRING 100

// Struct para dados básicos da filial
typedef struct {
    int codigo;
    char cidade[TAM_STRING];
    char gerente[TAM_STRING];
} DadosFilial;

// Struct principal da filial com matriz de vendas
typedef struct {
    DadosFilial dados;
    float matriz_vendas[MAX_PRODUTOS];
    float total_mensal;
    int posicao_ranking;
} Filial;

// Struct para o registro mensal completo
typedef struct {
    Filial filiais[MAX_FILIAIS];
    int total_filiais;
    int total_produtos;
    int mes;
    int ano;
} RegistroMensal;

// Protótipos das funções
void menu_principal();
void inicializar_sistema(RegistroMensal *reg);
void cadastrar_filial(RegistroMensal *reg);
void registrar_vendas(RegistroMensal *reg);
void calcular_totais(RegistroMensal *reg);
void gerar_ranking(RegistroMensal *reg);
void exibir_relatorio_geral(RegistroMensal *reg);
void exibir_relatorio_filial(RegistroMensal *reg);
void salvar_arquivo(RegistroMensal *reg, const char *arquivo);
int carregar_arquivo(RegistroMensal *reg, const char *arquivo);
void limpar_buffer();
void pausar();

int main() {
    RegistroMensal registro;
    int opcao;
    char nome_arquivo[TAM_STRING];
    
    inicializar_sistema(&registro);
    
    printf("==============================================\n");
    printf("  SISTEMA DE VENDAS MULTI-FILIAIS\n");
    printf("==============================================\n\n");
    
    do {
        menu_principal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        printf("\n");
        
        switch(opcao) {
            case 1:
                cadastrar_filial(&registro);
                break;
            case 2:
                registrar_vendas(&registro);
                break;
            case 3:
                calcular_totais(&registro);
                gerar_ranking(&registro);
                exibir_relatorio_geral(&registro);
                break;
            case 4:
                exibir_relatorio_filial(&registro);
                break;
            case 5:
                printf("Nome do arquivo para salvar (ex: vendas_2024_01.dat): ");
                fgets(nome_arquivo, TAM_STRING, stdin);
                nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
                salvar_arquivo(&registro, nome_arquivo);
                break;
            case 6:
                printf("Nome do arquivo para carregar: ");
                fgets(nome_arquivo, TAM_STRING, stdin);
                nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
                if(carregar_arquivo(&registro, nome_arquivo)) {
                    printf("Dados carregados com sucesso!\n");
                }
                pausar();
                break;
            case 0:
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                pausar();
        }
        
    } while(opcao != 0);
    
    return 0;
}

void menu_principal() {
    printf("\n==============================================\n");
    printf("                 MENU PRINCIPAL\n");
    printf("==============================================\n");
    printf("1 - Cadastrar Filial\n");
    printf("2 - Registrar Vendas\n");
    printf("3 - Exibir Relatorio Geral e Ranking\n");
    printf("4 - Exibir Relatorio de Filial Especifica\n");
    printf("5 - Salvar Dados em Arquivo\n");
    printf("6 - Carregar Dados de Arquivo\n");
    printf("0 - Sair\n");
    printf("==============================================\n");
}

void inicializar_sistema(RegistroMensal *reg) {
    reg->total_filiais = 0;
    reg->total_produtos = 0;
    reg->mes = 0;
    reg->ano = 0;
    
    // Inicializa todas as vendas com 0
    for(int i = 0; i < MAX_FILIAIS; i++) {
        reg->filiais[i].dados.codigo = 0;
        reg->filiais[i].total_mensal = 0.0;
        reg->filiais[i].posicao_ranking = 0;
        for(int j = 0; j < MAX_PRODUTOS; j++) {
            reg->filiais[i].matriz_vendas[j] = 0.0;
        }
    }
}

void cadastrar_filial(RegistroMensal *reg) {
    if(reg->total_filiais >= MAX_FILIAIS) {
        printf("Limite de filiais atingido!\n");
        pausar();
        return;
    }
    
    Filial *nova = &reg->filiais[reg->total_filiais];
    
    printf("=== CADASTRO DE FILIAL ===\n");
    printf("Codigo da filial: ");
    scanf("%d", &nova->dados.codigo);
    limpar_buffer();
    
    printf("Cidade: ");
    fgets(nova->dados.cidade, TAM_STRING, stdin);
    nova->dados.cidade[strcspn(nova->dados.cidade, "\n")] = 0;
    
    printf("Nome do gerente: ");
    fgets(nova->dados.gerente, TAM_STRING, stdin);
    nova->dados.gerente[strcspn(nova->dados.gerente, "\n")] = 0;
    
    nova->total_mensal = 0.0;
    nova->posicao_ranking = 0;
    
    // Inicializa vendas com 0
    for(int i = 0; i < MAX_PRODUTOS; i++) {
        nova->matriz_vendas[i] = 0.0;
    }
    
    reg->total_filiais++;
    
    printf("\nFilial cadastrada com sucesso!\n");
    pausar();
}

void registrar_vendas(RegistroMensal *reg) {
    if(reg->total_filiais == 0) {
        printf("Nenhuma filial cadastrada!\n");
        pausar();
        return;
    }
    
    if(reg->mes == 0) {
        printf("Informe o periodo (mes/ano):\n");
        printf("Mes (1-12): ");
        scanf("%d", &reg->mes);
        printf("Ano: ");
        scanf("%d", &reg->ano);
        limpar_buffer();
    }
    
    if(reg->total_produtos == 0) {
        printf("Quantos produtos serao vendidos? ");
        scanf("%d", &reg->total_produtos);
        limpar_buffer();
        
        if(reg->total_produtos > MAX_PRODUTOS) {
            printf("Limite: %d produtos. Ajustando...\n", MAX_PRODUTOS);
            reg->total_produtos = MAX_PRODUTOS;
        }
    }
    
    printf("\n=== REGISTRO DE VENDAS ===\n");
    printf("Filiais cadastradas:\n");
    for(int i = 0; i < reg->total_filiais; i++) {
        printf("%d - %s (Cod: %d)\n", i+1, 
               reg->filiais[i].dados.cidade, 
               reg->filiais[i].dados.codigo);
    }
    
    int escolha;
    printf("\nEscolha a filial (1-%d): ", reg->total_filiais);
    scanf("%d", &escolha);
    limpar_buffer();
    
    if(escolha < 1 || escolha > reg->total_filiais) {
        printf("Filial invalida!\n");
        pausar();
        return;
    }
    
    Filial *filial = &reg->filiais[escolha-1];
    
    printf("\nRegistrando vendas para: %s\n", filial->dados.cidade);
    printf("Gerente: %s\n\n", filial->dados.gerente);
    
    for(int i = 0; i < reg->total_produtos; i++) {
        printf("Valor vendido do Produto %d: R$ ", i+1);
        scanf("%f", &filial->matriz_vendas[i]);
    }
    limpar_buffer();
    
    printf("\nVendas registradas com sucesso!\n");
    pausar();
}

void calcular_totais(RegistroMensal *reg) {
    for(int i = 0; i < reg->total_filiais; i++) {
        float total = 0.0;
        for(int j = 0; j < reg->total_produtos; j++) {
            total += reg->filiais[i].matriz_vendas[j];
        }
        reg->filiais[i].total_mensal = total;
    }
}

void gerar_ranking(RegistroMensal *reg) {
    // Bubble sort para ordenar por total de vendas (decrescente)
    for(int i = 0; i < reg->total_filiais - 1; i++) {
        for(int j = 0; j < reg->total_filiais - i - 1; j++) {
            if(reg->filiais[j].total_mensal < reg->filiais[j+1].total_mensal) {
                // Troca as filiais
                Filial temp = reg->filiais[j];
                reg->filiais[j] = reg->filiais[j+1];
                reg->filiais[j+1] = temp;
            }
        }
    }
    
    // Atribui posições do ranking
    for(int i = 0; i < reg->total_filiais; i++) {
        reg->filiais[i].posicao_ranking = i + 1;
    }
}

void exibir_relatorio_geral(RegistroMensal *reg) {
    if(reg->total_filiais == 0) {
        printf("Nenhuma filial cadastrada!\n");
        pausar();
        return;
    }
    
    printf("\n");
    printf("==============================================\n");
    printf("         RELATORIO GERAL DE VENDAS\n");
    printf("              Periodo: %02d/%d\n", reg->mes, reg->ano);
    printf("==============================================\n\n");
    
    float total_geral = 0.0;
    
    printf("RANKING DE FILIAIS:\n");
    printf("----------------------------------------------\n");
    for(int i = 0; i < reg->total_filiais; i++) {
        printf("%do Lugar - %s\n", reg->filiais[i].posicao_ranking,
               reg->filiais[i].dados.cidade);
        printf("  Gerente: %s\n", reg->filiais[i].dados.gerente);
        printf("  Total vendido: R$ %.2f\n\n", reg->filiais[i].total_mensal);
        total_geral += reg->filiais[i].total_mensal;
    }
    
    printf("==============================================\n");
    printf("TOTAL GERAL DE VENDAS: R$ %.2f\n", total_geral);
    printf("==============================================\n");
    
    // Estatísticas por produto
    if(reg->total_produtos > 0) {
        printf("\nVENDAS POR PRODUTO:\n");
        printf("----------------------------------------------\n");
        for(int j = 0; j < reg->total_produtos; j++) {
            float total_produto = 0.0;
            for(int i = 0; i < reg->total_filiais; i++) {
                total_produto += reg->filiais[i].matriz_vendas[j];
            }
            printf("Produto %d: R$ %.2f\n", j+1, total_produto);
        }
    }
    
    pausar();
}

void exibir_relatorio_filial(RegistroMensal *reg) {
    if(reg->total_filiais == 0) {
        printf("Nenhuma filial cadastrada!\n");
        pausar();
        return;
    }
    
    printf("\nFiliais disponiveis:\n");
    for(int i = 0; i < reg->total_filiais; i++) {
        printf("%d - %s\n", i+1, reg->filiais[i].dados.cidade);
    }
    
    int escolha;
    printf("\nEscolha a filial (1-%d): ", reg->total_filiais);
    scanf("%d", &escolha);
    limpar_buffer();
    
    if(escolha < 1 || escolha > reg->total_filiais) {
        printf("Filial invalida!\n");
        pausar();
        return;
    }
    
    Filial *f = &reg->filiais[escolha-1];
    
    printf("\n==============================================\n");
    printf("        RELATORIO DETALHADO DA FILIAL\n");
    printf("==============================================\n");
    printf("Codigo: %d\n", f->dados.codigo);
    printf("Cidade: %s\n", f->dados.cidade);
    printf("Gerente: %s\n", f->dados.gerente);
    printf("Posicao no Ranking: %do lugar\n", f->posicao_ranking);
    printf("----------------------------------------------\n");
    printf("VENDAS POR PRODUTO:\n");
    for(int i = 0; i < reg->total_produtos; i++) {
        printf("  Produto %d: R$ %.2f\n", i+1, f->matriz_vendas[i]);
    }
    printf("----------------------------------------------\n");
    printf("TOTAL MENSAL: R$ %.2f\n", f->total_mensal);
    printf("==============================================\n");
    
    pausar();
}

void salvar_arquivo(RegistroMensal *reg, const char *arquivo) {
    FILE *fp = fopen(arquivo, "wb");
    
    if(fp == NULL) {
        printf("Erro ao criar arquivo!\n");
        pausar();
        return;
    }
    
    // Escreve o cabeçalho
    fwrite(&reg->total_filiais, sizeof(int), 1, fp);
    fwrite(&reg->total_produtos, sizeof(int), 1, fp);
    fwrite(&reg->mes, sizeof(int), 1, fp);
    fwrite(&reg->ano, sizeof(int), 1, fp);
    
    // Escreve os dados de cada filial
    for(int i = 0; i < reg->total_filiais; i++) {
        fwrite(&reg->filiais[i], sizeof(Filial), 1, fp);
    }
    
    fclose(fp);
    
    printf("Dados salvos com sucesso em '%s'!\n", arquivo);
    pausar();
}

int carregar_arquivo(RegistroMensal *reg, const char *arquivo) {
    FILE *fp = fopen(arquivo, "rb");
    
    if(fp == NULL) {
        printf("Erro ao abrir arquivo! Verifique se o arquivo existe.\n");
        return 0;
    }
    
    // Lê o cabeçalho
    fread(&reg->total_filiais, sizeof(int), 1, fp);
    fread(&reg->total_produtos, sizeof(int), 1, fp);
    fread(&reg->mes, sizeof(int), 1, fp);
    fread(&reg->ano, sizeof(int), 1, fp);
    
    // Lê os dados de cada filial
    for(int i = 0; i < reg->total_filiais; i++) {
        fread(&reg->filiais[i], sizeof(Filial), 1, fp);
    }
    
    fclose(fp);
    
    return 1;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}