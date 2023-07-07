/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  void *s can be safely cast
 * to a char * (null terminated string)
 */
unsigned int stringHash(void *s) {
  // -- TODO --
  //fprintf(stderr, "need to implement stringHash\n");
    unsigned int hash = 0;
    for(int i = 0; i < strlen((char*)s); i++) {
        hash += (char)((char*)s)[i];
    }
  /* To suppress compiler warning until you implement this function, */
  return hash;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  //fprintf(stderr, "You need to implement stringEquals");
    
  /* To suppress compiler warning until you implement this function */
  return strcmp((char*)s1, (char*)s2);
}


int get_word(FILE *pf, char **buf) {
    char c;
    int length = 0;
    int capacity = 5;
    char *tmp = (char*)malloc(capacity * sizeof(char));
    if(tmp == NULL)
        exit(1);

    while((c = fgetc(pf)) != EOF) {
        //if(c == ' ' && c == '\n') {
        if(isspace(c)) {
            break;
        } else {
            //fprintf(stderr, "read c = %c\n", c);
            if (length == capacity) {
                capacity += 5;
                tmp = realloc(tmp, capacity);
                if(tmp == NULL)
                    exit(1);
            }
            tmp[length] = c;
            length++;
        }
    }

    if(length == 0) {
        *buf = NULL;
    } else {
        tmp = realloc(tmp, length + 1);
        if(tmp == NULL)
            exit(1);
        tmp[length] = '\0';
        //length++;
        *buf = tmp;
    }
    return length;
}

/*
 * This function should read in every word and replacement from the dictionary
 * and store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final bit of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(61)
 * to cleanly exit the program.
 */
void readDictionary(char *dictName) {
  // -- TODO --
  //fprintf(stderr, "You need to implement readDictionary\n");
    // 1 open file
    FILE *pf = fopen(dictName, "r");
    if (pf == NULL)
        exit(61);
    char *key, *value;
    int len;
    while(1) {
        //fprintf(stderr, "--------key-------\n");
        len = get_word(pf, &key);
        //fprintf(stderr, "key len = %d, strlen = %ld\n", len, strlen(key));
        if(len == 0)
            break;
        //fprintf(stderr, "--------word-------\n");
        len = get_word(pf, &value);
        //fprintf(stderr, "word len = %d, strlen = %ld\n", len, strlen(value));
        if(len == 0)
            exit(1);
        insertData(dictionary, (void*)key, (void*)value);
    }
    //fprintf(stderr, "Dictionary loaded 22\n");
}


/*
 * This should process standard input (stdin) and perform replacements as 
 * described by the replacement set then print either the original text or 
 * the replacement to standard output (stdout) as specified in the spec (e.g., 
 * if a replacement set of `taest test\n` was used and the string "this is 
 * a taest of  this-proGram" was given to stdin, the output to stdout should be 
 * "this is a test of  this-proGram").  All words should be checked
 * against the replacement set as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the replacement set should 
 * it report the original word.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final bit of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  // -- TODO --
    //fprintf(stderr, "You need to implement processInput\n");
    char c;
    char *str = NULL;
    char *replace = NULL;
    char *tmpReplace = NULL;
    int length = 0;
    int capacity = 0;
    int keylen = 0;
    char *tmpStr = NULL;
    int i = 0;
    while((c = fgetc(stdin)) != EOF) {
        if (str == NULL) {
            capacity = 5;
            str = malloc(sizeof(char) * 5);
        }
        // put c and '\0' to str
        if (length + 1 >= capacity) {
            capacity += 5;
            str = realloc(str, capacity);
            if(str == NULL)
                exit(1);
        }
        // put c
        str[length] = c;
        length++;
        // put '\0'
        str[length + 1] = '\0';

        // end of word (space or '\n'), now str is like "abc \0" or "abc\n\0"
        // now print str and its replace
        if(isspace(c)) {
            // if replace not equal null
            if(replace != NULL) {
                // print replace
                fprintf(stdout, "%s", replace);
                // print left of str
                fprintf(stdout, "%s", str + keylen);
            } else
                // print str
                fprintf(stdout, "%s", str);
            // free str
            if(str != NULL) {
                free(str);
                str = NULL;
            }
            // reset other var
            length = 0;
            replace = NULL;
            tmpReplace = NULL;
            keylen = 0;
            continue;
        }
        // find replace with current str
        tmpReplace = (char*)findData(dictionary, (void*)str);
        if(tmpReplace != NULL) {
            replace = tmpReplace;
            // save the keylen for print
            keylen = strlen(str);
            continue;
        }
        // find replace with all but the first letter converted to lowercase
        tmpStr = malloc(sizeof(char) * strlen(str) + 1);
        memcpy(tmpStr, str, strlen(str) + 1);
        for(i = 1; i < strlen(tmpStr); i++) {
            if(!isspace(tmpStr[i]) && tmpStr[i] <= 90 && tmpStr[i] >= 65)
                tmpStr[i] += 32;
        }
        tmpReplace = (char*)findData(dictionary, (void*)tmpStr);
        if(tmpReplace != NULL) {
            replace = tmpReplace;
            keylen = strlen(str);
            free(tmpStr);
            tmpStr = NULL;
            continue;
        }
        
        // find replace with all letter converted to lowercase
        if(!isspace(tmpStr[0]) && tmpStr[0] <= 90 && tmpStr[0] >= 65)
            tmpStr[0] += 32;
        tmpReplace = (char*)findData(dictionary, (void*)tmpStr);
        if(tmpReplace != NULL) {
            replace = tmpReplace;
            keylen = strlen(str);
            free(tmpStr);
            tmpStr = NULL;
            continue;
        }

    }
}
