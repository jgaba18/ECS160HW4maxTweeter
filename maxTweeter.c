//
// Created by Jatin Gaba and Aashay Vadnagara on 3/8/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_CHARS      20000
#define INVALID_ERROR       "Invalid Input Format\n"
static int totalColumns = 0;

typedef struct node {
    char* name;
    int tweetCount;
    struct node * next;
} Node;

typedef struct list {
    Node * head;
} List;

Node * createnode(char* string){
    Node * newNode = malloc(sizeof(Node));
    newNode->name = string;
    newNode->tweetCount = 1;
    newNode->next = NULL;
    printf("%s, %d\n", newNode->name, newNode->tweetCount);
    return newNode;
}

List * emptylist(){
    List * list = malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void display(List * list) {
    Node * current = list->head;
    if(list->head == NULL)
        return;
    while(current->next != NULL){
        printf("%s, %d\n", current->name, current->tweetCount);
        current = current->next;
    }
    printf("%s, %d\n", current->name, current->tweetCount);
}

Node * find(char* name, List* list) {

    //start from the first link
    struct node* current = list->head;

    //if list is empty
    if(current == NULL) {
        return NULL;
    }

    //navigate through list
    while(strcmp(current->name, name)) {

        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //go to next link
            current = current->next;
        }
    }
    return current;
}

void add(char* name, List * list){
    Node * current = NULL;
    if(list->head == NULL){
        list->head = createnode(name);
    }
    else {
        current = find(name, list);
        if(current != NULL){
            current->tweetCount += 1;
            printf("%s, %d\n", current->name, current->tweetCount);
        }
        else{
            current = list->head;
            while (current->next!= NULL){
                current = current->next;
            }
            current->next = createnode(name);
        }
    }
}

///*********************************************************************************************/

int getParameterIndex(const char *buff, char* parameter) {
    char* header = strtok(buff, ",");
    int nameIndex = -1;
    int currIndex = 0;
    char paramBuff[40];
    sprintf(paramBuff, "\"%s\"", parameter);

    while(header != NULL){
        if(strcmp(header, paramBuff) == 0){
            nameIndex = currIndex;
        }
        currIndex++;
        header = strtok(NULL, ",");
    }
    totalColumns = currIndex;

    return nameIndex;
}

char* getTweeters(char* buff, int paramIndex){
    char* header = strtok(buff, ",");
    char* name = NULL;
    int counter = 0;

    while(header != NULL){
        if(counter == paramIndex){
            name = header;
        }
        counter++;
        header = strtok(NULL, ",");
    }

    if(counter != totalColumns){
        printf(INVALID_ERROR);
        return NULL;
    }
    return name;
}

int main(int argc, char** argv){

    // Get the path to the file through commandline
    if (argc < 1){
        printf("Error: Invalid Arguments.\n");
        return -1;
    }

    const char* path = argv[1];
    FILE* file;
    file = fopen(path, "r");
    if (file == NULL){
        perror("Error");
        return -1;
    }

    char buff[MAX_LINE_CHARS];
    if(fgets(buff, MAX_LINE_CHARS, file) == NULL){
        printf(INVALID_ERROR);
        return -1;
    }

    int paramIndex = getParameterIndex(buff, "name");
    if (paramIndex == -1){
        printf(INVALID_ERROR);
        return -1;
    }

    int lineCount = 2;
    List *Tweeterlist = emptylist();

    while(lineCount <= 20000){

        fgets(buff, MAX_LINE_CHARS, file);

        lineCount++;

        if(feof(file)){
            break;
        }

        // function to add/update name and to check
        // if csv is correctly formatted with commas
        char* tweeterName = getTweeters(buff, paramIndex);
        if(tweeterName == NULL){
            printf(INVALID_ERROR);
            return -1;
        }
        else{
            //insertFirst(tweeterList, tweeterName);
            add(tweeterName, Tweeterlist);
        }
    }

    if (lineCount > 20000) {
        printf(INVALID_ERROR);
        return -1;
    }
    fclose(file);
    return 0;
}