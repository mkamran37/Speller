/**
 * Implements a dictionary's functionality.
 */

#include<stdlib.h>

#include<ctype.h>

#include <stdbool.h>

#include <cs50.h>

#include<string.h>

#include<strings.h>

#include<stdio.h> 

#include "dictionary.h"

#define SIZE 27

typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

int words;

bool loaded = false;

node* hashtable[SIZE] = {NULL};

int generatehashkey(const char *name)
{
        int x = tolower(name[0])- 97;
        if (x < 0 || x > 25)
           x = 26;
        return x;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int l = strlen(word);
    int i;
    char copy[l+1];
    for(i = 0; i < l; i++)
    {
        copy[i] = tolower(word[i]);
        
    }
    copy[i] = '\0';
    
    int hash = generatehashkey(copy);
    
    node *entry = hashtable[hash];
    
    if(entry)
    {
        node *cursor = entry;
        while (cursor->next != NULL)
            {
                if (strcmp(copy, cursor->word) == 0)
                    return true;
                cursor = cursor->next;
            }

        if (strcmp(copy, cursor->word) == 0)
    		return true;
		cursor = cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    for (int i = 0; i < SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    
    // open the input dictionary file
    FILE* fp = fopen(dictionary, "r");
    
    if (fp == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }

    while (true)
    {
        // malloc a node for each new word
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Could not malloc a new node.\n");
            return false;
        }
        
        // read a word from the dictionary and store it in new_node->word
        fscanf(fp, "%s", new_node->word);
        new_node->next = NULL;
        
        if (feof(fp))
        {
            // hit end of file
            free(new_node);
            break;
        }

        words++;
        
        // hashtable[h] is a pointer to a key-value pair
        int h = generatehashkey(new_node->word);
        node* head = hashtable[h];
        
        // if bucket is empty, insert the first node
        if (head == NULL)
        {
            hashtable[h] = new_node;
        }
 
        else
        {
            new_node->next = hashtable[h];
            hashtable[h] = new_node;
        }
    }
    // close dictionary
    fclose(fp);
    loaded = true;
    return true;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loaded)
        return words;
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
 // for each node in the hashtable
    for (int i = 0; i < SIZE; i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    loaded = false;
    return true;
}
