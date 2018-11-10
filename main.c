#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno
{
    char nome[101];
    char ra[7];
    char login[11];
    char password[9];
} Aluno;

typedef struct requisito
{
    char codigo[6];
    char nome[101];
} Requisito;

typedef struct disciplina
{
    Requisito requisitos[2];
    char codigo[6];
    char nome[101];
    float faltas;
    float nota;
    int creditos;
    int qtde_requisitos;
} Disciplina;

typedef struct matricula
{
    Disciplina * disciplinas;
    int semestre;
    char ra[6];
    int total_creditos;
    int total_disciplinas;
} Matricula;

void limpar(char *s);
int do_login(char login[], char password[], char * ra);
Disciplina get_subjects_by_code(char *code);
Requisito get_requiriment_info_by_code(char *codigo);
void get_requiriments_by_code(char *codigo, Requisito requisitos[], int *qtde);
void students_registration();
void subjects_search();
void register_student_in_subjects(char *ra);
void save_student_subject_register(Matricula matricula);
int verify_semester(char * ra, int semestre);
int verify_student_requiriments_of_subjects(char * ra, char * requisito);

int main(void)
{
    int has_logged = 0;
    int opt;
    char login[10], password[8];
    char *ra;

    do
    {
        while (!has_logged)
        {
            printf("Usuario: ");
            fgets(login, 10, stdin);
            printf("Senha: ");
            fgets(password, 8, stdin);
            limpar(login);
            limpar(password);
            has_logged = do_login(login, password, ra);

            if (!has_logged)
            {
                printf("Usuario ou senha invalidos!\n\n");
            }
            else
            {
                printf("Login realizado com sucesso!!\n\n");
            }
        }

        printf("## Menu de Opcoes ##\n\n");
        printf("1. Cadastro de Alunos\n");
        printf("2. Consulta de Disciplinas\n");
        printf("3. Realizar Matricula\n");
        printf("\n");
        printf("0. Sair\n");
        printf("\n");
        printf("Digite sua opcao: ");

        scanf("%d", &opt);
        printf("\n");

        switch (opt)
        {
        case 1:
            printf("- CADASTRO DE ALUNOS -\n\n");
            students_registration();
            break;
        case 2:
            printf("- CONSULTA DE DISCIPLINAS -\n\n");
            subjects_search();
            break;
        case 3:
            register_student_in_subjects(ra);
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

void limpar(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (
            s[i] == '\n' ||
            s[i] == '\r')
            s[i] = '\0';
    }
}

int do_login(char login[], char password[], char *ra)
{
    FILE *fp;
    Aluno aluno;
    int login_match = 0;
    const char s[2] = ",";
    char *token;
    char linha[130];
    char *result;

    fp = fopen("Arqvs TXT/Alunos.txt", "r");

    if (fp == NULL)
    {
        printf("ERRO AO LER O ARQUIVO!\n");

        return 0;
    }

    int j;
    while (!feof(fp))
    {
        result = fgets(linha, 130, fp);

        if (result)
            token = strtok(result, s);

        for (j = 0; token != NULL; j++)
        {
            if (j == 0)
            {
                strcpy(aluno.ra, token);
                limpar(aluno.ra);
            }
            else if (j == 2)
            {
                strcpy(aluno.login, token);
                limpar(aluno.login);
            }
            else if (j == 3)
            {
                strcpy(aluno.password, token);
                limpar(aluno.password);
            }

            token = strtok(NULL, s);
        }

        if (strcmp(login, aluno.login) == 0 && strcmp(password, aluno.password) == 0)
        {
            strcpy(ra, aluno.ra);
            login_match = 1;
            break;
        }
    }

    fclose(fp);
    return login_match;
}

void students_registration()
{
    Aluno new_student;
    FILE *fp;

    fp = fopen("Arqvs TXT/Alunos.txt", "a");

    if (fp == NULL)
    {
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

void subjects_search()
{
    FILE *fp_disciplinas, *fp_requisitos;
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

    if (strcmp(disciplina.codigo, "") == 0)
    {
        printf("\nDisciplina nao encontrada!\n\n");
        return;
    }

    printf("Nome: %s\n", disciplina.nome);
    printf("Quantidade de Creditos: %d\n", disciplina.creditos);
    printf("Pre-requisito(s): ");
    for (j = 0; j < disciplina.qtde_requisitos; j++)
    {
        printf("%s - %s", disciplina.requisitos[j].codigo, disciplina.requisitos[j].nome);
        if (j + 1 < disciplina.qtde_requisitos)
        {
            printf(", ");
        }
    }
    printf("\n\n");

    return;
}

Requisito get_requiriment_info_by_code(char *code)
{
    FILE *fp_disciplinas;
    Requisito requisito;
    const char s[2] = ",";
    char *token;
    char linha[90];
    char *result;

    fp_disciplinas = fopen("Arqvs TXT/Disciplinas.txt", "r");

    if (fp_disciplinas == NULL)
    {
        printf("ERRO AO LER ARQUIVO DE DISCIPLINAS!\n");
        return requisito;
    }

    while (!feof(fp_disciplinas))
    {
        result = fgets(linha, 90, fp_disciplinas);

        if (result)
        {
            token = strtok(result, s);
        }
        limpar(code);
        if (strcmp(code, token) == 0)
        {
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

void get_requiriments_by_code(char *codigo, Requisito requisitos[], int *qtde)
{
    FILE *fp_requisitos;
    char codigo_requisito[7];
    const char s[2] = ",";
    char *token;
    char linha[90];
    char *result;
    int j = 0;

    *qtde = 0;

    fp_requisitos = fopen("Arqvs TXT/Prerequisitos.txt", "r");

    if (fp_requisitos == NULL)
    {
        printf("ERRO AO LER ARQUIVO DE PREREQUISITOS!\n");

        return;
    }

    while (!feof(fp_requisitos))
    {
        result = fgets(linha, 90, fp_requisitos);
        if (result)
            token = strtok(result, s);

        if (strcmp(codigo, token) == 0)
        {
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

Disciplina get_subjects_by_code(char *codigo)
{
    FILE *fp_disciplinas;
    Disciplina disciplina;
    const char s[2] = ",";
    char *token;
    char linha[130];
    char *result;
    int j;

    fp_disciplinas = fopen("Arqvs TXT/Disciplinas.txt", "r");

    if (fp_disciplinas == NULL)
    {
        printf("ERRO AO LER ARQUIVO DE DISCIPLINAS!\n");

        return disciplina;
    }

    strcpy(disciplina.codigo, "");

    while (!feof(fp_disciplinas))
    {
        result = fgets(linha, 130, fp_disciplinas);

        if (result)
            token = strtok(result, s);

        if (strcmp(codigo, token) == 0)
        {
            for (j = 0; token != NULL; j++)
            {
                if (j == 0)
                {
                    strcpy(disciplina.codigo, token);
                    limpar(disciplina.codigo);
                }
                else if (j == 1)
                {
                    strcpy(disciplina.nome, token);
                    limpar(disciplina.nome);
                }
                else if (j == 2)
                {
                    disciplina.creditos = atoi(token);
                    
                }

                token = strtok(NULL, s);
            }
            break;
        }
    }
    fclose(fp_disciplinas);

    if (strcmp(disciplina.codigo, "") > 0)
        get_requiriments_by_code(codigo, disciplina.requisitos, &disciplina.qtde_requisitos);

    disciplina.faltas = 0;
    disciplina.nota = 0;
    return disciplina;
}

void register_student_in_subjects(char *ra)
{
    FILE *fp_alunos_disciplinas;
    const char s[2] = ",";
    Disciplina disciplina;
    Matricula matricula;
    char *token, *result, *codigo_requisito;
    char linha[130], creditos[2], codigo[6];
    int semestre, requisitos_cumpridos = 1;
    int i, j;

    fp_alunos_disciplinas = fopen("Arqvs TXT/AlunosDisciplinas.txt", "r");

    if (fp_alunos_disciplinas == NULL)
    {
        printf("ERRO AO LER ARQUIVO DE MATRICULAS!\n");

        return;
    }

    printf("- MATRICULA -\n\n");
    printf("Digite o semestre: ");
    scanf("%d", &semestre);
    printf("\n");

    if (verify_semester(ra, semestre) == 0) {
        printf("SEMESTRE INVALIDO!\n\n");
        return;
    }

    printf("Digite XX000 para sair\n");
    strcpy(matricula.ra, ra);
    matricula.semestre = semestre;
    matricula.disciplinas = (Disciplina *) malloc(sizeof(Disciplina)*100);
    matricula.total_creditos = 0;
    matricula.total_disciplinas = 0;
    
    j = 0;
    do {
        fflush(stdin);
        printf("Digite a disciplina: ");
        fgets(codigo, 6, stdin);
        limpar(codigo);

        strcpy(disciplina.codigo, "");

        disciplina = get_subjects_by_code(codigo);
        limpar(disciplina.codigo);

        if (strcmp(disciplina.codigo, "") > 0) {
            requisitos_cumpridos = 1;
            for (i = 0; i < disciplina.qtde_requisitos; i++) {
                requisitos_cumpridos *= verify_student_requiriments_of_subjects(ra, disciplina.requisitos[i].codigo);
            }
            if (requisitos_cumpridos) {
                matricula.disciplinas[j] = disciplina;
                matricula.total_creditos += disciplina.creditos;
                matricula.total_disciplinas++;
                j++;

                if (matricula.total_creditos > 32) {
                    printf("LIMITE DE 32 CREDITOS ULTRAPASSADO!\n\n");
                    free(matricula.disciplinas);
                    matricula.total_disciplinas = 0;
                    break;
                }
            } else {
                printf("A disciplina %s possui requisitos nao cumpridos pelo aluno!\n", disciplina.codigo);
            }
        } else if (strcmp(codigo, "XX000") != 0) {
            printf("DISCIPLINA INVALIDA!\n");
        }
    } while(strcmp(codigo, "XX000") != 0);

    if (matricula.total_disciplinas > 0) {
        save_student_subject_register(matricula);
        printf("Total disciplinas cadastradas: %d\nTotal creditos: %d\n\n", matricula.total_disciplinas, matricula.total_creditos);
    }
}

int verify_semester(char * ra, int semestre) {
    FILE *fp_alunos_disciplinas;
    const char s[2] = ",";
    char *token, *result;
    int arq_semestre, maior_semestre, valido = 1;
    char linha[130];
    int j;

    if (semestre <= 0) return 0;

    fp_alunos_disciplinas = fopen("Arqvs TXT/AlunosDisciplinas.txt", "r");

    while (!feof(fp_alunos_disciplinas))
    {
        result = fgets(linha, 130, fp_alunos_disciplinas);

        if (result)
        {
            token = strtok(result, s);

            if (strcmp(ra, token) == 0)
            {
                for (j = 0; token != NULL; j++)
                {
                    if (j == 2)
                    {
                        arq_semestre = atoi(token);
                        if (arq_semestre > semestre) {
                            valido = 0;
                            break;
                        }
                    }
                    token = strtok(NULL, s);
                }
            }

            if (!valido) {
                break;
            }
        }
    }
    fclose(fp_alunos_disciplinas);

    return valido;
}


int verify_student_requiriments_of_subjects(char * ra, char * requisito) {
    FILE *fp_alunos_disciplinas;
    const char s[2] = ",";
    char *token, *result;
    float nota, faltas;
    char linha[130];
    int j, ok = 0;

    fp_alunos_disciplinas = fopen("Arqvs TXT/AlunosDisciplinas.txt", "r");

    while (!feof(fp_alunos_disciplinas))
    {
        result = fgets(linha, 130, fp_alunos_disciplinas);

        if (result)
        {
            token = strtok(result, s);

            if (strcmp(ra, token) == 0)
            {
                token = strtok(NULL, s);
                if (strcmp(requisito, token) == 0) {
                    token = strtok(NULL, s);
                    token = strtok(NULL, s);
                    nota = atof(token);
                    token = strtok(NULL, s);
                    faltas = atof(token);

                    if (nota >= 5.0 && faltas <= 25.0) {
                        ok = 1;
                        break;
                    }
                }
            }
        }
    }
    fclose(fp_alunos_disciplinas);

    return ok;
}

void save_student_subject_register(Matricula matricula) {
    FILE * fp;
    int i;

    fp = fopen("Arqvs TXT/AlunosDisciplinas.txt", "a");

    if (fp == NULL) {
        printf("ERRO AO LER ARQUIVO DE MATRICULAS!\n\n");
        return;
    }

    for (i = 0; i < matricula.total_disciplinas; i++) {
        fprintf(fp, "%s,%s,%d,%.2f,%.2f\n", matricula.ra, matricula.disciplinas[i].codigo, matricula.semestre, matricula.disciplinas[i].nota, matricula.disciplinas[i].faltas);
    }

    fclose(fp);
}