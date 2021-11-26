// Implements a dictionary's functionality

#include <stdbool.h>
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// pour eviter les collisions, on multiplie par la valeur ASCII max des lettres minuscules (cele de z)
// N doit etre de preference une nb premier ou une puissancde de 2
const unsigned int N = (LENGTH + 1) * ('z');


// Hash table
node *table[N];

//numer of word in dictionary
int nb_ofword = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //hash value of the check word
    unsigned int index = hash(word);

    //the cursor pointer that scrolls through the chained list
    node *cursor = table[index];

    // list corresponding to this hash value and search of this word
    //loop until reaching the end of the list
    while (cursor != NULL)
    {
        //use strcasecmp to see if word from dictionary loaded at the current node equals word (string array) from text file (per speller.c)
        //note strcmp equals 0 if it is a match
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        //if strings not equal, move crawler to the next node and then repeat loop (check for word) at that node
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hashvalue;
    for (hashvalue = 0; *word != '\0' ; word++)
    {
        hashvalue += tolower(*word);
        hashvalue = tolower(*word) + 31 * hashvalue;
    }
    return hashvalue % N;
}

// Loads dictionary into memory, returning true if successful else false

bool load(const char *dictionary)
{
    //Opening of the file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    string buffer[LENGTH + 1];

    //lPlaying strings from the file
    while (fscanf(file, "%s", *buffer) != EOF)
    {
        /*unsigned int index = hash(*buffer);*/

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        n->next = NULL;

        strcpy(n->word, *buffer);

        int index = hash(n->word);

        if (table[index] == NULL) // creation of chained list heads
        {
            table[index] = n;
        }

        else
        {
            n->next = table[index];
            table[index] = n;
        }
        nb_ofword ++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return nb_ofword;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *head;
    node *cursor;
    node *tmp;
    for (int i = 0; i < N; i++)
    {
        head = table[i];
        cursor = head;
        tmp = head;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}