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

void newRecord() {}

int stringToInteger(char *string) {
  int value = 0;
  for (int i = 0; string[i] != '\0'; i++) {
    value = value * 10 + (string[i] - '0');
  }

  return value;
}

char *nextData(int *startIndex, int *endIndex, char *line, char *field) {
  if (*endIndex == 0) *endIndex = strchr(line, ';') - line;
  strncpy(field, line + *startIndex, *endIndex);
  field[*endIndex] = '\0';
  *startIndex += *endIndex + 1;
  *endIndex = (int)(strchr(line + *startIndex, ';') - line) - *startIndex;
}

struct Music lineDataToMusic(char *line) {
  struct Music music;

  int startIndex = 0, endIndex = 0;

  nextData(&startIndex, &endIndex, line, music.name);

  char duration[50];
  nextData(&startIndex, &endIndex, line, duration);
  music.duration = stringToInteger(duration);

  nextData(&startIndex, &endIndex, line, music.style);
  nextData(&startIndex, &endIndex, line, music.artist.name);
  nextData(&startIndex, &endIndex, line, music.artist.nationality);

  char day[50];
  nextData(&startIndex, &endIndex, line, day);
  music.registrationDate.day = stringToInteger(day);

  char month[50];
  nextData(&startIndex, &endIndex, line, month);
  music.registrationDate.month = stringToInteger(month);

  char year[50];
  nextData(&startIndex, &endIndex, line, year);
  music.registrationDate.year = stringToInteger(year);

  return music;
}

void listRecords() {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    message("Erro ao procurar registro!");
    return;
  }

  printf("Listagem de Musicas Cadastradas\n\n");
  printf("%-30s%-30s%-30s%-30s\n", "Musica", "Artista", "Nacionalidade",
         "Cadastramento");

  char line[250];
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    struct Music music = lineDataToMusic(line);

    printf("%-30s%-30s%-30s%02d/%02d/%02d\n", music.name, music.artist.name,
           music.artist.nationality, music.registrationDate.day,
           music.registrationDate.month, music.registrationDate.year);
  }

  fclose(file);
}

void showRecordData(struct Music music) {
  printf("\nNome da musica: %s", music.name);
  printf("\nDuracao: %d mins", music.duration);
  printf("\nEstilo da musica: %s", music.style);
  printf("\nNome do autor: %s", music.artist.name);
  printf("\nNacionalidade do autor: %s", music.artist.nationality);
  struct Date date = music.registrationDate;
  printf("\nData de registro: %02d/%02d/%02d", date.day, date.month, date.year);
}

int find(char *key) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    message("Erro ao procurar registro!");
    return -1;
  }

  char line[250];
  int index = 0;
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    char musicName[50];
    int start = 0, end = 0;
    nextData(&start, &end, line, musicName);

    if (strcmp(musicName, key) != 0) {
      index++;
      continue;
    };

    fclose(file);
    free(file);
    return index;
  }

  fclose(file);
  free(file);
  return -1;
}

char *getRecordLineData(int index) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    message("Erro ao procurar registro!");
    return;
  }

  char *line = malloc(sizeof(char) * 250);
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    if (i != index) continue;

    fclose(file);
    free(file);
    return line;
  }
}

int findRecord() {
  char musicName[50];
  printf(" >> Insira o nome da musica procurada:");
  fgets(musicName, 50, stdin);
  musicName[strcspn(musicName, "\n")] = '\0';

  int recordIndex = find(musicName);
  if (recordIndex == -1) printf("[Musica nao encontrada]");
  return recordIndex;
}

void showRecord() {
  int recordIndex = findRecord();
  if (recordIndex == -1) return;
  showRecordData(lineDataToMusic(getRecordLineData(recordIndex)));
}

void recordRemove(int index) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  FILE *temp = fopen(TEMP_PATH, "w");
  if (file == NULL || temp == NULL) {
    message("Erro ao procurar registro!");
    return;
  }

  char line[250];
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    if (i == index) continue;
    fputs(line, temp);
  }

  fclose(file);
  free(file);
  fclose(temp);
  free(temp);

  remove(RECORDS_PATH);
  rename(TEMP_PATH, RECORDS_PATH);
}

// Forma de selecionar a função baseado em posição
const void (*FUNCTIONS[])() = {invalidOption, newRecord, recordRemove,
                               listRecords, showRecord};

int insertTest(int argc, char const *argv[]) {
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

int getDataAndShowTest(int argc, char const *argv[]) {
  showRecordData(
      lineDataToMusic("Roberto Martins;31;Phonk;Cordel;Russo;26;6;2023;"));

  return 0;
}

int testeListagem(int argc, char const *argv[]) {
  listRecords();
  return 0;
}

int testeFind(int argc, char const *argv[]) {
  printf("%d", find("Roberto Martins"));
  printf("%d", find("Rita Cassia"));
  printf("%d", find("Jezreel Moraes"));
  printf("%d", find("Gustavo Martins"));

  return 0;
}

int testeListRemoveThenListAgain(int argc, char const *argv[]) {
  listRecords();
  printf("\n\n\n");
  recordRemove(find("Gustavo Martins"));
  listRecords();

  return 0;
}

int main(int argc, char const *argv[]) {
  showRecord();
  return 0;
}
