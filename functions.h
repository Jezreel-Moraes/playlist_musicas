#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "menu.h"

boolean isValidOption(int option) { return option > 0 && option < 10; }

// Forma de selecionar a função baseado em posição
const void (*FUNCTIONS[])() = {};