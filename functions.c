#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.c"
#include "menu.c"

boolean isValidOption(int option) { return option > 0 && option < 10; }
void invalidOption() { message("Opcao invalida!"); }

void insert(char *record) {
  FILE *file = fopen(RECORDS_PATH, "at");
  if (file == NULL) {
    message("Erro ao inserir registro!");
    return;
  }

  fputs(record, file);
  fclose(file);
}

int find(char *key) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    message("Erro ao procurar registro!");
    return -1;
  }

  int index = -1;
  char line[100];
  for (int i = 0; fgets(line, 100, file) != NULL; i++) {
    int nameLength = strchr(line, (int)';') - line;
    char name[120];
    strncpy(name, line, nameLength);
    name[nameLength] = '\0';

    if (*name == *key) {
      index = i;
      break;
    }
  }
  fclose(file);
  return index;
}

void newRecord(struct Playlist *playlist) { insert("Banana\n"); }

void recordRemove(struct Playlist *playlist) {}

void listRecords() {
  clearScreen();
  int i = find("Maracuja");
  printf("Achado em i = %d", i);
}

void showRecord(struct Playlist *playlist) {}

// Forma de selecionar a função baseado em posição
const void (*FUNCTIONS[])() = {invalidOption, newRecord, recordRemove,
                               listRecords, showRecord};
