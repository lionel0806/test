#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define get_addr(ptr, type, link) \
	(type *)((char *)(ptr) - offsetof(type,link))

typedef struct person_info_t person_info_s;

struct person_info_t{
	char name[32];
	int age;
	char addr[512];
}my_info = {
	"lionel li",
	26,
	"zhuhai xiangzhou jiaxinghuatin"
};

int main(int argv, char **argv)
{
	
	printf("%s %d %s \n",get_addr(char, person_info_s, name), get_addr(int, person_info_s, age),get_addr(char, person_info_s, addr));
	return 0;
}
