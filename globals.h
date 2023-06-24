struct artist {
  char *name;
  char *nationality;
};

void enterToContinue() {
  printf("\n\n>> Pressione enter para continuar...");
  getch();
}

void message(char *message) {
  printf("\n%s", message);
  enterToContinue();
}

void clearScreen() { system("cls"); }

void invalidOption() { message("Opcao invalida!"); }