#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#ifndef _MSC_VER
#include <getopt.h>
#include <unistd.h>
#else
#include <io.h>
typedef unsigned int u_int32_t;
#endif

typedef struct {
	char filename[12];
	u_int32_t offset;
	u_int32_t size;
} DirEntry;

int main(int argc, char **argv)
{
	int fd;
	DirEntry *datafile;
	int num_entries, i;
	int c;
	char *outfile = NULL;
	int offset = 0;

#ifndef _MSC_VER
	while ((c = getopt(argc, argv, "o:")) != EOF) {
		switch (c) {
		case 'o':
			if (optarg) {
				outfile = strdup(optarg);
			}
			break;
		}
	}
	argc -= optind;
	argv += optind;
#else
	c = 1;
	while (c<argc) {
		if (argv[c][0]=='-') {
			if (argv[c][1]=='o') {
				if ( ((c+1)<argc) && (argv[c+1]) ) {
					outfile = strdup(argv[c+1]);
					c++;
				}
				c++;
				break;
			}
		}
		c++;
	}
	argc -= c;
	argv += c;
#endif

	if (outfile == NULL) {
		printf("You must specify output filename with -o\n");
		exit(1);
	}
	if (argc == 0) {
		printf("You must specify some files to pack, duh\n");
		exit(1);
	}
	num_entries = argc;

	/* prepare to pack things - get sizes and calculate offsets */
	printf("%u files to pack\n", num_entries);
	datafile = calloc(num_entries, sizeof(DirEntry));

	/* skip past the directory structure */
	offset = 4 + (num_entries * 20);

	for (i = 0; i < num_entries; i++) {
		struct stat dummy;
		if (stat(argv[i], &dummy)) {
			fprintf(stderr, "%s is not accessible: ", argv[i]);
			perror("");
			exit(1);
		}
		if (strlen(argv[i]) > 12) {
			fprintf(stderr, "filename %s is longer than 12 chars\n", argv[i]);
			exit(1);
		}
		strncpy(datafile[i].filename, argv[i], 12);
		datafile[i].size = dummy.st_size;
		/* num_entries * (12 + 8) */
		datafile[i].offset = offset;
		offset += datafile[i].size;
	}

	/* here, we checked that all files are ok, and ready to roll the packfile */
	fd = open(outfile, O_RDWR | O_CREAT, 0644);
	if (fd == -1) {
		perror("opening packfile");
		exit(1);
	}
	printf("Opened %s\n", outfile);

	/* write number of entries in this data file */
	write(fd, &num_entries, 4);

	/* write the directory structure */
	for (i = 0; i < num_entries; i++) {
		write(fd, &datafile[i], 20);
	}

	/* write in the actual files */
	for (i = 0; i < num_entries; i++) {
		int infd;
		char *buf;

		printf("adding %s ", argv[i]);

		infd = open(argv[i], O_RDONLY);
		if (infd == -1) {
			perror("opening file");
			exit(1);
		}
		buf = malloc(datafile[i].size + 16);
		read(infd, buf, datafile[i].size);
		close(infd);
		write(fd, buf, datafile[i].size);
		free(buf);
		printf(" OK\n");
	}
	close(fd);
	return 0;
}
