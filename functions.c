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

void populateRecordString(char *record, char *field, int *currentIndex) {
  for (int i = 0; field[i] != '\0'; i++) {
    record[(*currentIndex)++] = field[i];
  }
  record[(*currentIndex)++] = ';';
}

char *musicToString(struct Music *music) {
  char *record = malloc(sizeof(char) * 250);
  char *intToStr = malloc(sizeof(char) * 10);
  int currentIndex = 0;

  populateRecordString(record, music->name, &currentIndex);

  sprintf(intToStr, "%d", music->duration);
  populateRecordString(record, intToStr, &currentIndex);

  populateRecordString(record, music->style, &currentIndex);
  populateRecordString(record, music->artist.name, &currentIndex);
  populateRecordString(record, music->artist.nationality, &currentIndex);

  sprintf(intToStr, "%d", music->registrationDate.day);
  populateRecordString(record, intToStr, &currentIndex);

  sprintf(intToStr, "%d", music->registrationDate.month);
  populateRecordString(record, intToStr, &currentIndex);

  sprintf(intToStr, "%d", music->registrationDate.year);
  populateRecordString(record, intToStr, &currentIndex);

  record[currentIndex++] = '\n';
  record[currentIndex] = '\0';
  return record;
}

void insert(struct Music *music) {
  FILE *file = fopen(RECORDS_PATH, "at");
  if (file == NULL) {
    message("Erro ao inserir registro!");
    return;
  }

  char *record = musicToString(music);

  printf("record: %s\n", record);

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

void newRecord(struct Playlist *playlist) {}

void recordRemove(struct Playlist *playlist) {}

void listRecords() {}

void showRecord(struct Playlist *playlist) {}

// Forma de selecionar a função baseado em posição
const void (*FUNCTIONS[])() = {invalidOption, newRecord, recordRemove,
                               listRecords, showRecord};

int main(int argc, char const *argv[]) {
  struct Music *music = malloc(sizeof(struct Music));

  strcpy(music->name, "Roberto Martins");
  music->duration = 31;
  strcpy(music->style, "Phonk");
  strcpy(music->artist.name, "Cordel");
  strcpy(music->artist.nationality, "Russo");
  music->registrationDate.day = 26;
  music->registrationDate.month = 6;
  music->registrationDate.year = 2023;

  insert(music);
  printf("Inserido");
  free(music);
  return 0;
}