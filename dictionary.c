// Implements a dictionary's functionality

#include <stdbool.h>
#include <stddef.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 37;

// Hash table
node *table[N];

// Hash table size
unsigned int table_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);
    node *head = table[index];

    for (node *cursor = head; cursor != NULL; cursor = (*cursor).next)
    {
        if (strcasecmp((*cursor).word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        if (isupper(word[i]))
        {
            sum += tolower(word[i]);
        }
        else
        {
            sum += word[i];
        }
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    for (int c = fscanf(dictionary_file, "%s", word); c != EOF; c = fscanf(dictionary_file, "%s", word))
    {
        node *new_node_ptr = malloc(sizeof(node));
        if (new_node_ptr == NULL)
        {
            fclose(dictionary_file);
            return false;
        }
        strcpy((*new_node_ptr).word, word);

        int index = hash(word);

        (*new_node_ptr).next = table[index];
        table[index] = new_node_ptr;

        table_size += 1;
    }

    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return table_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        node *cursor = head;

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = (*cursor).next;
            free(tmp);
        }
    }
    return true;
}
