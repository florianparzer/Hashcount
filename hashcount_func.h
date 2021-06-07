/** \File
 * \brief Is a header that provides multible Functions for hashlists and wordlists. Author: Parzer Florian
 *
 */

#ifndef HASHCOUNT_FUNC_H_
#define HASHCOUNT_FUNC_H_

/**
 * \brief A dynamic list of words
 *
 * It is a dynamic list where eac element has a pointer to the next element, an integer variable that is used for counting and the word itself
 *
 */
typedef struct wordlist{
	struct wordlist *next; /**The next element of the lsit*/
	int count; /**The number of times the word was found*/
	char *word; /**The word itself*/
}wordlist;


/**
 * \brief A dynamic list of hash buckets
 *
 * It is a dynamic list where each element has a pointer to the next element, a pointer to a wordlist and the hash value
 *
 */
typedef struct hashlist{
	struct hashlist *next; /**The next bucket of the lsit*/
	wordlist *words; /**The list of words with the same hash value*/
	int hash; /**The hash value of the bucket*/
}hashlist;

#endif /* HASHCOUNT_FUNC_H_ */

void readFile(hashlist **list, FILE *file);

void addWordtoBucket(hashlist **list, int hash, char *word);

void addWord(wordlist **list, char *word);

void freeHashlist(hashlist *list);

int getHash(char *word);

void printHashlist(hashlist *list);

void printHashBucket(hashlist *list, long bucket);

void printBucketsWords(wordlist *buckets, FILE *file, FILE *output);

void printCensorBuckets(wordlist *buckets, FILE *file, FILE *output);

int contains(char *word, wordlist *buchets);

int compareWordlist(wordlist *list1, wordlist *list2);
