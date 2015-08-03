#include <stdio.h>

void list_integers() {
	int lower = 21;
	int upper = 309;
	
	printf("Counting from %d up to %d...\n", lower, upper);
	
	for (; lower <= upper; lower++) {
		printf("%d\n", lower);
	}
}

int main() {
	list_integers();
	
	return 0;
}
