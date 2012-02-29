#include "compress.h"
#include <stdio.h>

z_stream strm;
FILE *out;

void unfunc(const unsigned char* buff, int len) {
	fwrite(buff, 1, len, out);
	return;
	for(int i = 0; i < len; i++)
		putchar(buff[i]);
}

void func(const unsigned char* buff, int len) {
	uncompress(&strm, buff, len, unfunc);
}

int main(int argc, char *argv[]) {
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	inflateInit(&strm);

	out = fopen("output.txt", "w");
	compress(argv[1], func);	
	printf("%d %d\n", strm.total_in, strm.total_out);
	inflateEnd(&strm);
	fclose(out);
	return 0;
}

