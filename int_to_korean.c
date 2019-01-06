#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALUES C(일)C(이)C(삼)C(사)C(오)C(육)C(칠)C(팔)C(구)
#define RADIX C(일)C(십)C(백)C(천)C(만)

#define C(x) #x,
const char * const values_list[] = { VALUES };
const char * const radix_list[] = { RADIX };
#ifndef C
#define C(x) x,
enum values { VALUES BUTTOM };
#endif

void convert_print(int value, int rad) {
	if(rad > 0 && value != 1)
		printf("%s%s", values_list[value - 1], radix_list[rad]);
	else if(value == 1)
		printf("%s", radix_list[rad]);
	else if(rad == 0)
		printf("%s", values_list[value - 1]);
}

void converter(int num) {
	int i = 0, mod = 1, length = 0;
	char arr[6] = {0,};

	snprintf(arr, sizeof(arr), "%d", num);
	length = strlen(arr);
	while(length) {
		mod = 1;
		switch(length--) {
			case 5:
				mod *= 10;
			case 4:
				mod *= 10;
			case 3:
				mod *= 10;
			case 2:
				mod *= 10;
			default:
				break;
		}
		if(num >= mod)
			convert_print(num / mod, length);
		num %= mod;
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	int num = 0;
	printf("This is Number to Korean converter program.\nEnter Number: ");
	scanf("%d", &num);

	if(num < 1 || num > 99999) {
		fprintf(stderr, "Out of range! Range is 1 ~ 99999\n");
		exit(1);
	}

	converter(num);

	return 0;
}
