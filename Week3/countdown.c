#include <stdio.h>

void count_down() {
	int upper = 304;
	int lower = 17;
	int step = 5;
	
	printf("Reverse order list of integers greater than %d, beginning at %d, stepping by -%d.\n", lower, upper, step);
	
	for (; upper > lower; upper -= step) {
		printf("%d\n", upper);
	}
}

int main() {
	count_down();
	
	return 0;
}
