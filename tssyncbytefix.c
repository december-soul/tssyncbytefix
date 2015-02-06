/*
 *	This tiny tool helps to fix Mpeg-TS files which are out of 0x47 sync.
 *      A Mpeg-TS requires an 0x47 each 188 bytes. This marks an TS Pkg start.
 *	Some tools relying on the sync byte.
 *      if the files get out of sync by missing some bytes in the middle we can 
 *      fix it with this tool.
 *
 *     compile: g++ tssyncbytefix.c -o tssyncbytefix
 *     usage:   ./tssyncbytefix test.ts test_sync_fixed.ts
*/

#include <stdio.h>
#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <sys/types.h>
#include <unistd.h>

int findSyncByte (char *buf, size_t size) {
  int i = 0;
  for (i = 0; i < size; i++) {
      if (buf[i] == 0x47)
	break;
  }
  return i;
}

int main(int argc, char *argv[]) {

    size_t BUFFER_SIZE = 188;

    char buf[BUFFER_SIZE];
    size_t size;
    size_t offset = 0;

    int source = open(argv[1], O_RDONLY, 0);
    int dest = open(argv[2], O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);

    while ((size = read(source, buf, BUFFER_SIZE)) > 0) {
      long int currentpos = lseek(source, 0, SEEK_CUR);
      offset = findSyncByte(buf, size);
      if(offset == 0) {
        write(dest, buf, size);
      } else {
	printf("found sync byte at offset %d of filepos %ld\n", (int)offset, currentpos);
	lseek(source, currentpos - 188 + offset, SEEK_SET);
	printf("set fd to %ld\n", lseek(source, 0, SEEK_CUR));
      }
    }

    close(source);
    close(dest);
    return 0;
}
