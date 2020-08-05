// Implements a dictionary's functionality
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

/* padding used for strings */
#define PADDING_SIZE 16

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

/* function declaration because node is not defined in the header */
void unload_list(node *list);

/* Number of buckets in hash table */
const unsigned int N = 10000;

/* a cached array of all allocated entries */
int allocated_entries[N];
/* counter for number of nodes assigned to an entry in the hash table */
int allocation_count = 0;
/* a counter for number of nodes loaded (including collisons) */
int word_count = 0;
// Hash table
node *table[N] = {NULL};

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hash_of_word = hash(word);
    if (table[hash_of_word] != NULL)
    {
        node *current = table[hash_of_word];
        if (current->next != NULL)
        {
            while(current->next != NULL)
            {
                if (strcasecmp(word, current->word) == 0)
                {
                    return true;
                }
                current = current->next;
            }
        }
        else
        {
            if (strcasecmp(word, current->word) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /* length of given string */
    int length = strlen(word);
    /* length of padding required */
    int padding_length = PADDING_SIZE - (length % PADDING_SIZE);
    int new_length = padding_length + length;
    /* new padded string */
    char padded_string[new_length];
    /* copying the current string to padded string container */
    strcpy(padded_string, word);
    /* adding the padding */
    for (int i = 0; i < padding_length - 1; i++)
    {
        padded_string[i + length] = '0';
    }
    padded_string[new_length - 1] = '\0';

    for (int i = 0; i < new_length; i++)
    {
        padded_string[i] = tolower(padded_string[i]);
    }
    /* some primes for randomness */
    int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    unsigned int result = 0;

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
    /* The idea is to read the dictionary into a hash-table */
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    else
    {
        /* this is pretty self explanatory */
        while (!feof(file))
        {
            /* allocating space to store a new node */
            node *current = malloc(sizeof(node));
            if (current == NULL)
            {
                return false;
            }
            /* this function reads into the current node characters until a \n is encountered */
            read_word(file, current->word);
            /* calculating the hash here */
            int hash_of_current = hash(current->word);
            /* if there is alread an element at the given hash (collision) 
                we just append the node to the front of the linked list */
            if (table[hash_of_current] != NULL)
            {
                /* new node stores the pointer to the original element */
                node *prevoius = table[hash_of_current];
                /* now hash table points to the new element */
                table[hash_of_current] = current;
                /* and the new element points to the older node */
                current->next = prevoius;
                word_count += 1;
            }
            else
            {
                /* now hash table poits to the node */
                table[hash_of_current] = current;
                /* and the new node points to nothing */
                current->next = NULL;
                /* keeping a record of all allocated entries*/
                allocated_entries[allocation_count] = hash_of_current;
                allocation_count += 1;
                word_count += 1;
            }
        }
        fclose(file);
        printf("Allocation Count: %i\nWord Count: %i\n", allocation_count, word_count);
        return true;
    }
    fclose(file);
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count - 1;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < allocation_count; i++)
    {
        if (table[allocated_entries[i]] != NULL)
        {
            unload_list(table[allocated_entries[i]]);
        }
    }
    return true;
}

void read_word(FILE *file, char *buffer)
{
    char next;
    int i = 0;
    do
    {
        /* read the characters until we encounter a newline */
        next = fgetc(file);
        if (next == '\n')
        {
            break;
        }
        buffer[i] = next;
        i += 1;
    }
    while (!feof(file));
    /* dont forget the string terminator */
    buffer[i] = '\0';
}

void unload_list(node *list)
{
    if (list->next == NULL)
    {
        free(list);
    }
    else
    {
        unload_list(list->next);
        free(list);
    }
}