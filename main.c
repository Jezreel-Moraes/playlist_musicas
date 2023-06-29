#include "functions.c"

int main(int argc, char const *argv[]) {
  createFileIfNotExists();
  while (true) {
    showMenu();
    int option = getOption();

    if (!isValidOption(option)) {
      invalidOption();
      continue;
    }

    clearScreen();
    if (option == OPTIONS_LENGTH) break;
    (*FUNCTIONS[option])();
    enterToContinue();
  };

  clearScreen();
  printf(">> Fim da Execucao do programa <<\n");
  printf("Equipe: Gustavo HTM e Jezreel KM");
  return 0;
}