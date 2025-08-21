#include <stdio.h>      
#include <stdlib.h>     
#include <stdint.h>     
#include <string.h>     
#include <stdbool.h>    
#include <fcntl.h>      
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/stat.h>   
#include <time.h>       

#define BLOCK_SIZE 4096              
#define T_BLOCKS 64              
#define INODE_SIZE 256               
#define INODE_C 80               
#define INODE_BITMAP 1         
#define DATA_BITMAP 2          
#define INODE_TABLE 3    
#define DATA_BLOCK 8           
#define MAGIC_NUM 0xD34D          

typedef struct {
    uint16_t magic;               
    uint32_t block_size;         
    uint32_t t_block;        
    uint32_t inode_bitmap;  
    uint32_t data_bitmap;   
    uint32_t inode_table; 
    uint32_t first_data;    
    uint32_t inode_size;          
    uint32_t inode_c;         
    uint8_t reserved[4058];       
} SuperBlock;

typedef struct {
    uint32_t mode;                
    uint32_t uid;                 
    uint32_t gid;                 
    uint32_t size;                
    uint32_t atime;               
    uint32_t ctime;               
    uint32_t mtime;               
    uint32_t dtime;               
    uint32_t nlinks;              
    uint32_t num_blocks;          
    uint32_t direct_blocks[12];   
    uint32_t single_indirect;     
    uint32_t double_indirect;     
    uint32_t triple_indirect;     
    uint8_t reserved[156];        
} Inode;

SuperBlock superblock;            
uint8_t inode_bitmap[BLOCK_SIZE]; 
uint8_t data_bitmap[BLOCK_SIZE];  
Inode *inodes = NULL;             
bool *data_block_refered = NULL; 
int *data_block_refered_by = NULL; 
int total_errors = 0;             


void read_block(int fd, int block_num, void *buffer) {
    lseek(fd, block_num * superblock.block_size, SEEK_SET); 
    read(fd, buffer, superblock.block_size);                
}


bool check_bit(uint8_t *bitmap, int index) {
    int byte_index = index / 8;        
    int bit_offset = index % 8;        
    return (bitmap[byte_index] & (1 << bit_offset)) != 0; 
}


bool superblock_vali() {
    printf("\n1. Superblock Validator\n");
    bool is_valid = true;         

    printf("Magic number: 0x%X\n", superblock.magic);
    printf("Block size: %u bytes\n", superblock.block_size);
    printf("Total blocks: %u\n", superblock.t_block);
    printf("Inode bitmap block: %u\n", superblock.inode_bitmap);
    printf("Data bitmap block: %u\n", superblock.data_bitmap);
    printf("Inode table start block: %u\n", superblock.inode_table);
    printf("First data block: %u\n", superblock.first_data);
    printf("Inode size: %u bytes\n", superblock.inode_size);
    printf("Inode count: %u\n", superblock.inode_c);

    if (superblock.magic != MAGIC_NUM) {
        printf("invalid magic number. needed  0x%X, got 0x%X\n", MAGIC_NUM, superblock.magic);
        is_valid = false;
        total_errors++;
    }
    if (superblock.block_size != BLOCK_SIZE) {
        printf("invalid block size. needed %d, got %d\n", BLOCK_SIZE, superblock.block_size);
        is_valid = false;
        total_errors++;
    }
    if (superblock.t_block != T_BLOCKS) {
        printf("invalid total blocks. needed %d, got %d\n", T_BLOCKS, superblock.t_block);
        is_valid = false;
        total_errors++;
    }
    if (superblock.inode_bitmap != INODE_BITMAP) {
        printf("invalid inode bitmap block. needed %d, got %d\n", INODE_BITMAP, superblock.inode_bitmap);
        is_valid = false;
        total_errors++;
    }
    if (superblock.data_bitmap != DATA_BITMAP) {
        printf("invalid data bitmap block. needed %d, got %d\n", DATA_BITMAP, superblock.data_bitmap);
        is_valid = false;
        total_errors++;
    }
    if (superblock.inode_table != INODE_TABLE) {
        printf("invalid inode table start block. needed %d, got %d\n", INODE_TABLE, superblock.inode_table);
        is_valid = false;
        total_errors++;
    }
    if (superblock.first_data != DATA_BLOCK) {
        printf("invalid first data block. needed %d, got %d\n", DATA_BLOCK, superblock.first_data);
        is_valid = false;
        total_errors++;
    }
    if (superblock.inode_size != INODE_SIZE) {
        printf("invalid inode size. needed %d, got %d\n", INODE_SIZE, superblock.inode_size);
        is_valid = false;
        total_errors++;
    }
    if (superblock.inode_c != INODE_C) {
        printf("invalid inode count. needed %d, got %d\n", INODE_C, superblock.inode_c);
        is_valid = false;
        total_errors++;
    }
    const char* sb_result;
    if (is_valid) {
        sb_result = "PASSED";
    } else {
        sb_result = "FAILED";
    }
    printf("Superblock validation: %s\n", sb_result);
    return is_valid;
}

bool inode_vali(int inode_index) {
    return inodes[inode_index].nlinks > 0 && inodes[inode_index].dtime == 0;
}




void inode_bitmap_check() {
    printf("\n3. Inode Bitmap Checker\n");
    int errors = 0;               
    int valid_inodes = 0;         
    int used_in_bitmap = 0;       

    int i = 0;
    while (i < superblock.inode_c) {
        if (check_bit(inode_bitmap, i)) {
            used_in_bitmap++;     
        }
        if (inode_vali(i)) {
            valid_inodes++;       
        }
        i++;
    }

    printf("Inodes marked as used in bitmap: %d\n", used_in_bitmap);
    printf("Valid inodes found: %d\n", valid_inodes);

    i = 0;
    while (i < superblock.inode_c) {
        if (check_bit(inode_bitmap, i)) {
            if (!inode_vali(i)) {
                printf("Inode %d is marked as used in bitmap but is not valid (nlinks=%u, dtime=%u)\n", i, inodes[i].nlinks, inodes[i].dtime);
                errors++;
                total_errors++;
            }
        }
        else {
            if (inode_vali(i)) {
                printf("Inode %d is valid but not marked as used in bitmap\n", i);
                errors++;
                total_errors++;
            }
        }
        i++;
    }

    const char* inode_result;
    if (errors == 0) {
        inode_result = "PASSED";
    } else {
        inode_result = "FAILED";
    }
    printf("Inode bitmap consistency check: %s (%d errors)\n", inode_result, errors);
}


bool block_check(uint32_t block_num) {
    return block_num >= superblock.first_data && block_num < superblock.t_block;
}


void process_inode_blocks(int inode_index) {
    Inode *inode = &inodes[inode_index];  
    
    int i = 0;
    while (i < 12) {
        uint32_t block_num = (*inode).direct_blocks[i];
        if (block_num != 0) {     
            if (block_check(block_num)) {
                int data_block_index = block_num - superblock.first_data;  
                data_block_refered[data_block_index] = true;  
                
                if (data_block_refered_by[data_block_index] == -1) {
                    data_block_refered_by[data_block_index] = inode_index;  
                }
            }
        }
        i++;
    }
    
    if ((*inode).single_indirect != 0) {
        if (block_check((*inode).single_indirect)) {
            int data_block_index = (*inode).single_indirect - superblock.first_data;
            data_block_refered[data_block_index] = true;
            
            if (data_block_refered_by[data_block_index] == -1) {
                data_block_refered_by[data_block_index] = inode_index;
            }
        }
    }
    
    if ((*inode).double_indirect != 0) {
        if (block_check((*inode).double_indirect)) {
            int data_block_index = (*inode).double_indirect - superblock.first_data;
            data_block_refered[data_block_index] = true;
            
            if (data_block_refered_by[data_block_index] == -1) {
                data_block_refered_by[data_block_index] = inode_index;
            }
        }
    }
    
    if ((*inode).triple_indirect != 0) {
        if (block_check((*inode).triple_indirect)) {
            int data_block_index = (*inode).triple_indirect - superblock.first_data;
            data_block_refered[data_block_index] = true;
            
            if (data_block_refered_by[data_block_index] == -1) {
                data_block_refered_by[data_block_index] = inode_index;
            }
        }
    }
}

void track_blocks() {
    int d_count = superblock.t_block - superblock.first_data;  
    
    int i = 0;
    while (i < d_count) {
        data_block_refered[i] = false;        
        data_block_refered_by[i] = -1;        
        i++;
    }

    i = 0;
    while (i < superblock.inode_c) {
        if (inode_vali(i)) {                 
            process_inode_blocks(i);             
        }
        i++;
    }
}

void data_bitmap_check() {
    printf("\n2. Data Bitmap Consistency Checker\n");
    int errors = 0;                               
    int d_count = superblock.t_block - superblock.first_data;  
    int db_used = 0;                   
    int blocks_refered = 0;                    

    int i = 0;
    while (i < d_count) {
        if (check_bit(data_bitmap, i)) {
            db_used++;                 
        }
        if (data_block_refered[i]) {
            blocks_refered++;                  
        }
        i++;
    }

    printf("Data blocks marked as used in bitmap: %d\n", db_used);
    printf("Data blocks refered by valid inodes: %d\n", blocks_refered);

    i = 0;
    while (i < d_count) {
        int block_num = i + superblock.first_data;  
        if (check_bit(data_bitmap, i)) {
            if (!data_block_refered[i]) {
                printf("Block %d is marked used in bitmap but not refered by any inode\n", block_num);
                errors++;
                total_errors++;
            }
        }
        else {
            if (data_block_refered[i]) {
                printf("Block %d is refered by inode %d but not marked as used in bitmap\n", block_num, data_block_refered_by[i]);
                errors++;
                total_errors++;
            }
        }
        i++;
    }

    const char* data_result;
    if (errors == 0) {
        data_result = "PASSED";
    } else {
        data_result = "FAILED";
    }
    printf("Data bitmap consistency check: %s (%d errors)\n", data_result, errors);
}


void dup_check() {
    printf("\n4. Duplicate Checker\n");
    int d_count = superblock.t_block - superblock.first_data;  
    int duplicate_count = 0;      
    
    int *block_ref_count = calloc(d_count, sizeof(int));  
    if (!block_ref_count) {
        perror("Memory allocation  failed");
        return;
    }
    
    int i = 0;
    while (i < superblock.inode_c) {
        if (inode_vali(i)) {
            Inode *inode = &inodes[i];
            int j = 0;
            while (j < 12) {
                uint32_t block_num = (*inode).direct_blocks[j];
                if (block_num != 0 && block_check(block_num)) {  
                    int data_block_index = block_num - superblock.first_data;
                    block_ref_count[data_block_index]++;  
                }
                j++;
            }
            if ((*inode).single_indirect != 0 && block_check((*inode).single_indirect)) {
                int data_block_index = (*inode).single_indirect - superblock.first_data;
                block_ref_count[data_block_index]++;  
            }
            if ((*inode).double_indirect != 0 && block_check((*inode).double_indirect)) {
                int data_block_index = (*inode).double_indirect - superblock.first_data;
                block_ref_count[data_block_index]++;  
            }
            if ((*inode).triple_indirect != 0 && block_check((*inode).triple_indirect)) {
                int data_block_index = (*inode).triple_indirect - superblock.first_data;
                block_ref_count[data_block_index]++;  
            }
        }
        i++;
    }
    

    i = 0;
    while (i < d_count) {
        if (block_ref_count[i] > 1) {  
            printf("Block %d is refered by %d different inodes\n", i + superblock.first_data, block_ref_count[i]);
            duplicate_count++;
            total_errors++;
        }
        i++;
    }
    
    const char* dup_result;
    if (duplicate_count == 0) {
        dup_result = "PASSED";
    } else {
        dup_result = "FAILED";
    }
    printf("Duplicate blocks check: %s (%d duplicates found)\n", dup_result, duplicate_count);
    
    free(block_ref_count);  
}

void bad_block_check() {
    printf("\n5. Bad Block Checker\n");
    int bad_blocks = 0;  
    
    int i = 0;
    while (i < superblock.inode_c) {
        if (inode_vali(i)) {
            Inode *inode = &inodes[i];
            int j = 0;
            while (j < 12) {
                uint32_t block_num = (*inode).direct_blocks[j];
                if (block_num != 0) {  
                    if (block_num < superblock.first_data || block_num >= superblock.t_block) {
                        printf("Inode %d has bad direct block pointer %u (out of valid range %u-%u)\n", i, block_num, superblock.first_data, superblock.t_block - 1);
                        bad_blocks++;
                        total_errors++;
                    }
                }
                j++;
            }
            if ((*inode).single_indirect != 0) {
                if ((*inode).single_indirect < superblock.first_data || 
                    (*inode).single_indirect >= superblock.t_block) {
                    printf("Inode %d has bad single indirect block pointer %u (out of valid range %u-%u)\n", i, (*inode).single_indirect, superblock.first_data, superblock.t_block - 1);
                    bad_blocks++;
                    total_errors++;
                }
            }
            if ((*inode).double_indirect != 0) {
                if ((*inode).double_indirect < superblock.first_data || 
                    (*inode).double_indirect >= superblock.t_block) {
                    printf("Inode %d has bad double indirect block pointer %u (out of valid range %u-%u)\n", i, (*inode).double_indirect, superblock.first_data, superblock.t_block - 1);
                    bad_blocks++;
                    total_errors++;
                }
            }
            if ((*inode).triple_indirect != 0) {
                if ((*inode).triple_indirect < superblock.first_data || 
                    (*inode).triple_indirect >= superblock.t_block) {
                    printf("Inode %d has bad triple indirect block pointer %u (out of valid range %u-%u)\n", i, (*inode).triple_indirect, superblock.first_data, superblock.t_block - 1);
                    bad_blocks++;
                    total_errors++;
                }
            }
        }
        i++;
    }
    


    const char* result_str;
    if (bad_blocks == 0) {
        result_str = "PASSED";
    } else {
        result_str = "FAILED";
    }
    printf("Bad blocks check: %s (%d bad blocks found)\n", result_str, bad_blocks);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filesystem_image>\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];  
    
    int fd = open(image_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening  image");
        return 1;
    }

    printf("Analyzing file: %s\n", image_path);
    
    lseek(fd, 0, SEEK_SET);  
    read(fd, &superblock, sizeof(SuperBlock));  
    
    bool superblock_valid = superblock_vali();
    
    inodes = (Inode *)malloc(superblock.inode_c * sizeof(Inode));
    if (inodes == NULL) {
        perror("Error allocating memory for inodes");
        close(fd);
        return 1;
    }
    
    int d_count = superblock.t_block - superblock.first_data;
    data_block_refered = (bool *)malloc(d_count * sizeof(bool));
    data_block_refered_by = (int *)malloc(d_count * sizeof(int));
    
    if (data_block_refered == NULL || data_block_refered_by == NULL) {
        perror("Error allocating memory for data block tracking");
        free(inodes);
        if (data_block_refered) free(data_block_refered);
        if (data_block_refered_by) free(data_block_refered_by);
        close(fd);
        return 1;
    }
    read_block(fd, superblock.inode_bitmap, inode_bitmap);
    read_block(fd, superblock.data_bitmap, data_bitmap);
    int inodes_per_block = superblock.block_size / superblock.inode_size;
    printf("Inodes per block: %d\n", inodes_per_block);
    
    int i = 0;
    while (i < superblock.inode_c) {
        int block_offset = i / inodes_per_block;
        int inode_offset = i % inodes_per_block;
        
        int block_num = superblock.inode_table + block_offset;
        int offset = inode_offset * superblock.inode_size;
        
        lseek(fd, block_num * superblock.block_size + offset, SEEK_SET);
        
        read(fd, &inodes[i], superblock.inode_size);
        i++;
    }
    
    track_blocks();
    data_bitmap_check();    
    inode_bitmap_check();   
    dup_check();           
    bad_block_check();                 
    
    printf("File system check complete\n");
    printf("Total inconsistencies found: %d\n", total_errors);
    
    free(inodes);
    free(data_block_refered);
    free(data_block_refered_by);
    close(fd);
    
    return 0;
}
