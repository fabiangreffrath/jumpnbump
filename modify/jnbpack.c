/*
 * pack.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 * 
 * Copyright (C) 2001 "timecop" <timecop@japan.co.jp>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * This file is part of Jump'n'Bump.
 *
 * Jump'n'Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump'n'Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#ifdef LINUX
#include <getopt.h>
#endif
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <io.h>
#endif

typedef struct {
	char filename[12];
	unsigned int offset;
	unsigned int size;
} DirEntry;

#ifndef O_BINARY
#define O_BINARY 0
#endif

int main(int argc, char **argv)
{
	int fd;
	DirEntry *datafile;
	int num_entries, i;
	int c;
	char *outfile = NULL;
	int offset = 0;

#ifdef LINUX
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
	fd = open(outfile, O_RDWR | O_CREAT | O_BINARY, 0644);
	if (fd == -1) {
		perror("opening packfile");
		exit(1);
	}
	printf("Opened %s\n", outfile);

	/* write number of entries in this data file */
	{
		char temp;

		temp = (num_entries >>  0) & 0xff;
		write(fd, &temp, 1);
		temp = (num_entries >>  8) & 0xff;
		write(fd, &temp, 1);
		temp = (num_entries >> 16) & 0xff;
		write(fd, &temp, 1);
		temp = (num_entries >> 24) & 0xff;
		write(fd, &temp, 1);
	}

	/* write the directory structure */
	for (i = 0; i < num_entries; i++) {
		char temp;

		write(fd, &datafile[i].filename, 12);
		temp = (datafile[i].offset >>  0) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].offset >>  8) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].offset >> 16) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].offset >> 24) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].size >>  0) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].size >>  8) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].size >> 16) & 0xff;
		write(fd, &temp, 1);
		temp = (datafile[i].size >> 24) & 0xff;
		write(fd, &temp, 1);
	}

	/* write in the actual files */
	for (i = 0; i < num_entries; i++) {
		int infd;
		char *buf;

		printf("adding %s ", argv[i]);

		infd = open(argv[i], O_RDONLY | O_BINARY);
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
