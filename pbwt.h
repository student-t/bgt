#ifndef PBWT_H
#define PBWT_H

#include <stdint.h>

typedef struct { // full codec
	int32_t m, l, *S0, *S;
	uint8_t *u;
} pbc_t;

typedef struct {
	uint32_t r;
	uint32_t i:31, b:1;
} pbs_dat_t;

struct pbf_s;
typedef struct pbf_s pbf_t;

#ifdef __cplusplus
extern "C" {
#endif

/***********************************
 * File-based high-level functions *
 ***********************************/

/**
 * Open PBF file for write
 *
 * @param fn     file name. NULL or "-" for stdout
 * @param m      number of columns
 * @param g      number of groups
 * @param shift  keeping S every 1<<shift rows
 */
pbf_t *pbf_open_w(const char *fn, int m, int g, int shift);

/**
 * Open PBF for read
 *
 * @param fn     file name. NULL or "-" for stdin
 */
pbf_t *pbf_open_r(const char *fn);

/**
 * Close a PBF file handler and deallocate memory
 *
 * @param pb     PBF file handler
 */
int pbf_close(pbf_t *pb);

/**
 * Write one group to PBF
 *
 * @param pb     PBF file handler
 * @param a      g-by-m matrix to encode
 */
int pbf_write(pbf_t *pb, uint8_t *const*a);

/**
 * Read one group from PBF
 *
 * @param pb     PBF file handler
 * @return  g-by-m matrix of decoded bits if pbf_subset() has not been called
 *          or g-by-n_sub matrix if called.
 */
const uint8_t **pbf_read(pbf_t *pb);

/**
 * Seek to a specified row
 *
 * @param pb     PBF file handler
 * @param k      seek to the k-th row
 */
int pbf_seek(pbf_t *pb, uint64_t k);

/**
 * Specify a subset of columns to decode
 *
 * @param pb     PBF file handler
 * @param n_sub  number of columns to decode; 0 for decoding all columns
 * @param sub    indicies of columns to decode
 */
int pbf_subset(pbf_t *fp, int n_sub, int *sub);

int pbf_get_g(const pbf_t *pb);
int pbf_get_m(const pbf_t *pb);
int pbf_get_n(const pbf_t *pb);
int pbf_get_shift(const pbf_t *pb);

/***********************
 * Low-level functions *
 ***********************/

/**
 * Initialize a PBWT codec with $m columns
 *
 * @param m   number of columns
 * @return    pointer to a pbc_t struct. It should be freed with free().
 */
pbc_t *pbc_init(int m);

/**
 * Encode a bit string
 *
 * @param pb   codec
 * @param a    bit string
 * @return The transformed run-length encoded string is kept in pb->u. pb->l
 *         gives the length of the encoded string.
 */
void pbc_enc(pbc_t *pb, const uint8_t *a);

/**
 * Decode a transformed run-length encoded string
 *
 * The decoded string is stored in pb->u.
 *
 * @param pb   codec
 * @param b    encoded string generated by pbc_enc()
 */
void pbc_dec(pbc_t *pb, const uint8_t *b);

/**
 * Decode a subset of columns without decoding all columns
 *
 * @param m       total number of columns
 * @param n_sub   number of columns to decode
 * @param sub     S(sub[i].r)=sub[i].S gives the column index to decode
 * @param u       encoded string generated by pbc_enc()
 * @return  number of 1 bit in the full PBWT. If the PBWT contains both 0 and 1,
 *          decoded bits stored in sub[i].b and sub[i].r is updated for the next row.
 *          If the PBWT is full of 0 or full of 1, sub[i].{r,b} are not modified.
 */
int pbs_dec(int m, int n_sub, pbs_dat_t *sub, const uint8_t *u);

#ifdef __cplusplus
}
#endif

#endif
