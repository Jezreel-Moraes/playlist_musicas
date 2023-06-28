struct Artist {
  char name[50];
  char nationality[50];
};

struct Date {
  int day, month, year;
};

struct Music {
  char name[50];
  int duration;
  char style[50];
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