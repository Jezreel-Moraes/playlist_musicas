#define OPTIONS_LENGTH 5

const char MENU_OPTIONS[OPTIONS_LENGTH][50] = {"Inserir", "Remover", "Listar",
                                               "Cadastrar", "Sair do programa"};

void showMenu() {
  clearScreen();
  printf("Gerenciamento de conjuntos\n\n");

  for (int i = 0; i < OPTIONS_LENGTH; i++)
    printf("[%d] - %s\n", i + 1, MENU_OPTIONS[i]);
  printf("\n>> Escolha uma opcao: ");
}

int getOption() {
  int option;
  scanf("%d", &option);
  return option;
}