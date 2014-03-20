#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define READ_BUF_SIZE_MAX 512

int main(int argc, char **argv)
{
    char buf[READ_BUF_SIZE_MAX + 1];
    stack_t stack;
    int elem;
    stack_new(&stack, sizeof(int));

    for(;;)
    {
        if(fgets(buf, READ_BUF_SIZE_MAX, stdin))
        {
            if(strncmp(buf, "push", 4) == 0)
            {
                printf("cmd:%s\n", buf);
                elem = atoi(buf + 5);
                stack_push(&stack, &elem);
            }
            else if(strncmp(buf, "pop", 3) == 0)
            {
                printf("cmd:%s\n", buf);
                stack_pop(&stack, &elem);
                printf(">>>>> %d\n",elem);
            }
            else if(strncmp(buf, "exit", 4) == 0)
            {
                printf("cmd:%s\n", buf);
                break;
            }
            else
            {
                printf("command unsurport\n");
            }
        }
    }

    stack_dispos(&stack);
    return 0;
}
