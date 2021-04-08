#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "md5.c"

#define TREE_HEIGHT 3 
#define BLOCK_SIZE 1024
#define DATA_BLOCKS 4 

typedef struct { 
    char *hash;
} tree_node;

typedef struct {
    size_t n;
    size_t tree_height;
    size_t hash_size;
    size_t data_block_size;
    size_t data_blocks;
    tree_node *nodes;
} hash_tree;

void build_merkle_tree(hash_tree *mt, char **data);
void freeMerkleTree(hash_tree *mt);
void print_tree(hash_tree *mt);

void build_merkle_tree(hash_tree *mt, char *data[]) {

    if (mt->data_blocks > (1 << (mt->tree_height - 1))) exit(0);
    int i, leaf_node_start;
    leaf_node_start = (1 << (mt->tree_height - 1));
    mt->n = leaf_node_start + mt->data_blocks - 1;
    mt->nodes = (tree_node *)malloc(sizeof(tree_node) * (mt->n + 1));
    for (i = leaf_node_start; i <= mt->n; i++) {
        mt->nodes[i].hash = (char *)malloc(sizeof(char *) * mt->hash_size);
        md5((unsigned char*)data[i-leaf_node_start], mt->data_block_size, (unsigned char*)mt->nodes[i].hash);
    }
    for (i = leaf_node_start - 1; i > 0; i--) {
    	char *buffer = (char *)malloc(sizeof(char *) * (2 * mt->hash_size + 1));
        memcpy(buffer, mt->nodes[2*i].hash, mt->hash_size);
        memcpy(buffer+mt->hash_size, mt->nodes[2*i+1].hash, mt->hash_size);
        mt->nodes[i].hash = (char *)malloc(sizeof(char *) * mt->hash_size);
        md5((unsigned char*)buffer, 2*mt->hash_size, (unsigned char*)mt->nodes[i].hash);
        free(buffer);
    }

}

void freeMerkleTree(hash_tree *mt) {
    int i;
    if (!mt){
        return;
	}
    if (mt->nodes) {
        for (i=1; i<=mt->n; i++)
            if(mt->nodes[i].hash)
                free(mt->nodes[i].hash);
        free(mt->nodes);
    }
    return;
}

void print_tree(hash_tree *mt) {

    int i;
    printf("--------------------------------\n");

    for(i=1; i<=mt->n; i++) {
    	switch(i) {
        	case 1:
				MD5Print((unsigned char*)mt->nodes[i].hash);
				printf("\n\n");
				break;
			case 2:
				printf(" ");
				MD5Print((unsigned char*)mt->nodes[i].hash);
				break;
			case 3:
				printf(" ");
				MD5Print((unsigned char*)mt->nodes[i].hash);
				printf("\n\n");
				break;
			case 4:
				printf("  ");
				MD5Print((unsigned char*)mt->nodes[i].hash);
				break;
			case 5:
				printf("  ");
				MD5Print((unsigned char*)mt->nodes[i].hash);
				printf("\n\n");
				break;
			case 6:
				printf("  ");
				MD5Print((unsigned char*)mt->nodes[i].hash);
				break;
			case 7:
				printf("  ");
				MD5Print((unsigned char*)mt->nodes[i].hash);
				printf("\n");
				break;
        }
    }

    printf("--------------------------------\n");
    return;

}

int main()
{
	int i, j, f;
    char *data[DATA_BLOCKS], *data_copy[DATA_BLOCKS], *buffer[BLOCK_SIZE];
    char *get_data[DATA_BLOCKS];
	char filePath[100];
    FILE *file[DATA_BLOCKS];
    
    for(i=0; i<DATA_BLOCKS; i++) {
    	printf("filePath %d: ", i+1);
    	scanf("%s", filePath);
    	file[i] = fopen(filePath, "r");
	}
    
//    file[0] = fopen("data1.txt", "r");
//    file[1] = fopen("data2.txt", "r");
//    file[2] = fopen("data3.txt", "r");
//    file[3] = fopen("data4.txt", "r");
    
    for(f=0; f<DATA_BLOCKS; f++) {
    	get_data[f] = (char *)malloc(sizeof(char) * BLOCK_SIZE);
	    while (fgets(get_data[f], sizeof(get_data[f]), file[f]) != NULL)
	    {
	    	printf("file data%d", f+1);
	        printf(": %s\n", get_data[f]);
	    }
	    fclose(file[f]); 
	}	
	
    hash_tree mt = {0, TREE_HEIGHT, MD5_DIGEST_LENGTH, BLOCK_SIZE, DATA_BLOCKS, NULL};
    build_merkle_tree(&mt, get_data);
	print_tree(&mt);
	printf("Final Hash Value:\t");
	MD5Print((unsigned char*)mt.nodes[1].hash);
	freeMerkleTree(&mt);	
    return 0;

}
