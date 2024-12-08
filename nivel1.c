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

void change_new_line_for_null(char* line){
    size_t len_line = strlen(line);
    if(len_line>0 && line[len_line-1] == '\n'){
        line[len_line - 1] = '\0';
    }
}

char *read_line(char* line){
	imprimir_prompt();
	if(fgets(line, COMMAND_LINE_SIZE, stdin) != NULL){
        change_new_line_for_null(line);
        return line;
	}else{
		if(feof(stdin)){
			printf("\rAdios, hasta luego!\n");
			exit(0);
		}else{
			perror("feof()");
		    return NULL;
		}
	}
}

int parse_args(char** args,char* linea){
	char *delimitadores = " \r\t\n";
	char *token = strtok(linea,delimitadores);
	int contador_tokens = 0;
	bool corregir = false;
	while(token != NULL){
		args[contador_tokens]= token;
		#ifdef DEBUGN1
		fprintf(stderr,
		GRIS_T"[parse_args()->token %d: %s]\n",
        contador_tokens,
		args[contador_tokens]);
		#endif
		if(args[contador_tokens][0] == '#'){
			corregir = true;
			break;
		}
		contador_tokens += 1;
		token = strtok(NULL, delimitadores);
	}
	args[contador_tokens] = NULL;
	#ifdef DEBUGN1
	if(corregir){
		fprintf(stderr,
		GRIS_T"[parse_args()->token %d corregido: %s]\n",
		contador_tokens,
		args[contador_tokens]);
	}else{
		fprintf(stderr,
		GRIS_T"[parse_args()->token %d: %s]\n",
		contador_tokens,
		args[contador_tokens]);
	}
	#endif
	return contador_tokens;
}

int execute_line(char* line){
	char *args[ARGS_SIZE];
	if((parse_args(args, line) > 0)){
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
