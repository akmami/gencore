#ifndef CHTSLIB_H
#define CHTSLIB_H

#include <string>
#include <stdexcept>
#include <htslib/sam.h>
#include <htslib/bgzf.h>
#include <sys/stat.h>

class chtslib {
public:
    samFile *fp_in;
    bam_hdr_t *bam_hdr;
    hts_idx_t *bam_file_index;

    chtslib(const char* filename);
    ~chtslib();

    int read(bam1_t *aln);
    hts_itr_t* create_iter(int tid, hts_pos_t beg, hts_pos_t end);
};

void destroy_bam_aln(bam1_t *aln);
void destroy_iter(hts_itr_t* iter);

#endif