#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Set the max word length
#define MAX_WORD_LENGTH 100

// Trie structure
struct Trie{	
	struct Trie* children[26];
    int wordCount;
};


void insert(struct Trie *pTrie, char *word);
int numberOfOccurances(struct Trie *pTrie, char *word);
struct Trie *deallocateTrie(struct Trie *pTrie);
struct Trie *createTrie();
int readDictionary(char *filename, char **pInWords);


int main(void) {
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word){
	if(!pTrie || !word){
		printf("invalid node or word\n");
		return;
	}

	struct Trie* curr = pTrie;
	for(int i = 0; i < strlen(word); i++){

		int id = word[i] - 'a';
		if(id < 0 || id >= 26){
			printf("invalid charas in word %s\n", word[i]);
			return;
		}

		if(!(curr -> children[id])){
			curr -> children[id] = createTrie();
		}
		curr = curr -> children[id];
	}
	curr -> wordCount++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word){
	struct Trie* curr = pTrie;
	for(int i = 0; i < strlen(word); i++){
		int id = word[i] - 'a';
		if(!(curr -> children[id]))
			return 0;
		curr = curr -> children[id];
	}
	return curr -> wordCount;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie){
	if(!pTrie)
		return NULL;
	for(int i = 0; i < 26; i++){
		deallocateTrie(pTrie -> children[i]);
	}
	free(pTrie);
	return NULL;
}

// Initializes a trie structure
struct Trie *createTrie(){
	struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
	if(newTrie){
		newTrie -> wordCount = 0;
		for(int i = 0; i < 26; i++)
			newTrie -> children[i] = NULL;
	}
	return newTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords){
	FILE* file = fopen(filename, "r");
	if(!file){
		printf("Error opening file: %s\n", filename);
		return 0;
	}

	int numWords;
	fscanf(file, "%d", &numWords);
	fgetc(file);

	int wordCount = 0;
	char word[MAX_WORD_LENGTH];

	while(fscanf(file, "%s", word) != EOF && wordCount < 256){
		pInWords[wordCount] = (char*)malloc(strlen(word) + 1);
		strcpy(pInWords[wordCount], word);
		wordCount++;
	}

	fclose(file);
	return wordCount;
}