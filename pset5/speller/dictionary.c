// Implements a dictionary's functionality
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "dictionary.h"

#define PADDING_SIZE 16
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /* length of given string */
    int length = strlen(word);
    /* length of padding required */
    int padding_length = PADDING_SIZE - (length % PADDING_SIZE);

    /* new padded string */
    char padded_string[padding_length + length];
    /* copying the current string to padded string container */
    strcpy(padded_string, word);
    /* adding the padding */
    for (int i = 0; i < padding_length; i++)
    {
        padded_string[i + length] = '0' + (padding_length - i);
    }

    /* some primes for randomness */
    int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    unsigned int result = 0;
    int new_length = padding_length + length;

    /* calculations of the hash */
    for (int i = 0; i < new_length; i++)
    {
        /* the integer representing the char */
        int int_of_char = (int) padded_string[i];
        /* randomly (pseudo) selecting a prime number for the given character */
        int index_of_primes = ((i * length) + (int) floor(pow(int_of_char, 0.5))) % 10;


        double current = (double) int_of_char * pow(primes[index_of_primes], 0.5);
        current = pow((pow(result, 2) + pow(current, 2)), 0.5);
        result = result + (int) current;
        /* N is the maximum number of buckets in our hashtable */
        result = result % N;
    }
    return result;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
