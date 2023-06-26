struct Artist {
  char *name;
  char *nationality;
};

struct Date {
  int day, month, year;
};

struct Music {
  char *name;
  int duration;
  char *style;
  struct Artist artist;
  struct Date registrationDate;
};

struct Playlist {
  struct Music *musics;
};

#define RECORDS_PATH "records.txt"

void enterToContinue() {
  printf("\n\n>> Pressione enter para continuar...");
  getch();
}

void message(char *message) {
  printf("\n%s", message);
  enterToContinue();
}

void clearScreen() { system("cls"); }