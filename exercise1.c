#include <stdio.h>
#include <unistd.h>

int main() {
	printf("Hello world!\n");
	printf("%d\n", (int)getpid());
	return (0);
}
