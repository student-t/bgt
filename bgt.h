#ifndef BGT_H
#define BGT_H

#include "vcf.h"
#include "pbwt.h"

typedef struct {
	int n_samples;
	char **samples;
	void *h_samples; // hash table
	pbf_t *pb;
	htsFile *bcf;
	bcf_hdr_t *h0; // site-only VCF header
	hts_idx_t *idx; // BCF index
	hts_itr_t *itr;

	int n_sub, *sub;
	bcf_hdr_t *h_sub;
} bgt_t;

bgt_t *bgt_open(const char *prefix);
void bgt_close(bgt_t *bgt);

#endif
