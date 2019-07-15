#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MaxFriendsCount 100
#define MaxNameLength 100

struct NODE {
    int userIdentity;
    char username[MaxNameLength];
    int userFriendsIdentities[MaxFriendsCount];
    struct NODE* leftNode;
    struct NODE* rightNode;
};


// inserts new node to binary tree
struct NODE* insertNewUser(struct NODE* root, struct NODE* node){
    struct NODE* current;
    struct NODE* parent;

    if(root == NULL){
        root = node;
    } else{
        current = root;
        parent = NULL;
        struct NODE* temp = current;
        while(1){
            parent = current;
            // if node's number is less than parent's number, go to left of the parent
            if(node->userIdentity < parent->userIdentity){
                current = current->leftNode;

                if(current == NULL){
                    parent->leftNode = node;
                    return temp;
                }
            }
            // if node's number is greater than parent's number, go to right of the parent
            else {
                current = current->rightNode;

                if(current == NULL){
                    parent->rightNode = node;
                    return temp;
                }
            }
        }
    }

    return root;
}

// finds the node which has minimum identity value of the tree
struct NODE* minNode(struct NODE* node){
    struct NODE* current = node;

    // loop to find leftmost node which has min value
    while(current->leftNode->userIdentity > 0){
        current = current->leftNode;
    }

    return current;
}

// deletes the node which has given identity value and keeps the binary tree order rule.
struct NODE* deleteUser(struct NODE* node, int identity){
    if(node == NULL){
        return node;
    }

    if(identity > node->userIdentity){
        node->rightNode = deleteUser(node->rightNode, identity);
    } else if (identity < node->userIdentity){
        node->leftNode = deleteUser(node->leftNode, identity);
    } else {

        // child count of node is less than 2
        if(node->leftNode == NULL){
            struct NODE* tmp = node->rightNode;
            free(node);
            return tmp;
        }
        else if(node->rightNode == NULL){
            struct NODE* tmp = node->leftNode;
            free(node);
            return tmp;
        }

        //child of count is 2
        //min value of right part will be replaced with deleted node to provide tree rule again.
        struct NODE* tmp = minNode(node->rightNode);
        node->userIdentity = tmp->userIdentity;
        node->rightNode = deleteUser(node->rightNode, tmp->userIdentity);
    }

    return node;

}

// finds the node which has given identity number and prints the username of that node
struct NODE* contains(struct NODE* node,int identity, int writeEnable){
    // if tree is null.
    if(node == NULL){
        printf("Tree is null. Not found.");
        return NULL;
    }
    // if identity is found in root.
    if(node->userIdentity == identity){
        if(writeEnable == 1){
            printf("%s\n", node->username);
        }
        return node;
    }

    struct NODE* current = node;
    while(current->userIdentity != identity){

        if(current->userIdentity < identity){
            current = current->rightNode;
        } else {
            current = current->leftNode;
        }

        if(current == NULL){
            return NULL;
        }

        if(current->userIdentity == identity){
            if(writeEnable == 1){
                printf("%s\n", current->username);
            }
            return current;
        }
    }

    return NULL;
}

// finds the node which has given identity value
// and prints all friend names of that node
void friends(struct NODE* node, int identity){
    struct NODE* targetNode = contains(node, identity, 0);
    if(targetNode != NULL){
        printf("----Friends Of User----\n");
        int i = 0;
        int nextFriendIdentity = targetNode->userFriendsIdentities[0];
        while(i < MaxFriendsCount && nextFriendIdentity > 0){
            contains(node, nextFriendIdentity, 1);
            i++;
            nextFriendIdentity = targetNode->userFriendsIdentities[i];
        }
    } else {
        printf("User is not found.");
    }

}

//calculates the total node count of tree
int size(struct NODE* node){
    if(node == NULL){
        return 0;
    }

    return (size(node->leftNode) + 1 + size(node->rightNode));
}

//prints identity and username of all nodes from leftmost part to rightmost part
void printInOrder(struct NODE* node){
    if (node != NULL) {
        printInOrder(node->leftNode);
        if(node->userIdentity > 0){
            printf("%d, %s\n", node->userIdentity, node->username);
        }
        printInOrder(node->rightNode);
    }
}

//prints username of all nodes from leftmost part to rightmost part
void printInOrderUsername(struct NODE* node){
    if (node != NULL) {
        printInOrder(node->leftNode);
        if(node->userIdentity > 0){
            printf("%s\n", node->username);
        }
        printInOrder(node->rightNode);
    }
}

// prints nodes which are under the node has given identity number
void printNext(struct NODE* node, int identity){
    struct NODE* targetUser = contains(node, identity, 0);
    if(targetUser != NULL){
        printInOrderUsername(targetUser->leftNode);
        printInOrderUsername(targetUser->rightNode);
    } else {
        printf("Node has no child.");
    }
}

// prints nodes whose identity number is greater than given identity number
void printGreater(struct NODE* node, int identity){

    if(node == NULL){
        return;
    }
    if(node->userIdentity > identity){
        printf("%d, %s\n", node->userIdentity, node->username);
    }
    printGreater(node->leftNode, identity);
    printGreater(node->rightNode, identity);
}

// split identityList with "-"
void splitIdentityPart(char* identityText, int* identityList){
    char delimeter[2] = "-";
    char* ptr = strtok(identityText, delimeter);
    int i = 0;
    while(ptr != NULL){
        identityList[i] = atoi(ptr);
        ptr = strtok(NULL, delimeter);
        i++;
    }
}

// split the line with comma and fill the node properties with comma seperated parts.
void splitLineAndSetNode(char* text, const char* delimeter, struct NODE* node){
    char* ptr =  strtok(text, delimeter);
    int i = 0;
    while(ptr != NULL){
        // first part of line and it is identity of user
        if(i == 0){
            node->userIdentity = atoi(ptr);
        }
        // second part of line and it is name of user
        else if(i == 1){
            strcpy(node->username, ptr);
        }
        // third part of line and it is consisting of identity list and it means friends of user
        else if(i == 2){
            splitIdentityPart(ptr, node->userFriendsIdentities);
        }

        ptr = strtok(NULL, delimeter);
        i++;
    }
}


// evaluates the text comes from user input and runs a function regarding to command.
void RunFunction(struct NODE* tree){

    char functionCall[1000];
    printf("Please enter a function you want to call?\n");
    fgets(functionCall, 1000, stdin);
    char delimeter[2] = ",";

    char* ptr =  strtok(functionCall, &delimeter);
    if(ptr != NULL){

        char* text;
        text = strchr(functionCall, delimeter);

        if(strcmp(ptr,"insertNewUser") == 0){
            struct NODE* node = (struct NODE*) malloc(sizeof(struct NODE));
            node->leftNode = NULL;
            node->rightNode = NULL;
            splitLineAndSetNode(text, delimeter, node);
            insertNewUser(tree, node);
            RunFunction(tree);
        } else if(strcmp(ptr,"deleteUser") == 0){
            ptr = strtok(NULL, &delimeter);
            deleteUser(tree, atoi(ptr));
            RunFunction(tree);
        } else if(strcmp(ptr,"contains") == 0){
            ptr = strtok(NULL, &delimeter);
            contains(tree, atoi(ptr),1);
            RunFunction(tree);
        } else if(strcmp(ptr,"friends") == 0){
            ptr = strtok(NULL, &delimeter);
            friends(tree, atoi(ptr));
            RunFunction(tree);
        } else if(strcmp(functionCall,"size\n") == 0){
            printf("%d\n", size(tree));
            RunFunction(tree);
        } else if(strcmp(ptr,"printNext") == 0){
            ptr = strtok(NULL, &delimeter);
            printNext(tree, atoi(ptr));
            RunFunction(tree);
        } else if(strcmp(ptr,"printGreater") == 0){
            ptr = strtok(NULL, &delimeter);
            printGreater(tree, atoi(ptr));
            RunFunction(tree);
        } else if(strcmp(ptr,"printInOrder\n") == 0){
            printInOrder(tree);
            RunFunction(tree);
        } else if(strcmp(functionCall,"exit\n") == 0){
            exit(0);
        } else {
            printf("Please be sure that you type correct expression\n");
            RunFunction(tree);
        }
    }
}

// Prints the detailed menu of program.
void programMenu(struct NODE* tree){
    printf("----FunctionList----\n");
    printf("1. insertNewUser | Usage => insertNewUser,identity,username,friends | Example => insertNewUser,101,hasan bolu,59-1\n");
    printf("2. deleteUser    | Usage => deleteUser,identity                     | Example => deleteUser,1\n");
    printf("3. contains      | Usage => contains,identity                       | Example => contains,59\n");
    printf("4. friends       | Usage => friends,identity                        | Example => friends,59\n");
    printf("5. size          | Usage => size                                    | Example => size\n");
    printf("6. printNext     | Usage => printNext,identity                      | Example => printNext,38\n");
    printf("7. printGreater  | Usage => printGreater,identity                   | Example => printGreater,42\n");
    printf("8. printInOrder  | Usage => printInOrder                            | Example => printInOrder\n");
    printf("9. exit          | Usage => exit                                    | Example => exit\n");
    printf("\n");
    RunFunction(tree);
}

int main()
{
    FILE* file = fopen("Input.txt", "r");

    int line = 0;
    char text[512];
    struct NODE* tree = NULL;

    while(fgets(text, 512, file)){
        line++;
        char delimeter[2] = ",";
        struct NODE* node = (struct NODE*) malloc(sizeof(struct NODE));
        node->leftNode = NULL;
        node->rightNode = NULL;
        splitLineAndSetNode(&text, &delimeter, node);
        tree = insertNewUser(tree, node);
    }

    programMenu(tree);
}




