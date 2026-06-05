#include "../include/Allocator.h"

int main()
{
    int *a = malloc_clone(100);
    int *b = malloc_clone(200);
    int *c = malloc_clone(300);

    detect_leaks();

    (void)a;
    (void)b;
    (void)c;

    return 0;
}