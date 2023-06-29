#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "globals.c"
#include "menu.c"

// Utils

int stringToInteger(char *string) {
  int value = 0;
  for (int i = 0; string[i] != '\0'; i++) {
    value = value * 10 + (string[i] - '0');
  }

  return value;
}

char *toLower(char *string) {
  char *newString = malloc(sizeof(char) * strlen(string));
  strcpy(newString, string);
  for (char *p = newString; *p; p++) *p = tolower(*p);
  return newString;
}

int countRecords() {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    printf("Erro ao procurar registros!");
    return 0;
  }

  char line[250];
  int counter = 0;
  while (fgets(line, 250, file) != NULL) {
    counter++;
  }

  fclose(file);
  return counter;
}

// Validações

boolean isValidOption(int option) { return option > 0 && option < 10; }
void invalidOption() { printf("Opcao invalida!"); }

boolean isNumber(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] < '0' || str[i] > '9') continue;
    return true;
  }

  return false;
}

boolean haveNoRecords() {
  int recordCounter = countRecords();
  if (recordCounter == 0) {
    printf("Nenhum registro encontrado!");
    return true;
  }

  return false;
}

boolean isBlank(char *string) {
  while (*string != '\0') {
    if (!isspace(*string)) {
      return false;
    }
    string++;
  }
  return true;
}

// Procedimentos e funções

void getStringInput(char *prompt, char *field) {
  printf("\n%s", prompt);
  fgets(field, 50, stdin);
  field[strcspn(field, "\n")] = '\0';
}

void getIntegerInput(char *prompt, int *field) {
  char *stringInput = malloc(sizeof(char) * 50);
  boolean valid;
  do {
    clearScreen();
    printf("\n%s", prompt);
    fgets(stringInput, 50, stdin);
    stringInput[strcspn(stringInput, "\n")] = '\0';
    valid = isNumber(stringInput);

    if (!valid) message("[ERRO] - Valor invalido");
  } while (!valid);

  *field = stringToInteger(stringInput);
}

void enterStringInput(char *prompt, char *field) {
  boolean valid;
  do {
    clearScreen();
    getStringInput(prompt, field);
    valid = isBlank(field);

    if (valid) message("[ERRO] preencha o campo");
  } while (valid);
}

void createFileIfNotExists() {
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

void insert(struct Music *music) {
  FILE *file = fopen(RECORDS_PATH, "at");
  if (file == NULL) {
    printf("\nErro ao inserir registro!");
    return;
  }

  char *record = musicToString(music);

  fputs(record, file);
  fclose(file);
}

char *getRecordLineData(int index) {
  FILE *file = fopen(RECORDS_PATH, "rt");
  if (file == NULL) {
    printf("Erro ao procurar registro!");
    return "";
  }

  char *line = malloc(sizeof(char) * 250);
  for (int i = 0; fgets(line, 250, file) != NULL; i++) {
    if (i != index) continue;
    if (haveNoRecords()) return "";

    fclose(file);
    free(file);
    return line;
  }

  fclose(file);
  free(file);
  return line;
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

int findRecord() {
  char musicName[50];
  getStringInput(" >> Insira o nome da musica procurada: ", musicName);

  int recordIndex = find(musicName);
  if (recordIndex == -1) printf("\n[Musica nao encontrada]");
  return recordIndex;
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

// Implementação das funções solicitadas

// Inserir

void newRecord() {
  FILE *file = fopen(RECORDS_PATH, "at");
  if (file == NULL) {
    printf("Erro ao inserir registro!");
    return;
  }

  struct Music music;
  boolean alreadyExists;
  int validDate;

  do {
    enterStringInput(" >> Nome da musica: ", music.name);
    alreadyExists = find(music.name) != -1;

    if (alreadyExists) message("[ERRO] - Musica ja existente");
  } while (alreadyExists);

  getIntegerInput(" >> Duracao [mins]: ", &music.duration);
  enterStringInput(" >> Estilo: ", music.style);
  enterStringInput(" >> Nome do artista: ", music.artist.name);
  enterStringInput(" >> Nacionalidade do artista: ", music.artist.nationality);

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  music.registrationDate.day = tm.tm_mday;
  music.registrationDate.month = tm.tm_mon + 1;
  music.registrationDate.year = tm.tm_year + 1900;

  insert(&music);
  fclose(file);
  free(file);

  printf("\n[Nova musica inserida com sucesso!]");
}

// Remover

void recordRemove() {
  if (haveNoRecords()) return;

  int recordIndex = findRecord();
  if (recordIndex == -1) return;
  recordLineDateRemove(recordIndex);
  printf("\n[Musica removida com sucesso!]");
}

// Listar

void listRecords() {
  if (haveNoRecords()) return;
  listAllRecords();
}

// Consultar

void showRecord() {
  if (haveNoRecords()) return;

  int recordIndex = findRecord();
  if (recordIndex == -1) return;
  showRecordData(lineDataToMusic(getRecordLineData(recordIndex)));
}

// Lista com as funções do menu
const void (*FUNCTIONS[])() = {invalidOption, newRecord, recordRemove,
                               listRecords, showRecord};
