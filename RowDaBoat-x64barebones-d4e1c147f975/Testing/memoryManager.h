#include <stddef.h>

typedef struct metadata
{
    unsigned char isAllocated : 1;
    unsigned char isLeft : 1;
    unsigned char order : 6;

}metadata;

void* alloc(size_t bytes);
int calculateBlockOrder(size_t bytes);
double logBase2(size_t number);
int jumpNextPosition(int position, int order);
void readMetadata(int position, metadata* blockMetadata);
void initializeMemory();