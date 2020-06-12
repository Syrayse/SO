#include "Language.h"
#include "Response.h"
#include "Common.h"
#include <stdio.h>

// Para a linguagem portuguesa
void task_execute_pt(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Nova tarefa #%ld\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_killed_pt(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "A tarefa #%ld foi assassinada\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exist_pt(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "A tarefa #%ld nao existe\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exec_pt(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "A tarefa #%ld nao esta em execucao\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

// Para a linguagem inglesa
void task_execute_en(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "New task #%ld\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_killed_en(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Task #%ld has been killed\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exist_en(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Task #%ld doesn't existe\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exec_en(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Task #%ld is not in execution\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

// Para a linguagem alema
void task_execute_de(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Neue Aufgabe #%ld\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_killed_de(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Aufgabe #%ld wurde getotet\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exist_de(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Aufgabe #%ld existiert nicht\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exec_de(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Aufgabe #%ld ist nicht in Ausfuhrung\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

// Para a linguagem francesa
void task_execute_fr(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "Nouvelle tache #%ld\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_killed_fr(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "La tache #%ld a ete tuee\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exist_fr(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "La tache #%ld n'existe pas\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

void task_nen_exec_fr(Response response) {
        char *buff = "";
        ssize_t n = asprintf(&buff, "La tache #%ld n'est pas en cours d'execution\n", response->id_task);

        if(write(1, buff, n) == -1)
                throw_error(2, "erro na escrita!");
}

// setters de lingua
void echo_message(Response response) {
        if(write(1, response->buffer, response->length) == -1)
                throw_error(2, "erro na escrita!");
}

void portuguese_language(LangDispatchFunc *dispatchers) {
        dispatchers[TASK_EXECUTE] = task_execute_pt;
        dispatchers[TASK_KILLED] = task_killed_pt;
        dispatchers[TASK_NEN_EXIST] = task_nen_exist_pt;
        dispatchers[TASK_NEN_EXEC] = task_nen_exec_pt;
        dispatchers[ECHO] = echo_message;
}

void english_language(LangDispatchFunc *dispatchers) {
        dispatchers[TASK_EXECUTE] = task_execute_en;
        dispatchers[TASK_KILLED] = task_killed_en;
        dispatchers[TASK_NEN_EXIST] = task_nen_exist_en;
        dispatchers[TASK_NEN_EXEC] = task_nen_exec_en;
        dispatchers[ECHO] = echo_message;
}

void german_language(LangDispatchFunc *dispatchers) {
        dispatchers[TASK_EXECUTE] = task_execute_de;
        dispatchers[TASK_KILLED] = task_killed_de;
        dispatchers[TASK_NEN_EXIST] = task_nen_exist_de;
        dispatchers[TASK_NEN_EXEC] = task_nen_exec_de;
        dispatchers[ECHO] = echo_message;
}

void french_language(LangDispatchFunc *dispatchers) {
        dispatchers[TASK_EXECUTE] = task_execute_fr;
        dispatchers[TASK_KILLED] = task_killed_fr;
        dispatchers[TASK_NEN_EXIST] = task_nen_exist_fr;
        dispatchers[TASK_NEN_EXEC] = task_nen_exec_fr;
        dispatchers[ECHO] = echo_message;
}
