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
void invalidOption() { printf("Opcao invalida!"); }

createFileIfNotExists() {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file != NULL) {
    fclose(file);
    free(file);
    return;
  };

  FILE *newFile = fopen(RECORDS_PATH, "wt");
  if (newFile != NULL) {
    fclose(newFile);
    free(newFile);
    return;
  };

  message("[Erro] Não foi possível criar o arquivo de registros");
  exit(-1);
}

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
    printf("Erro ao inserir registro!");
    return;
  }

  char *record = musicToString(music);

  printf("record: %s\n", record);

  fputs(record, file);
  fclose(file);
}

void getStringInput(char *prompt, char *atr) {
  printf("\n\n%s", prompt);
  fgets(atr, 49, stdin);
  atr[strcspn(atr, "\n")] = '\0';
}

void getIntegerInput(char *prompt, int *atr) {
  printf("\n\n%s", prompt);
  scanf("%d", atr);
  fflush(stdin);
}

int isValidDate(int days, int month, int year) {
  int daysForMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (year > 2023) return 0;

  if (month < 1 || month > 12) return 0;

  if (days < 1 || days > daysForMonth[month - 1]) return 0;

  return 1;
}

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

void showRecordData(struct Music music) {
  printf("\nNome da musica: %s", music.name);
  printf("\nDuracao: %d mins", music.duration);
  printf("\nEstilo da musica: %s", music.style);
  printf("\nNome do autor: %s", music.artist.name);
  printf("\nNacionalidade do autor: %s", music.artist.nationality);
  struct Date date = music.registrationDate;
  printf("\nData de registro: %02d/%02d/%02d", date.day, date.month, date.year);
}

char *toLower(char *string) {
  char *newString = malloc(sizeof(char) * strlen(string));
  strcpy(newString, string);
  for (char *p = newString; *p; p++) *p = tolower(*p);
  return newString;
}

int find(char *key) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    printf("Erro ao procurar registro!");
    return -1;
  }

  char *formattedKey = toLower(key);

  char line[250];
  int index = 0;
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    char musicName[50];
    int start = 0, end = 0;
    nextData(&start, &end, line, musicName);

    if (strcmp(toLower(musicName), formattedKey) != 0) {
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
    printf("Erro ao procurar registro!");
    return;
  }

  char *line = malloc(sizeof(char) * 250);
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    if (i != index) continue;

    fclose(file);
    free(file);
    return line;
  }

  fclose(file);
  free(file);
  return line;
}

int findRecord() {
  char musicName[50];
  getStringInput(" >> Insira o nome da musica procurada: ", musicName);

  int recordIndex = find(musicName);
  if (recordIndex == -1) printf("[Musica nao encontrada]");
  return recordIndex;
}

void listAllRecords() {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    printf("Erro ao procurar registros!");
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

int countRecords() {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    printf("Erro ao procurar registros!");
    return;
  }

  char line[250];
  int counter = 0;
  while (fgets(line, 250, file) != NULL) {
    counter++;
  }

  fclose(file);
  return counter;
}

void recordLineDateRemove(int index) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  FILE *temp = fopen(TEMP_PATH, "w");
  if (file == NULL || temp == NULL) {
    printf("Erro ao procurar registro!");
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

boolean haveNoRecords() {
  int recordCounter = countRecords();
  if (recordCounter == 0) {
    printf("Nenhum registro encontrado!");
    return true;
  }

  return false;
}

void newRecord() {
  FILE *file = fopen(RECORDS_PATH, "at");
  if (file == NULL) {
    message("Erro ao inserir registro!");
    return;
  }

  struct Music music;
  bool musicDuplicated;
  int validDate;

  do {
    clearScreen();
    getStringInput("Informe o nome da musica: ", music.name);
    musicDuplicated = find(music.name) != -1;

    if (musicDuplicated) message("\nMusica ja existente, tente novamente...");
  } while (musicDuplicated);

  getIntegerInput("Informe seu tempo de duracao em minutos: ", &music.duration);

  getStringInput("Informe o estilo musical da musica: ", music.style);

  getStringInput("Informe o nome do artista: ", music.artist.name);

  getStringInput("Informe a nacionalidade do artista: ",
                 music.artist.nationality);

  do {
    printf("\n\nDigite a data no formato dia/mes/ano: ");
    scanf("%d/%d/%d", &music.registrationDate.day,
          &music.registrationDate.month, &music.registrationDate.year);

    if (!validDate) message("\nData invalida, tente novamente...");
  } while (!validDate);

  insert(&music);
  fclose(file);
  free(file);
}

void recordRemove() {
  if (haveNoRecords()) return;

  int recordIndex = findRecord();
  if (recordIndex == -1) return;
  recordLineDateRemove(recordIndex);
  printf(" >> Musica removida com sucesso!");
}

void listRecords() {
  if (haveNoRecords()) return;
  listAllRecords();
}

void showRecord() {
  if (haveNoRecords()) return;

  int recordIndex = findRecord();
  if (recordIndex == -1) return;
  showRecordData(lineDataToMusic(getRecordLineData(recordIndex)));
}

// Forma de selecionar a função baseado em posição
const void (*FUNCTIONS[])() = {invalidOption, newRecord, recordRemove,
                               listRecords, showRecord};
