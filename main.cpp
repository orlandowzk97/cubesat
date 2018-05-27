#include "zenith.h"
#include "mvp0.h"

#include <stdio.h>
using namespace std;


int main () {
    int a=1;
    char nome[20] = "mvp0.dat";
    while(a == 1) {
        a = Operator(nome);
        printf("%d", a);
    }
    return 0;
}