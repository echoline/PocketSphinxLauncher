#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <linux/limits.h>
#define PATH_MAX 65535

int main(int argc, char **argv) {
	int i, j, len, res, total;
	char **inputs, chr, chrhex[10], buf[1024], filename[PATH_MAX];
	FILE *f = NULL, *out;

	if (argc < 3) {
		fprintf(stderr, "usage: %s [inputfile ...] [outputfile.h]\n",
			argv[0]);
		return -1;
	}

	out = fopen(argv[argc-1], "ab");
	if (!out) {
		perror(argv[argc-1]);
		return -1;
	}

	inputs = (char**)calloc(1, sizeof(char*)*argc);
	for (i = 1; i < argc - 1; i++) {
		strcpy(filename, argv[i]);
		i++;
		f = fopen(filename, "r");
		perror(filename);
		if (!f) continue;
		while (strcspn(filename, ".") < strlen(filename))
			filename[strcspn(filename, ".")] = '_';
		sprintf(buf, "char %s[] = \"", filename);
		total = 0;
		len = strlen(buf);
		while ((res = fwrite(buf + total, 1, len - total, out)) <
				(len - total) && (res > 0)) total += res;
		while ((chr = fgetc(f)) != EOF) {
			sprintf(chrhex, "\\x%02X", chr);
			len = strlen(chrhex);
			while (fwrite(chrhex, 1, strlen(chrhex), out) == 0) perror("chrhex");
		}
		sprintf(buf, "\";\n\n", argv[i]);
		total = 0;
		len = strlen(buf);
		while ((res = fwrite(buf + total, 1, len - total, out)) <
				(len - total) && (res > 0)) total += res;
		fclose(f);
	}

	fclose(out);

	return 0;
}
