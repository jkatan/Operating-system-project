
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "memoryManager.h"

unsigned char memory[4194304];

void initializeMemory()
{
    memset(memory, 0, 4194304);
    memset(memory, 42, 1);
}

void* alloc(size_t bytes)
{
    if(bytes > 4194303)
    {
        return NULL;
    }

    int position = 0;
    int orderOfBlockRequired = calculateBlockOrder(bytes);

    unsigned char c = 0;
    metadata * currentBlockMetadata = memset(&c, 0, 1);

    while(position < 4194304)
    {
        readMetadata(position, currentBlockMetadata);

        if(orderOfBlockRequired <= currentBlockMetadata->order)
        {
            if(currentBlockMetadata->isAllocated)
            {
                position = jumpNextPosition(position, currentBlockMetadata->order);
            }
            else if(orderOfBlockRequired == currentBlockMetadata->order)
            {
                memory[position]++;
                position++;
                printf("Position: %d\n", position);
                return (void*) (&memory + position);
            }
            else
            {
                int buddy = jumpNextPosition(position, currentBlockMetadata->order - 1);

                char metadataLeft = 2;
                char metadataRight = 0;

                metadataLeft += ((currentBlockMetadata->order - 1) * 4);
                metadataRight += ((currentBlockMetadata->order - 1) * 4);

                memory[position] = metadataLeft;
                memory[buddy] = metadataRight;
            }
        }

        else
        {
            position = jumpNextPosition(position, orderOfBlockRequired);
        }

    }

    return NULL;

}

void dealloc(void* address)
{
    if(address <= (void*)&memory || (void*)(address > &memory + 4194304))
    {
        return;
    }

    int position = address - (void*)&memory - 1;

    if(position % 4096 != 0 || memory[position] % 2 == 0)
    {
        return;
    }

    unsigned char c1 = 0;
    unsigned char c2 = 0;
    metadata * metadata1 = (&c1, 0, 1);
    metadata * metadata2 = (&c2, 0, 1);

    memory[position] -= 1;

    while(position >= 0 && position < 4194304)
    {
        readMetadata(position, metadata1);
        if(metadata1->isLeft)
        {
            int positionBuddy = jumpNextPosition(position, metadata1->order);

            if(positionBuddy < 0 || positionBuddy >= 4194304)
            {
                return;
            }

            readMetadata(positionBuddy, metadata2);

            if(metadata2->isAllocated || metadata1->order != metadata2->order)
            {
                return;
            }

            mergeBlocks(position, positionBuddy, metadata1->order);

        }
        else
        {
            int positionBuddy = jumpPreviousPosition(position, metadata1->order);

            if(positionBuddy < 0 || positionBuddy >= 4194304)
            {
                return;
            }

            readMetadata(positionBuddy, metadata2);

            if(metadata2->isAllocated || metadata1->order != metadata2->order)
            {
                return;
            }

            mergeBlocks(positionBuddy, position, metadata1->order);
        }
    }

}

void mergeBlocks(int leftPosition, int rightPosition, int order)
{
    char newMetadata = 4 * (order + 1);

    if(isLeft(position, order))
    {
        newMetadata += 2;
    }

    memory[rightPosition] = 0;

    memory[leftPosition] = newMetadata;
}

int isLeft(int position, int order)
{
    return (position % pow(2, 13 + order) == 0) ? 1 : 0;
}

int jumpNextPosition(int position, int order)
{
    position += (int)(pow(2, order + 12));
    return position;
}

int jumpPreviousPosition(int position, int order)
{
    position -= (int)(pow(2, order + 12));
    return position;
}

void readMetadata(int position, metadata* blockMetadata)
{
    memset(blockMetadata, memory[position], 1);
}

int calculateBlockOrder(size_t bytes)
{
    if(bytes < 4096)
    {
        return 0;
    }

    return floor(logBase2(bytes) - logBase2(4096)) + 1;
}

double logBase2(size_t number)
{
    return log(number)/log(2);
}