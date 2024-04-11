#include <stdio.h>
#include <stdlib.h>
// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Node structure for linked list in each hash table slot
struct ListNode
{
	struct RecordType record; // Contains the data (id, name, order)
	struct ListNode* next;    // Pointer to the next node in the list
};

// Hash table structure
struct HashType
{
	struct ListNode** table; // Array of pointers to ListNode
	int size;                // Size of the hash table
};
// Compute the hash function
int hash(int x,int tablesize)
{
	return x % 23;
}


// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
	int index = hash(record.id , hashTable->size);
	struct ListNode* newNode = (struct ListNode*) malloc(sizeof(struct ListNode));
	newNode->record = record;
	newNode->next = hashTable->table[index]; // Insert at the beginning
	hashTable->table[index] = newNode;
}
void initHashTable(struct HashType* hashTable, int size)
{
	int i;
	hashTable->size = size;
	hashTable->table = (struct ListNode**) malloc(sizeof(struct ListNode*) * size);
	for (i = 0; i < size; ++i)
	{
		hashTable->table[i] = NULL; // Initialize all slots as empty
	}
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray)
{
	int i;
	struct ListNode* current;
	for (i = 0; i < pHashArray->size; ++i)
	{
		current = pHashArray->table[i];
		if (current != NULL)
		{
			printf("index %d -> ", i);
			while (current != NULL)
			{
				printf("%d, %c, %d -> ", current->record.id, current->record.name, current->record.order);
				current = current->next;
			}
			printf("NULL\n");
		}
	}
}


#include <stdio.h>
#include <stdlib.h>

// Assume the structures RecordType, ListNode, and HashType
// and the functions hash, initHashTable, insertRecord, displayRecordsInHash are defined as previously discussed.

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    struct HashType hashTable;
    int i, hashTableSize = 23; 

    // Parse data from file (function implementation assumed to be provided)
    recordSz = parseData("input_lab_9.txt", &pRecords);

    // Initialize hash table
    initHashTable(&hashTable, hashTableSize);

    // Insert records into hash table
    for(i = 0; i < recordSz; ++i) {
        insertRecord(&hashTable, pRecords[i]);
    }

    // Display records in hash table
    displayRecordsInHash(&hashTable);

 
    for(i = 0; i < hashTableSize; ++i) {
        struct ListNode* current = hashTable.table[i];
        while(current != NULL) {
            struct ListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }

    // Free the hash table array and parsed records
    free(hashTable.table);
    free(pRecords);

    return 0;
}
