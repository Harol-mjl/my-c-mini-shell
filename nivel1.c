#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <libgen.h>

#define RESET_COLOR "\033[0m"
#define NEGRO_T "\x1b[30m"
#define NEGRO_F "\x1b[40m"
#define GRIS_T "\x1b[37m"
#define ROJO_T "\x1b[31m"
#define VERDE_T "\x1b[32m"
#define AMARILLO_T "\x1b[33m"
#define AZUL_T "\x1b[34m"
#define MAGENTA_T "\x1b[35m"
#define CYAN_T "\x1b[36m"
#define BLANCO_T "\x1b[97m"
#define BLANCO_T2 "\033[1;37m"
#define NEGRITA "\x1b[1m"
#define ORANGE_T "\033[38;5;214m"

#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define PROMPT '$'

#define FALSE 0
#define TRUE 1
#define ERROR -1

#define DEBUGN1

void imprimir_prompt(){
    //current working direcctory
    char cwd[COMMAND_LINE_SIZE];
    char* wordHome = "~/";
    char* wordPrompt = "--->";
    char* nameUser = getenv("USER");//get the name of the actual user
    
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        char* directory_name = basename(cwd);
        fprintf(stdout,VERDE_T NEGRITA"%s", nameUser);
        fprintf(stdout,BLANCO_T NEGRITA"%s", wordHome);
        fprintf(stdout,CYAN_T NEGRITA"%s", directory_name);
        fprintf(stdout,BLANCO_T2 NEGRITA"%s", wordPrompt);
        fprintf(stdout,ORANGE_T NEGRITA"%c: "RESET_COLOR, PROMPT);
    } else {
        perror("getcwd() return null");
    }
}

void replace_newline_to_null(char* line){
    size_t len_line = strlen(line);
    if(len_line > 0 && line[len_line - 1] == '\n'){
        line[len_line - 1] = '\0';
    }
}

char *read_line(char* line){
	imprimir_prompt();
	if (fgets(line, COMMAND_LINE_SIZE, stdin) == NULL){
        if (feof(stdin)){// Detectamos el (Control + D)
            printf("\nAdios, hasta luego!\n");
            exit(0);
        } else {
            perror("feof()");
            return NULL;
        }
	}
    replace_newline_to_null(line);
    return line;
}

void print_token_debugN1(int contador_tokens, char *token, bool corregido){
	#ifdef DEBUGN1
		if (!corregido){
			fprintf(
					stderr,
					GRIS_T"[parse_args()-> token %d: %s]\n",
					contador_tokens,
					token
					);
		} else {
			fprintf(
					stderr,
					GRIS_T"[parse_args()-> token %d corregido: %s]\n",
					contador_tokens,
					token
					);
		}
	#endif
}

int is_commentary(int contador_tokens, char *token){
	if(token[0] == '#'){
		print_token_debugN1(contador_tokens, token, false);
		return TRUE;
	} else {
		return FALSE;
	}
}

int parse_args(char** args, char* linea){
	char *delimitadores = " \r\t\n";
	char *token = strtok(linea, delimitadores);
	int contador_tokens = 0;
	
	while(token != NULL){
		if(is_commentary(contador_tokens, token) == 1){
			args[contador_tokens] = NULL;
			print_token_debugN1(contador_tokens, args[contador_tokens], true);
			return contador_tokens;
		}
		args[contador_tokens] = token;//Almacena el token en args
		print_token_debugN1(contador_tokens, token, false);
		token = strtok(NULL, delimitadores);//Siguiente token
		contador_tokens += 1;	
	}
	
	args[contador_tokens] = NULL;
	print_token_debugN1(contador_tokens, token, false);
	return contador_tokens;
}

int execute_line(char* line){
	char *args[ARGS_SIZE];
	int num_args = parse_args(args, line);
	if(num_args > 0){
		if(strcmp(args[0], "cd") == 0){
			printf("[internal_cd()→Cambia de directorio]\n");
			return 1;
		}else if(strcmp(args[0], "export") == 0){
			printf("[internal_export()→Cambia alguna variable de entorno]\n");
			return 1;
		}else if(strcmp(args[0], "source") == 0){
			printf("[internal_source()→Ejecuta comandos de un archivo]\n");
			return 1;
		}else if(strcmp(args[0], "jobs") == 0){
			printf("[internal_jobs()→Muestra el PID de los procesoso que no esten en foreground]\n");
			return 1;
		}else if(strcmp(args[0], "fg") == 0){
			printf("[internal_fg()→Trae trabajo a primer plano]\n");
			return 1;
		}else if(strcmp(args[0], "bg") == 0){
			printf("[internal_bg()→Continúa trabajo en segundo plano]\n");
			return 1;
		}else if(strcmp(args[0], "exit") == 0){
			printf("\rAdios, hasta luego!\r\n");
			exit(0);
			return 1;
		}
	}
	return 0;
}

int main(){
    char line[COMMAND_LINE_SIZE];
    while(1){
        if(read_line(line))
            execute_line(line);
    }
    return 0;
}
