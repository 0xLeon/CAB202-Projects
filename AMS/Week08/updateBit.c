#include <stdio.h>

unsigned char update_bit(unsigned char reg, unsigned char val) {
	return (reg ^ ((-val ^ reg) & (1 << 4)));
}

void print_bits(unsigned char value) {
	char bits[] = {'0', '1'};
	
	for (int bit = 7; bit >= 0; bit--) {
		unsigned char bit_val = value;
		
		bit_val &= (1 << bit);
		bit_val >>= bit;
		
		printf("%c", bits[bit_val]);
	}
}

int main(void) {
	unsigned char input_val[] = {
		0x00,
		0xff,
		(1 << 4) | (1 << (4 + 1)) | (1 << (4 - 1)),
		(1 << (4 + 1) ) | (1 << (4 - 1)),
	};

	unsigned char bit_val[] = {0, 1};

	for (int i = 0; i < (sizeof(input_val) / sizeof(unsigned char)); i++) {
		for (int j = 0; j < 2; j++) {
			unsigned char result = update_bit(input_val[i], bit_val[j]);
			
			printf("When input is 0x%02x, and bit_val is %d, the result is ", input_val[i], bit_val[j]);
			print_bits(result);
			printf("%s", "\n");
		}
	}

	return 0;
}
