//
// Created by 68725 on 2022/8/13.
//
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

#define check_mem_leak

void *malloc_hook(size_t size, const char *file, const char *func, int line)
{
    void *p = malloc(size);

    char buff[128] = {0};
    sprintf(buff, "./mem/%p.mem", p);

    FILE *fp = fopen(buff, "w");
    fprintf(fp, "[+%s:%s:%d] --> addr:%p, size:%ld\n", file, func, line, p, size);
    fflush(fp);
    fclose(fp);

    return p;
}

void free_hook(void *p, const char *file, const char *func, int line)
{
    char buff[128] = {0};
    sprintf(buff, "./mem/%p.mem", p);

    if (unlink(buff) < 0)
    { // no exist
        printf("double free: %p\n", p);
        return;
    }

    free(p);
}

#ifdef check_mem_leak
#define malloc(size) malloc_hook(size, __FILE__, __FUNCTION__, __LINE__)
#define free(p) free_hook(p, __FILE__, __FUNCTION__, __LINE__)
#endif

int main()
{
    void *p1 = malloc(10);
    void *p2 = malloc(15);
    void *p3 = malloc(20);
    
    free(p2);
    free(p3);
}