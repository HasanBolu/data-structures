#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WordLength 6
#define MAX 10000


struct Vertex {
    char word[WordLength] ;
    int visited;
};

// queue initiliase start
int queueLength = 0;
int queue[MAX];
int rear = -1;
int front = 0;

struct Vertex* vertices[MAX];

int adjacencyMatrix[MAX][MAX];
int vertexCount = 0;

// adds index of vertice into queue
void AddItemToQueue(int index){
    rear = rear + 1;
    queue[rear] = index;
    queueLength = queueLength + 1;
}

// gets index of vertice into queue
int RemoveItemFromQueue(){
    int index = queue[front];
    queueLength = queueLength - 1;
    front = front + 1;
    return index;
}

void InsertVertex(char* word){
    struct Vertex* vertex = (struct Vertex*) malloc(sizeof(struct Vertex));
    vertex->visited = 0;
    int i;
    for (i = 0; i < WordLength; i++) {
        vertex->word[i] = *(word + i);
    }
    vertices[vertexCount] = vertex;
    vertexCount = vertexCount + 1;
}

int isQueueEmpty(){
    if(queueLength == 0){
        return 1;
    }
    else {
        return 0;
    }
}

// gets one of unvisited adjacent vertices of given vertice
int GetUnvisitedAdjecentVertexIndex(int index){
    int i;
    for (i = 0; i < vertexCount; i++) {
        if(adjacencyMatrix[index][i] == 1 && vertices[i]->visited == 0)
        {
            return i;
        }
    }

    return -1;
}

void printVertex(int index){
    printf("%s->", vertices[index]->word);
}


// finds path between two vertices using breadth first search algorithm
void BreadthFirstSearch(int startIndex, int destinationIndex){

    printVertex(startIndex);
    vertices[startIndex]->visited = 1;
    AddItemToQueue(startIndex);

    int unvisitedVertexIndex;
    while(!isQueueEmpty()){
        int vertexIndex =  RemoveItemFromQueue();

        while((unvisitedVertexIndex = GetUnvisitedAdjecentVertexIndex(vertexIndex)) != -1){
            vertices[unvisitedVertexIndex]->visited = 1;
            printVertex(unvisitedVertexIndex);
            AddItemToQueue(unvisitedVertexIndex);
            if(unvisitedVertexIndex == destinationIndex){
                return;
            }
        }
    }

}

// finds that whether given two words are connected or not
int AreNodesConnected(char* word1, char* word2){
    int diffCount = 0;
    int i;
    for (i = 0; i< WordLength; i++) {
        if(*(word1+i) != *(word2+i)){
            diffCount++;
            if(diffCount > 1){
                return 0;
            }
        }
    }

    return 1;
}

// creates edge between two vertices by updating adjacencyMatrix
void SetAdjacencyRow(int vertexIndex){
    int i = 0;
    while(vertices[i] != NULL){
        if(i != vertexIndex){
            if(AreNodesConnected(&vertices[vertexIndex]->word, &vertices[i]->word) == 1){
                adjacencyMatrix[vertexIndex][i] = 1;
            }
        }
        i = i + 1;
    }
}


// finds indexes of given words and finds path between two words using breadth first search
int FindPathBetweenTwoWords(char* word1, char* word2){
    int i = 0;
    int index1 = -1;
    int index2 = -1;
    while(vertices[i] != NULL){
        if(strcmp(vertices[i]->word, word1) == 0){
            index1 = i;
        }
        if(strcmp(vertices[i]->word, word2) == 0){
            index2 = i;
        }
        i = i + 1;
    }

    if(index1 == -1 || index2 == -1){
        return -1;
    }

    BreadthFirstSearch(index1, index2);
    return 1;
}



int main() {
    int i;
    int j;

    // there is no edge at initial state
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }

    // reads words from kelime.txt file line by line
    // creates vertices with these words
    FILE* file = fopen("words.txt", "r");
    int line = 0;
    char* word = (char*) malloc(WordLength * sizeof(char));
    char* wordList = (char*) malloc(MAX * WordLength * sizeof(char));


    while(fgets(word, WordLength, file)){
        if(strcmp(word, "\n") != 0){
            int k;
            for (k = 0; k < WordLength; k++) {
                *(wordList + line*WordLength + k) =  *(word+k);
            }
            InsertVertex(word);
            line = line + 1;
        }

    }

    // sets adjacency matrix using  by checking letter difference between words
    int a=0;
    while(vertices[a] != NULL){
        SetAdjacencyRow(a);
        a = a + 1;
    }

    // prints small part of adjacency matrix

    printf("Test of AreNodesConnected method:\n");
    char* word3 = "write";
    char* word4 = "wrote";

    int isConnected = AreNodesConnected(word3, word4);

    if(isConnected == 1){
        printf("%s and %s words are connected", word3, word4);
    } else {
        printf("%s and %s words are not connected", word3, word4);
    }


    printf("\n\n");

    char* word1 = "among";
    char* word2 = "think";

    printf("\n");
    int result = FindPathBetweenTwoWords(word1, word2);
    if(result != 1){
        printf("\nWord set should contain entered two words");
    }

    return 0;
}