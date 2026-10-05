#include <stdio.h>


int main(int argc, char* argv[]) {
	if (argc != 2 || argv[1] == NULL) {
		printf("Child: error of child's arguments\n");
		return 1;
	}

	FILE* f = fopen(argv[1], "w");
	if (!f) {
		perror("fopen");
		return 1;
	}

	char cin[256];
	float a, b, c;
	while (fgets(cin, sizeof(cin), stdin)) {
		if (sscanf(cin, "%f %f %f", &a, &b, &c) != 3) continue;
		if (b == 0.0 || c == 0.0) {
			printf("Child: error of division zero\n");
			fflush(stdout);
			fclose(f);
			return 2;
		}
		a /= b;
		a /= c;
		fprintf(f, "%f\n", a);
	}

	fclose(f);

	return 0;
}
