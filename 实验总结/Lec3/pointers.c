#include <stdio.h>
#include <stdlib.h>

void
f(void)
{
    int a[4];
    int *b = malloc(16);
    int *c;
    int i;

    printf("1: a = %p, b = %p, c = %p\n", a, b, c);
    // 1: a = 0x7ffeedc707d0, b = 0x7fa586c057f0, c = 0x0
    c = a;
    for (i = 0; i < 4; i++)
	a[i] = 100 + i;
    c[0] = 200;
    printf("2: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    // 2: a[0] = 200, a[1] = 101, a[2] = 102, a[3] = 103

    c[1] = 300;
    *(c + 2) = 301;
    3[c] = 302; // 这都能访问c[3]么
    printf("3: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    // 3: a[0] = 200, a[1] = 300, a[2] = 301, a[3] = 302

    c = c + 1;
    *c = 400;
    printf("4: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    // 4: a[0] = 200, a[1] = 400, a[2] = 301, a[3] = 302

    // c本来是int*, 强制转为char*,+1后，再转为int*；看起来不是完全指向a[2]
    c = (int *) ((char *) c + 1); 
    *c = 500;
    printf("5: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    // 5: a[0] = 200, a[1] = 128144, a[2] = 256, a[3] = 302

    b = (int *) a + 1;
    c = (int *) ((char *) a + 1);
    printf("6: a = %p, b = %p, c = %p\n", a, b, c);
    // 6: a = 0x7ffeedc707d0, b = 0x7ffeedc707d4, c = 0x7ffeedc707d1
}

int
main(int ac, char **av)
{
    f();
    return 0;
}