#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#ifndef _MSC_VER
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

    if (argc < 2) {
	printf("dumbass, specify filename to unpack\n");
	exit(1);
    }

    fd = open(argv[1], O_RDONLY);
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

	outfd = open(filename, O_RDWR | O_CREAT, 0644);
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
