/*
 * #include <execinfo.h>
 * 
 * int backtrace(void **buffer, int size);
 *
 * char **backtrace_symbols(void *const *buffer, int size);
 *
 * void backtrace_symbols_fd(void *const *buffer, int size, int fd);
 *
 * backtrace将当前程序的调用信息存储在buffer中
 * backtrace_symbols则是将buffer翻译为字符串，注意该函数用到了malloc，所以需要手工释放内存
 *
 * 我们来看man手册提供的如下代码：
 */

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void myfunc3(void)
{
    int j, nptrs;
    #define SIZE 100
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses \n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
     * would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if(strings == NULL)
    {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for(j = 0; j < nptrs; j++)
    {
        printf("%s\n", strings[j]);
    }

    free(strings);
}

/* "static" means don't export the symbol ... */
static void myfunc2(void)
{
    myfunc3();
}

void myfunc(int ncalls)
{
    if(ncalls > 1)
        myfunc(ncalls - 1);
    else
        myfunc2();
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "%s num-calls\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myfunc(atoi(argv[1]));
    exit(EXIT_SUCCESS);
}









#include <execinfo.h> 
