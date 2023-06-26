#include "functions.h"

int main(int argc, char const *argv[]) {
  struct Playlist playlist;
  playlist.musics = malloc(sizeof(struct Music));

  while (true) {
    showMenu();
    int option = getOption();

    if (!isValidOption(option)) {
      invalidOption();
      continue;
    }

    if (option == OPTIONS_LENGTH) break;
    (*FUNCTIONS[option])(playlist);
  };

  clearScreen();
  printf(">> Fim da Execucao do programa <<\n");
  printf("Equipe: Gustavo HTM e Jezreel KM");

  free(&playlist);
  return 0;
}