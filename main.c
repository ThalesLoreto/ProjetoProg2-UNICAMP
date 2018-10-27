#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno {
	char nome[100];
	char ra[6];
	char login[10];
	char password[8];
} Aluno;

void limpar(char * s) {
    int i;
    for(i=0; s[i] != '\0'; i++) {
        if (s[i] == '\n') s[i] = '\0';
    }
}

int do_login(char login[], char password[]) {
    FILE * fp;
	Aluno aluno;
	int login_match = 0;
	const char s[2] = ",";
	char *token;
    char linha[90];
    char *result;

    fp = fopen ("Arqvs TXT/Alunos.txt", "r");
    token = strtok(fp, s);

    int i = 0, j;
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

            printf( "%s\n", token );
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

int main(void)
{
	int has_logged = 0;
	char login[10], password[8];

	//while (1) {
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
            }
        }

        printf("Login realizado com sucesso!!\n");
    //}





    return 0;
}
