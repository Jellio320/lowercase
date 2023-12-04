#include <config.h>

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define MODE_LOWER 0
#define MODE_UPPER 1
#define MODE_FLIP 2

char mode = MODE_LOWER;
FILE *input = NULL, *output = NULL;

const char PROGRAM_NAME[10] = PACKAGE_NAME;
const char *COPYRIGHT =
	"Copyright (C) 2023 Jaymes Elliott <" PACKAGE_BUGREPORT ">";

void usage(void) {
	printf("Usage: %s [OPTIONS]... [FILE]\n\n"
		   "Options:\n"
		   "	-f, --file=FILE		convert characters' in FILE;\n"
		   "	-F, --flip		convert characters' casing into its opposite;\n"
		   "	-h, --help		prints this help menu;\n"
		   "	-L, --lower		convert characters' casing into lowercase "
		   "(default);\n"
		   "	-o, --output=FILE	prints converted characters into FILE;\n"
		   "	-U, --upper		convert characters' casing into uppercase;\n"
		   "	-v, --version		prints version information;\n\n"
		   "%s\n" PACKAGE_NAME " repository <" PACKAGE_URL ">\n\n",
		   PROGRAM_NAME, COPYRIGHT);
}

int getoptions(int argc, char **argv) {
	opterr = 0;
	struct option loptions[9] = {{"file", required_argument, 0, 'f'},
								 {"flip", no_argument, 0, 'F'},
								 {"help", no_argument, 0, 'h'},
								 {"lower", no_argument, 0, 'L'},
								 {"output", required_argument, 0, 'o'},
								 {"upper", no_argument, 0, 'U'},
								 {"verbose", no_argument, &opterr, 1},
								 {"version", no_argument, 0, 'v'},
								 {NULL, 0, NULL, 0}};

	int index = 0, option = 0, passed = 0;
	while ((option = getopt_long(argc, argv, ":f:FhLo:Uv", loptions, &index)) !=
		   -1) {

		switch (option) {

		case 0:
			if (loptions[index].flag != 0)
				break;

			printf("options '%s'", loptions[index].name);
			if (optarg)
				printf(" with arg '%s'", optarg);

			putchar('\n');
			break;

		case 'f':
			/* set input file */
			if (input == stdin) {
				input = fopen(optarg, "r");
				if (input == NULL) {
					perror(PROGRAM_NAME);
					return -1;
				}
			}
			break;

		case 'F':
			mode = MODE_FLIP;
			break;

		case 'h':
			usage();
			exit(EXIT_SUCCESS);
			break;

		case 'L':
			mode = MODE_LOWER;
			break;

		case 'o':
			/* set output file */
			if (output == stdout) {
				output = fopen(optarg, "w");
				if (output == NULL) {
					perror(PROGRAM_NAME);
					return -1;
				}
			}
			break;

		case 'U':
			mode = MODE_UPPER;
			break;

		case 'v':
			printf("%s v" VERSION "\n", PROGRAM_NAME);
			puts(COPYRIGHT);
			exit(EXIT_SUCCESS);
			break;

		case '?':
			puts("Unknown option");
			return -1;
			break;

		case ':':
			puts("Missing arguments");
			return -1;
			break;

		default:
			puts("Unknown argument");
			break;
		}

		passed++;
	}

	return passed;
}

int main(int argc, char **argv) {
	input = stdin;
	output = stdout;
	if (getoptions(argc, argv) == -1)
		return EXIT_FAILURE;

	if (input == stdin && optind < argc) {
		input = fopen(argv[optind], "r");
		if (input == NULL) {
			perror(PROGRAM_NAME);
			return EXIT_FAILURE;
		}
	}

	int c = 0;
	while ((c = fgetc(input)) != EOF) {

		switch (mode & 0x03) {

		default:
		case MODE_LOWER:
			if (isupper(c))
				c = tolower(c);
			break;

		case MODE_UPPER:
			if (islower(c))
				c = toupper(c);
			break;

		case MODE_FLIP:
			/*if(islower(c))
				c = toupper(c);
			else if(isupper(c))
				c = tolower(c);*/
			if (isalpha(c))
				c = c ^ 0x20; /* 32 0100000 */
			break;
		}

		fputc(c, output);
	}

	if (errno) {
		perror(PROGRAM_NAME);
		return EXIT_FAILURE;
	}

	/* if(input != stdin) */
	fclose(input);

	/* if(output != stdout) */
	fclose(output);

	return EXIT_SUCCESS;
}
