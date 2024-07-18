#include "chtslib.h"


chtslib::chtslib(const char* filename) : fp_in(nullptr), bam_hdr(nullptr), bam_file_index(nullptr) {
    this->fp_in = sam_open(filename, "r");
    if (!this->fp_in) {
        throw std::runtime_error("Error opening file: " + std::string(filename));
    }
    this->bam_hdr = sam_hdr_read(this->fp_in);
    if (this->bam_hdr == NULL) {
        sam_close(this->fp_in);
        throw std::runtime_error("Error reading BAM header");
    }
    this->bam_file_index = sam_index_load(this->fp_in, this->fp_in->fn);
    if (!this->bam_file_index) {
        throw std::runtime_error("Error creating BAM index");
    }
};


chtslib::~chtslib() {
    if (this->bam_hdr) {
        bam_hdr_destroy(this->bam_hdr);
        this->bam_hdr = nullptr;
    }
    if (this->fp_in) {
        sam_close(this->fp_in);
        this->fp_in = nullptr;
    }
    if (this->bam_file_index) {
        hts_idx_destroy(this->bam_file_index);
        this->bam_file_index = nullptr;
    }
};


int chtslib::read(bam1_t *aln) {
    return sam_read1(this->fp_in, this->bam_hdr, aln);
};


hts_itr_t* chtslib::create_iter(int tid, hts_pos_t beg, hts_pos_t end) {
    return sam_itr_queryi( this->bam_file_index, tid, beg, end);
};


void destroy_bam_aln(bam1_t *aln) {
    if (aln) {
         bam_destroy1(aln);
    }
    aln = nullptr;
};


void destroy_iter(hts_itr_t* iter) {
    if (iter) {
        sam_itr_destroy(iter);
    }
    iter = nullptr;
};