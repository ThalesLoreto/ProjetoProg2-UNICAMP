#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno {
	char nome[101];
	char ra[7];
	char login[11];
	char password[9];
} Aluno;

typedef struct requisito {
	char codigo[6];
	char nome[101];
} Requisito;

typedef struct disciplina {
	char codigo[6];
	char nome[101];
	char creditos;
    int qtde_requisitos;
	Requisito requisitos[2];
} Disciplina;

void limpar(char * s);
int  do_login(char login[], char password[]);
Disciplina get_subjects_by_code(char * code);
Requisito get_requiriment_info_by_code(char * codigo);
void get_requiriments_by_code(char * codigo, Requisito requisitos[], int * qtde);
void students_registration();
void subjects_search();


int main(void)
{
	int has_logged = 0;
    int opt;
	char login[10], password[8];

	do {
        while (!has_logged) {
            printf("Usuario: ");
            fgets(login, 10, stdin);
            printf("Senha: ");
            fgets(password, 8, stdin);
            limpar(login);
            limpar(password);
            has_logged = do_login(login, password);

            if (!has_logged) {
                printf("Usuario ou senha invalidos!\n\n");
            } else {
                printf("Login realizado com sucesso!!\n\n");
            }
        }

        printf("## Menu de Opções ##\n\n");
        printf("1. Cadastro de Alunos\n");
        printf("2. Consulta de Disciplinas\n");
        printf("\n");
        printf("0. Sair\n");
        printf("\n");
        printf("Digite sua opcao: ");

        scanf("%d", &opt);
        printf("\n");

        switch(opt) {
            case 1:
                printf("- CADASTRO DE ALUNOS -\n\n");
                students_registration();
                break;
            case 2:
                printf("- CONSULTA DE DISCIPLINAS -\n\n");
                subjects_search();
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opt != 0);

    return 0;
}


void limpar(char * s) {
    int i;
    for(i=0; s[i] != '\0'; i++) {
        if (
            s[i] == '\n' || 
            s[i] == '\r'
        ) s[i] = '\0';
    }
}

int do_login(char login[], char password[]) {
    FILE * fp;
	Aluno aluno;
	int login_match = 0;
	const char s[2] = ",";
	char *token;
    char linha[130];
    char *result;

    fp = fopen ("Arqvs TXT/Alunos.txt", "r");

    if (fp == NULL) {
        printf("ERRO AO LER O ARQUIVO!\n");

        return 0;
    }

    int j;
    while (!feof(fp))
    {
        result = fgets(linha, 90, fp);

        if (result)
          token = strtok(result, s);

        for(j = 0; token != NULL; j++)
        {
            if (j == 2) {
                strcpy(aluno.login, token);
                limpar(aluno.login);
            }
            else if (j == 3) {
                strcpy(aluno.password, token);
                limpar(aluno.password);
            }

            token = strtok(NULL, s);
        }

        if (strcmp(login, aluno.login) == 0 && strcmp(password, aluno.password) == 0) {
            login_match = 1;
            break;
        }
    }

    fclose(fp);
    return login_match;
}

void students_registration() {
    Aluno new_student;
    FILE * fp;

    fp = fopen ("Arqvs TXT/Alunos.txt", "a");

    if (fp == NULL) {
        printf("ERRO AO LER O ARQUIVO!\n");

        return;
    }

    printf("Digite o RA do Aluno: ");
    fgets(new_student.ra, 6, stdin);
    fgets(new_student.ra, 7, stdin);

    printf("Digite o Nome do Aluno: ");
    fgets(new_student.nome, 101, stdin);
    fgets(new_student.nome, 101, stdin);

    printf("Digite o login: ");
    fgets(new_student.login, 11, stdin);

    printf("Digite a senha: ");
    fgets(new_student.password, 9, stdin);

    limpar(new_student.ra);
    limpar(new_student.nome);
    limpar(new_student.login);
    limpar(new_student.password);

    fprintf(fp, "%s,%s,%s,%s\n", new_student.ra, new_student.nome, new_student.login, new_student.password);

    fclose(fp);
}

void subjects_search() {
    FILE * fp_disciplinas, * fp_requisitos;
    Disciplina disciplina;
    char codigo[6];
    const char s[2] = ",";
	char *token;
    char linha[130];
    char *result;
    char creditos[2];
    int j;

    printf("Digite a disciplina: ");
    fgets(codigo, 6, stdin);
    fgets(codigo, 6, stdin);

    limpar(codigo);
    disciplina = get_subjects_by_code(codigo);

    if (strcmp(disciplina.codigo, "") == 0) {
        printf("Disciplina não encontrada!\n\n");
        return;
    }

    printf("Nome: %s\n", disciplina.nome);
    printf("Quantidade de Creditos: %c\n", disciplina.creditos);
    printf("Pre-requisito(s): ");
    for (j = 0; j < disciplina.qtde_requisitos; j++) {
        printf("%s - %s", disciplina.requisitos[j].codigo, disciplina.requisitos[j].nome);
        if (j+1 < disciplina.qtde_requisitos){
            printf(", ");
        }
    }   
    printf("\n\n");

    return;
}

Requisito get_requiriment_info_by_code(char * code) {
    FILE * fp_disciplinas;
    Requisito requisito;
    const char s[2] = ",";
	char *token;
    char linha[90];
    char *result;

    fp_disciplinas = fopen("Arqvs TXT/Disciplinas.txt", "r");


    if (fp_disciplinas == NULL) {
        printf("ERRO AO LER ARQUIVO DE DISCIPLINAS!\n");
        return requisito;
    }

    while (!feof(fp_disciplinas))
    {
        result = fgets(linha, 90, fp_disciplinas);
       
        if (result) {
            token = strtok(result, s);
        }
        limpar(code);
        if (strcmp(code, token) == 0) {
            strcpy(requisito.codigo, token);
            limpar(requisito.codigo);

            token = strtok(NULL, s);

            strcpy(requisito.nome, token);
            limpar(requisito.nome);

            break;
        }
    }
    fclose(fp_disciplinas);

    return requisito;
}

void get_requiriments_by_code(char * codigo, Requisito requisitos[], int * qtde) {
    FILE * fp_requisitos;
    char codigo_requisito[7];
    const char s[2] = ",";
	char *token;
    char linha[90];
    char *result;
    int j = 0;

    *qtde = 0;

    fp_requisitos = fopen("Arqvs TXT/Prerequisitos.txt", "r");

    if (fp_requisitos == NULL) {
        printf("ERRO AO LER ARQUIVO DE PREREQUISITOS!\n");

        return;
    }

    while (!feof(fp_requisitos))
    {
        result = fgets(linha, 90, fp_requisitos);
        if (result)
          token = strtok(result, s);

        if (strcmp(codigo, token) == 0) {
            token = strtok(NULL, s);
            strcpy(codigo_requisito, token);
            limpar(codigo_requisito);

            requisitos[j] = get_requiriment_info_by_code(codigo_requisito);
            
            *qtde += 1;
            j++;
        }
    }
    fclose(fp_requisitos);

    return;
}



Disciplina get_subjects_by_code(char * codigo) {
    FILE * fp_disciplinas;
    Disciplina disciplina;
    const char s[2] = ",";
	char *token;
    char linha[130];
    char *result;
    char creditos[2];
    int j;

    fp_disciplinas = fopen("Arqvs TXT/Disciplinas.txt", "r");

    if (fp_disciplinas == NULL) {
        printf("ERRO AO LER ARQUIVO DE DISCIPLINAS!\n");

        return disciplina;
    }

    strcpy(disciplina.codigo, "");

    while (!feof(fp_disciplinas))
    {
        result = fgets(linha, 130, fp_disciplinas);

        if (result)
          token = strtok(result, s);

        if (strcmp(codigo, token) == 0) {
            for(j = 0; token != NULL; j++)
            {
                if (j == 0) {
                    strcpy(disciplina.codigo, token);
                    limpar(disciplina.codigo);
                }
                else if (j == 1) {
                    strcpy(disciplina.nome, token);
                    limpar(disciplina.nome);
                }
                else if (j == 2) {
                    strcpy(creditos, token);
                    limpar(creditos);
                    disciplina.creditos = creditos[0];
                }

                token = strtok(NULL, s);
            }
            break;
        }
    }
    fclose(fp_disciplinas);

    if (strcmp(disciplina.codigo, "") > 0)
        get_requiriments_by_code(codigo, disciplina.requisitos, &disciplina.qtde_requisitos);

    return disciplina;
}

