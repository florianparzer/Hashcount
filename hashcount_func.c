/** \File
 * \brief Provides multible Functions for hashlists and wordlists. Author: Parzer Florian
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashcount_func.h"



/**
 * \brief Adds a word to a hashlist
 *
 * Adds a word to the bucket of the hashlist with the hash of the parameter hash and adding the word to the wordlist in the bucket.
 * If a element with the same hash does not exist, it will be created.
 *
 * \param list	The hashlist to which the word is added.
 * \param hash	The hashvalue of the bucket to which the word will be added
 * \param word	The word the hash is calculated from
 *
 */
void addWordToBucket(hashlist **list, int hash, char *word){
	if (*list == NULL){
		//Is executed when list has not been initialized
		//Changes Pointer to list
		hashlist *newElement = malloc(sizeof(hashlist));
		if(newElement == NULL){
			printf("ERROR\n");
			return;
		}
		wordlist *newWords = NULL;
		newElement->hash = hash;
		addWord(&newWords, word);
		newElement->words=newWords;

		newElement->next=NULL;
		*list = newElement;
		return;
	}
	hashlist *element = *list;
	hashlist *previous = NULL;
	while (element != NULL){
		if(element->hash == hash){
			//Is executed when element is the right hash bucket
			addWord(&element->words, word);
			break;
		}else if (element->hash > hash){
			//Is executed when the element has already a higher hash
			//Changes Pointer to list
			hashlist *newElement = malloc(sizeof(hashlist));
			if(newElement == NULL){
				printf("ERROR\n");
				return;
			}
			wordlist *newWords = NULL;
			newElement->hash = hash;
			addWord(&newWords, word);
			newElement->words=newWords;

			newElement->next=element;
			if(previous == NULL){
				*list = newElement;
			}else{
				previous->next=newElement;
			}
			break;
		}else if(element->next==NULL){
			//Is executed when there has not been an element found with the same hash
			hashlist *newElement = malloc(sizeof(hashlist));
			if(newElement == NULL){
				printf("ERROR\n");
				return;
			}
			wordlist *newWords = NULL;
			newElement->hash = hash;
			addWord(&newWords, word);
			newElement->words=newWords;

			newElement->next=NULL;
			element->next=newElement;
			break;
		}
		previous = element;
		element = element->next;
	}
}

/**
 * \brief Reads the File and fills the hashlist with the words of the file
 *
 * The method reads the file. It splits the stream at the charackters 'newline', 'space', '.', ':', ',', ';', '?' and 'space'. Afterwards it stores
 * the words in the hashlist
 *
 * \param list	The hashlist to which the word is added.
 * \param file	The File Stream of the Input file
 *
 */
void readFile(hashlist **list, FILE *file){
	char *word;
	int beginning = 0;
	int ending = 0;
	int length = 0;
	int ch;

	while(1){
			ch = fgetc(file);
			if(ch == '\n'|| ch == ' ' || ch == '.' || ch == ';'	|| ch == ':' || ch == ',' || ch == '?' || ch == '\t' || ch == EOF){
				ending = ftell(file);
				if(ch == EOF){
					ending++;
				}
				length = ending - beginning;
				fseek(file, beginning, SEEK_SET);
				word = malloc(sizeof(char)*length);
				fgets(word, length, file);
				if(strcmp(word, "\0") != 0){
					addWordToBucket(list, getHash(word), word);
				}else{
					free(word);
				}
				fseek(file, 1, SEEK_CUR);
				beginning = ending;
			}
			if(ch == EOF){
				break;
			}
		}
}

/**
 * \brief Compares the lower case of two words.
 *
 * Compares the lower case two two word with strcmp
 *
 * \param word  The first word
 * \param other	The second word
 *
 * \return 0 if equals, a negative value when word is alphabetically lower and a positive value when word is alphabetically higher
 */
int stringCompareLower(char *word, char *other){
	char *cpy =malloc(strlen(word) * sizeof(char));
	char *cpyOther =malloc(strlen(word) * sizeof(char));
	strncpy(cpy, word, strlen(word));
	strncpy(cpyOther, other, strlen(other));
	for(int i = 0; i < (int) strlen(cpy); i++){
		cpy[i] = tolower(cpy[i]);
	}

	for(int i = 0; i < (int) strlen(cpyOther); i++){
			cpyOther[i] = tolower(cpyOther[i]);
	}
	int cmp = strcmp(cpy, cpyOther);
	free(cpy);
	free(cpyOther);
	return cmp;
}
/**
 * \brief Adds a word to the wordlist.
 *
 * Adds the word to the wordlist. If the word is already contained a counter is increased.
 *
 * \param list  Is the wordlist in which the word is added. If it is NULL, the list will be created.
 * \param word	The word the hash is calculated from.
 *
 */
void addWord(wordlist **list, char *word){
	wordlist *newElement = malloc(sizeof(wordlist));
	if(newElement == NULL){
		printf("ERROR\n");
		return;
	}
	newElement->word = word;
	newElement->count = 1;
	newElement->next = NULL;
	if(*list == NULL){
		*list=newElement;
		return;
	}
	wordlist *element = *list;
	wordlist *prev = NULL;
	while(element != NULL){

		if(strcmp(element->word, word) == 0){
			element->count++;
			break;
		}
		if(stringCompareLower(word, element->word) <0){
			newElement->next = element;
			if(prev == NULL){
				*list = newElement;
			}else{
				prev->next = newElement;
			}
			break;
		}

		if(element->next == NULL){
			element->next=newElement;
			break;
		}
		prev = element;
		element = element->next;
	}
	return;
}



/**
 * \brief Frees the with malloc allocated memory of a hashlist
 *
 * It iterates over every element of the hashlist and  frees every element that reserved its memory with the melloc method
 *
 * \param list	The hashlist that schould be freed
 *
 *
 */
void freeHashlist(hashlist *list){
	hashlist *element = list;
	hashlist *nextElement = NULL;
	wordlist *words = NULL;
	wordlist *nextWords = NULL;
	while(element!=NULL){
		nextElement = element->next;
		words = element->words;
		while(words != NULL){
			nextWords=words->next;
			free(words);
			words = nextWords;
		}
		free(element);
		element = nextElement;
	}
}



/**
 * \brief Calculates the hashvalue of a word.
 *
 * Calculates the bash of the word by summing the values of the characters and calculate the result withc modulo
 *
 * \param word	The word the hash is calculated from
 *
 * \return returns the calculated hashvalue as Integer
 *
 */
int getHash(char *word){
	int hash = 0;
	int i = 1;
	while(*word != '\0'){
		hash+=i*(*word);
		word++;
		i++;
	}
	hash%=100;
	return hash;
}

/**
 * \brief Prints the hashlist in a readable way
 *
 * Prints every bucket of the Hashlist in a seperate line. If the words per line exeed 10 a new line is started
 *
 * \param list	The hashlist that will be printed.
 *
 */
void printHashlist(hashlist *list){
	hashlist *element = list;
	wordlist *word = NULL;
	int i = 0;
	int isFirst = 1;
	while(element != NULL){
		printf("\nbucket[%d]: ", element->hash);
		word = element->words;
		while(word != NULL){

			if(isFirst){
				printf("%s", word->word);
				if(word->count > 1){
					printf("(%d)", word->count);
				}
				isFirst = 0;
			}else if(i == 9){
				printf(", %s", word->word);
				if(word->count > 1){
					printf("(%d)", word->count);
				}
				printf("\n");
			}else if(i == 0){
				printf("%s", word->word);
				if(word->count > 1){
						printf("(%d)", word->count);
				}
			}else{
				printf(", %s", word->word);
				if(word->count > 1){
					printf("(%d)", word->count);
				}
			}
			word = word->next;
			i++;
			i%=10;
		}
		i = 0;
		isFirst = 1;
		element = element->next;
	}
	printf("\n");
}

/**
 * \brief Prints on bucket of the hashlist
 *
 * Prints the stated bucket. If the words per line exeed 10 a new line is started. If the Bucket is not found it will print an error Message.
 *
 * \param list	The hashlist in which the bucket will be searched in.
 * \param bucket	The hashvalue of the bucket that should be printed.
 */
void printHashBucket(hashlist *list, long bucket){
	hashlist *element = list;
	wordlist *word = NULL;
	int i = 0;
	int isFirst = 1;
	while(element != NULL){
		if(element->hash != bucket){
			i = 0;
			isFirst = 1;
			element = element->next;
			continue;
		}
		printf("\nbucket[%d]: ", element->hash);
		word = element->words;
		while(word != NULL){
			if(isFirst){
				printf("%s", word->word);
				if(word->count > 1){
					printf("(%d)", word->count);
				}
				isFirst = 0;
			}else if(i == 9){
				printf(", %s", word->word);
				if(word->count > 1){
					printf("(%d)", word->count);
				}
				printf("\n");
			}else if(i == 0){
				printf("%s", word->word);
				if(word->count > 1){
						printf("(%d)", word->count);
				}
			}else{
				printf(", %s", word->word);
				if(word->count > 1){
					printf("(%d)", word->count);
				}
			}
			word = word->next;
			i++;
			i%=10;
		}
		printf("\n");
		return;
	}
	fprintf(stderr, "Bucket does not exist\n");
}

/**
 * \brief Only the words who have the same hash value as the stated buckets
 *
 * It prints only the words that's hash maches one of the buckets. Every other character of the input file will not be printed.
 *
 * \param buckets	The hash values of the stated buckets
 * \param file		The input file stream
 * \param output	The output file stream
 */
void printBucketsWords(wordlist *buckets, FILE *file, FILE *output){
	char *word;
	int beginning = 0;
	int ending = 0;
	int length = 0;
	int ch;
	int isContained = 1;
	fseek(file, beginning, SEEK_SET);
	while(1){
		ch = fgetc(file);
		if(ch == '\n'|| ch == ' ' || ch == '.' || ch == ';'	|| ch == ':' || ch == ',' || ch == '?' || ch == '\t' || ch == EOF){
			ending = ftell(file);
			if(ch == EOF){
				ending++;
			}
			length = ending - beginning;
			fseek(file, beginning, SEEK_SET);
			word = malloc(sizeof(char)*length);
			fgets(word, length, file);
			if(strcmp(word, "\0") != 0){
				isContained = contains(word, buckets);
				if(isContained == 1){
					fprintf(output, "%s\n", word);
				}
			}
			free(word);
			fseek(file, 1, SEEK_CUR);
			beginning = ending;
		}
		if(ch == EOF){
			break;
		}
	}
}

/**
 * \brief Prints the text of the input file in the output file and censors the words in the stated buckets.
 *
 * It prints every character of the input file in the output file. If the word is contained in the stated buckets it will be censored.
 *
 * \param buckets	The hash values of the stated buckets
 * \param file		The input file stream
 * \param output	The output file stream
 */
void printCensorBuckets(wordlist *buckets, FILE *file, FILE *output){
	char *word;
	int beginning = 0;
	int ending = 0;
	int length = 0;
	int ch;
	int isContained = 1;
	fseek(file, beginning, SEEK_SET);
	while(1){
		ch = fgetc(file);
		if(ch == '\n'|| ch == ' ' || ch == '.' || ch == ';'	|| ch == ':' || ch == ',' || ch == '?' || ch == '\t' || ch == EOF){
			ending = ftell(file);
			if(ch == EOF){
				ending++;
			}
			length = ending - beginning;
			fseek(file, beginning, SEEK_SET);
			word = malloc(sizeof(char)*length);
			fgets(word, length, file);
			if(strcmp(word, "\0") != 0){
				isContained = contains(word, buckets);
				if(isContained == 0){
					fprintf(output, "%s", word);
				}else{
					for(int i = 0; i<(length-1); i++){
						fprintf(output, "-");
					}
				}
			}
			free(word);

			fseek(file, 1, SEEK_CUR);
			beginning = ending;
			if(ch == EOF){
				break;
			}
			fprintf(output, "%c", ch);
		}
	}
}

/**
 * \brief Checks if the word has the same hash value as one of the buckets
 *
 * Checks if the word has the same hash value as one of the buckets by comparing it to every element of the wordlist.
 *
 * \param word		Is the word that is checked
 * \param buckets	Are the hash values that are compaired against
 * \return 1 if the word has the same hashv alue as one of the buckets, otherwise it will return 0
 */
int contains(char *word, wordlist *buckets){
	wordlist *element =  buckets;
	int hash = getHash(word);
	while(element != NULL){
		char *error;
		long bucketHash = strtol(element->word, &error, 10);
		if(element->word == error){
			fprintf(stderr, "Please enter a valid number");
		}
		if(bucketHash == hash){
			return 1;
		}
		element = element->next;
	}
	return 0;
}
