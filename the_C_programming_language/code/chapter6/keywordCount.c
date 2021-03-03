#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "getch.c"
#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key))

struct key
{
    char *word;
    int count;
} keytab[] = {"auto", 0, "break", 0, "case", 0, "char", 0, "const", 0, "continue", 0, "default", 0, /* ... */ "unsigned", 0, "void", 0, "volatile", 0, "while", 0};

int getword(char *, int);
int binsearch(char *, struct key *, int); /* count C keywords */

int main_xiabiao()
{
    int n;
    char word[MAXWORD];
    int j =0;
    while (getword(word, MAXWORD) != EOF)
    {
        if (isalpha(word[0]))
            if ((n = binsearch_xiabiao(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
        if(++j>9)
            break;
    }

    for (n = 0; n < NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
    return 0;
}

int main()
{
    char word[MAXWORD];
    struct key *p;
    while(getword(word,MAXWORD)!=EOF)
    {
        if(isalpha(word[0]))
            if((p = binserch(word,keytab,NKEYS)) !=NULL)
                p->count++;
    }
    for (p = keytab; p < keytab + NKEYS; p++)
    {
        if (p->count > 0)
            printf("%4d %s\n", p->count, p->word);
    }
    return 0;
}

/* binsearch:  find word in tab[0]...tab[n-1] */
int binsearch_xiabiao(char *word, struct key tab[], int n)
{
    int cond;
    int low, high, mid;
    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

struct key *
binserch(char *word, struct key tab[], int n)
{
    int cond;
    struct key *low = &tab[0];
    //&tab[-1]和&tab[n]都超出了数组tab的范围。
    // 前者是绝对非法的，而对后者的间接引用也是非法的。
    // 但是，C语言的定义保证数组末尾之后的第一个元素（即&tab[n]）的指针算术运算可以正确执行
    struct key *high = &tab[n];
    struct key *mid;

    while(low < high) // 用指针时，判断的是[low,high) 区间
    {
        mid = low+(high - low)/2;
        if(cond = strcmp(word,mid->word) < 0)// word的首字母小于mid的首字母，word应在mid的前面
            high=mid;
        else if(cond >0)
            low = mid+1;
        else
            return mid;
    }
    return NULL;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim)
{
    int c;
    char *w = word;
    while ((c = getch())) //isspace:判断字符c是否为空格字符
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c)) //isalpha：判断字符c是否为英文字母
    {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++)
    {
        if (!isalnum(*w = getch())) //判断字符变量c是否为字母或数字，若是则返回非零，否则返回零
        {
            ungetch(*w); // 读到的字符c不是字母或数字时，将它放回输入中
            break;
        }
    }

    *w = '\0';
    return word[0];
}