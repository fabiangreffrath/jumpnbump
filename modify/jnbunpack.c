/*
 * unpack.c
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
#include <string.h>
#include <sys/types.h>
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

    if (argc < 2) {
	printf("dumbass, specify filename to unpack\n");
	exit(1);
    }

    fd = open(argv[1], O_RDONLY | O_BINARY);
    if (fd == -1) {
	perror("open datafile");
	exit(1);
    }
    /* get number of entries */
    read(fd, &num_entries, 4);

    printf("%d entries in datafile\n", num_entries);

    datafile = calloc(num_entries, sizeof(DirEntry));
    read(fd, datafile, num_entries * sizeof(DirEntry));
    printf("Directory Listing:\n");
    for (i = 0; i < num_entries; i++) {
	char filename[14];
	memset(filename, 0, sizeof(filename));
	strncpy(filename, datafile[i].filename, 12);
	printf("%02d:\t%s (%u bytes)\n", i, filename,
		datafile[i].size);
    }

    for (i = 0; i < num_entries; i++) {
	int outfd;
	char filename[14];
	char *buf;
	memset(filename, 0, sizeof(filename));
	strncpy(filename, datafile[i].filename, 12);
	printf("Extracting %s ", filename);

	outfd = open(filename, O_RDWR | O_CREAT | O_BINARY, 0644);
	if (!outfd) {
	    perror("cant open file");
	    exit(1);
	}
	lseek(fd, datafile[i].offset, SEEK_SET);
	buf = calloc(1, datafile[i].size + 16);
	read(fd, buf, datafile[i].size);
	write(outfd, buf, datafile[i].size);
	close(outfd);
	free(buf);
	printf("OK\n");
    }
    close(fd);
    return 0;
}
