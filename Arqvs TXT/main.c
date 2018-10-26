#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno {
    char nome[100];
    char ra[6];
    char login[10];
    char password[8];
} Aluno;

void limpa(char * s) {
    int i;
    for(i=0; s[i] != '\0'; i++){
        if (s[i] == '\n') s[i] = '\0';
    }
}

int do_login(char login[], char password[]) {
    FILE * fp;
    Aluno aluno;
    limpa(login);
    limpa(password);
    int login_match = 0;

    fp = fopen ("Alunos.txt", "r");
    while(fgets(fp,"%s %s %s %s", aluno.ra, aluno.nome, aluno.login, aluno.password) != EOF){
        printf("|%s| - |%s|\n", login, aluno.login);
        printf("|%s| - |%s|\n", password, aluno.password);
        if (strcmp(login, aluno.login) == 0 && strcmp(password, aluno.password) == 0) {
            login_match = 1;
            break;
        }
    }

    fclose(fp);
    return login_match;
}

int main(void)
{
    int has_logged = 0;
    char login[10], password[8];

    while (!has_logged) {
        printf("Usuario: ");
        fgets(login, 10, stdin);
        printf("Senha: ");
        fgets(password, 8, stdin);
        has_logged = do_login(login, password);

        if (!has_logged) {
            printf("Usuario ou senha invalidos!\n\n");
        }
    }

    printf("Logado com sucesso!\n");



    return 0;
}
