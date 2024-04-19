#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//COP3502C


#define ALPHABET_SIZE 26 // Number of English lowercase letters

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count; // To store the number of times a word ends at this node
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Creates a new trie node initialized to NULL
struct TrieNode *getNewTrieNode() {
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (pNode) {
        pNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pNode->children[i] = NULL;
        }
    }
    return pNode;
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) {
        pTrie->root = getNewTrieNode();
    }
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *crawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!crawl->children[index]) {
            crawl->children[index] = getNewTrieNode();
        }
        crawl = crawl->children[index];
        word++;
    }
    crawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct TrieNode *crawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!crawl->children[index]) {
            return 0;
        }
        crawl = crawl->children[index];
        word++;
    }
    return crawl->count;
}

// Deallocate the trie structure
void deallocateTrieNode(struct TrieNode *pNode) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pNode->children[i]) {
            deallocateTrieNode(pNode->children[i]);
        }
    }
    free(pNode);
}

struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie) {
        deallocateTrieNode(pTrie->root);
    }
    free(pTrie);
    return NULL;
}

// This function will read the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    int numWords;
    fscanf(file, "%d", &numWords);
    for (int i = 0; i < numWords; ++i) {
        pInWords[i] = malloc(100); // assuming max word length is 99
        fscanf(file, "%s", pInWords[i]);
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}