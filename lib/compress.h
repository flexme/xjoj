/*
 *  Copyright (C) <2008>  <chenkun cs.kunchen@gmail.com>

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __COMPRESS_H
#define __COMPRESS_H

#include <zlib.h>
#include <stdio.h>

#define CHUNK 16384

template <class FUNC_T>
int compress(const char* filename, FUNC_T *handler) {
	int ret, flush;
	unsigned int have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp == NULL)
		return Z_ERRNO;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if(ret != Z_OK) {
		fclose(fp);
		return ret;
	}

	do {
		strm.avail_in = fread(in, 1, CHUNK, fp);
		if(ferror(fp)) {
			deflateEnd(&strm);
			fclose(fp);
			return Z_ERRNO;
		}

		flush = feof(fp) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;

		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;

			ret = deflate(&strm, flush);
			have = CHUNK - strm.avail_out;
			(*handler)(out, have);
		} while(strm.avail_out == 0);
	} while(flush != Z_FINISH);

	deflateEnd(&strm);
	fclose(fp);
	return Z_OK;
}

template <class FUNC_T>
int uncompress(z_stream *strm, const unsigned char* buf, int len, FUNC_T *handler) {
	int ret;
	unsigned int have;
	unsigned char out[CHUNK];

	if(len <= 0)
		return Z_OK;

	strm->avail_in = len;
	strm->next_in = (unsigned char *)buf;
	
	do {
		strm->avail_out = CHUNK;
		strm->next_out = out;

		ret = inflate(strm, Z_NO_FLUSH);
		switch(ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(strm);
				return ret;
		}

		have = CHUNK - strm->avail_out;
		(*handler)(out, have);
	} while(strm->avail_out == 0);

	return ret == Z_DATA_ERROR ? Z_DATA_ERROR : Z_OK;
}


#endif

