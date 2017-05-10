/**
 * This tool index a later chromosome to speed up the search of unmapped reads.
 * It works for all species with the knowledge of a "late" chromosome. For human
 * and mouse, the default chrM should work. The input bam must be sorted and 
 * indexed by samtools.
 *
 *
 * The MIT License (MIT)
 * Copyright (c) 2016 Wanding.Zhou@vai.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#include <stdio.h>
#include <string.h>
#include "sam.h"

/* this utility gets unmapped reads from the end of bam file assuming existence of chrM */

void quickunmap(char *infn, char *reg) {
	samfile_t *in = samopen(infn, "rb", 0);
  if (!in) {
    fprintf(stderr, "Cannot open input bam %s.\n", infn);
    fflush(stderr);
    exit(1);
  }
	samfile_t *out = samopen("-", "wh", in->header);
	bam_index_t *idx = bam_index_load(infn);
	int tid, beg, end;
	bam_parse_region(in->header, reg, &tid, &beg, &end);
	bam_iter_t iter = bam_iter_query(idx, tid, beg ,end);
	bam1_t *b = bam_init1();
	int ret;
	bam_iter_seek(in->x.bam, iter);
	int cnt_unmapped = 0;
	while ((ret = bam_read1(in->x.bam, b)) >= 0) {
		if (b->core.flag & BAM_FUNMAP) {
			samwrite(out, b);
			/* char *s = bam_format1_core(in->header, b, BAM_OFDEC); */
			/* puts(s); */
			/* free(s); */
      cnt_unmapped++;
		}
	}
	bam_destroy1(b);
	bam_index_destroy(idx);
	samclose(in);
	samclose(out);
	fprintf(stderr, "[%s:%d] Found %d unmapped reads\n", __func__, __LINE__, cnt_unmapped);
	fflush(stderr);
}


static int usage() {
  fprintf(stderr, "\n");
  fprintf(stderr, "Usage: quickunmap [options] in.bam\n");
  fprintf(stderr, "Input options:\n");
	fprintf(stderr, "     in.bam    input bam, must be sorted and indexed.\n");
	fprintf(stderr, "     -g        region (optional, jump to region before processing), typically a late chromosome.\n");
  fprintf(stderr, "     -h        this help.\n");
  fprintf(stderr, "\n");
  return 1;
}

int main(int argc, char *argv[]) {
	int c;
	char *reg = 0;										/* region */
	char *infn = 0;
  if (argc < 2) return usage();
  while ((c = getopt(argc, argv, "g:h")) >= 0) {
    switch (c) {
		case 'g': reg = optarg; break;
    case 'h': return usage();
    default:
      fprintf(stderr, "[%s:%d] Unrecognized command: %c.\n", __func__, __LINE__, c);
      exit(1);
      break;
    }
  }

  if (optind < argc)
    infn = argv[optind];
  else {
    fprintf(stderr, "Please provide input bam. Abort.\n");
    fflush(stderr);
    exit(1);
  }
    

  if (!reg) {
    fprintf(stderr, "No \"late\" chromosome specified, use chrM\n");
    fflush(stderr);
    reg = strdup("chrM");
  }

	quickunmap(infn, reg);

	return 0;
}
