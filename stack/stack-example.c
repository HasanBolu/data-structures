#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "math.h"

#define MAX 100

struct numberStack {
    int numbers[MAX];
    int top;
};

struct operationStack {
    char operations[MAX];
    int top;
};

int calculateDecimalNumber(int *arr, int length){
    int result = 0;
    int i;
    for (i = 0; i < length; i++) {
        int num = *(arr + i);
        int power = (length - i - 1);
        result = result + num * round(pow(10,power));
        arr[i] = 0;
    }
    return result;
}

void initializeOperationStack(struct operationStack *stack){
    stack->top = -1;
}

void initializeNumberStack(struct numberStack *stack){
    stack->top = -1;
}

void numberStackPush(struct numberStack *stack, int item){
    if (stack->top == 99) {
        return;
    } else {
        stack->top = stack->top + 1;
        stack->numbers[stack->top] = item;
    }
}

int numberStackPop(struct numberStack *stack){
    if (stack->top == -1) {
        return NULL;
    }
    else {
        int num;
        num = stack->numbers[stack->top];
        stack->numbers[stack->top] = NULL;
        stack->top = stack->top - 1;
        return num;
    }
}

void operationStackPush(struct operationStack *stack, char item){
    if (stack->top == 99) {
        printf("Overflow...\n");
        return;
    } else {
        stack->top = stack->top + 1;
        stack->operations[stack->top] = item;
    }
}

char operationStackPop(struct operationStack *stack){
    if (stack->top == -1) {
        return NULL;
    }
    else {
        char operation = stack->operations[stack->top];
        stack->operations[stack->top] = NULL;
        stack->top = stack->top - 1;
        return operation;
    }
}

int calculateOperation(int num1, int num2, char operation){
    int result = 0;
    switch (operation) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '/':
            result = num1 / num2;
            break;
        case '*':
            result = num1 * num2;
            break;
    }
    return result;
}

int getOperationPriority(char operation){
    int priority = 0;
    switch (operation) {
        case '(':
            priority = 3;
            break;
        case '*':
        case '/':
            priority = 2;
            break;
        case '+':
        case '-':
            priority = 1;
            break;
    }
    return priority;
}

void printOperationStack(struct operationStack *opStack){
    if (opStack->top == -1) {
        printf("Operation Stack:\n");
    }
    else{
        printf("Operation Stack:");
    }

    for (int i = 0; i <= opStack->top ; i++) {
        char val = *(opStack->operations + i);
        if (i == opStack->top) {
            printf("%c\n",val);
        } else {
            printf("%c",val);
        }
    }
}

void printNumberStack(struct numberStack *numStack){
    printf("\nNumber Stack:");
    for (int i = 0; i <= numStack->top ; i++) {
        printf("%d ", numStack->numbers[i]);
    }
}

void printStackState(struct numberStack *numStack, struct operationStack *opStack){
    printNumberStack(numStack);
    printOperationStack(opStack);
}

int main(){
    struct numberStack numStack;
    initializeNumberStack(&numStack);
    struct operationStack opStack;
    initializeOperationStack(&opStack);

    char input[200];
    printf("Please enter a operation expression that you want to calculate.\n");
    fgets(input, sizeof input, stdin);

    int arr[3];
    int counter = 0;
    int len = strlen(&input);
    int i;
    for (i = 0; i < len; i++) {
        if (isdigit(input[i])) {
            int num = input[i] - '0';
            arr[counter] = num;
            counter++;
            // inputun son karakteri ise veya sayı 3 haneye ulaştıysa sayının alınması lazım.
            if (counter == 3 || i == len - 1) {
                int result = calculateDecimalNumber(&arr, counter);
                numberStackPush(&numStack, result);
                printStackState(&numStack, &opStack);
                counter = 0;
            }
        } else {
            // 3 haneye ulaşamadan karaktere geçiş olduysa sayıyı kaydet.
            if (counter > 0) {
                int result = calculateDecimalNumber(&arr, counter);
                numberStackPush(&numStack, result);
                printStackState(&numStack, &opStack);
                counter = 0;
            }

            char el = input[i];
            // boşluk karakteri ise pas geç.
            if (isspace(el)) {
                continue;
            }

            char lastOperation = operationStackPop(&opStack);
            // eğer hiç işlem yoksa stack'e kaydet.
            if (lastOperation == NULL) {
                operationStackPush(&opStack, el);
                printStackState(&numStack, &opStack);
            } else {
                // stackteki son işlem ile mevcut okunan işlem önceliğe göre karşılaştırılır.
                // eğer mevcut okunan işlemin önceliği stackteki son işlemden küçük veya eşitse işlem yapılır ve stackler güncellenir.
                int lastPriority =  getOperationPriority(lastOperation);
                int currPriority =  getOperationPriority(el);
                if (lastPriority >= currPriority && lastOperation != '(' && el != ')' ) {
                    int num2 = numberStackPop(&numStack);
                    int num1 = numberStackPop(&numStack);
                    numberStackPush(&numStack, calculateOperation(num1,num2,lastOperation));
                    operationStackPush(&opStack, el);
                    printStackState(&numStack, &opStack);
                } else {
                    operationStackPush(&opStack, lastOperation);
                    // eğer kapa parantez geldiyse ilk görülen aç paranteze kadar işlemler yapılır ve stackler güncellenir.
                    if (el==')') {
                        char currOp = operationStackPop(&opStack);
                        while(currOp != '('){
                            int num2 = numberStackPop(&numStack);
                            int num1 = numberStackPop(&numStack);
                            numberStackPush(&numStack, calculateOperation(num1,num2,currOp));
                            currOp = operationStackPop(&opStack);
                            printStackState(&numStack, &opStack);
                        }
                    } else {
                        operationStackPush(&opStack, el);
                        printStackState(&numStack, &opStack);
                    }
                }
            }
        }
    }

    char operation = operationStackPop(&opStack);
    while(operation != NULL){
        int num2 = numberStackPop(&numStack);
        int num1 = numberStackPop(&numStack);
        numberStackPush(&numStack, calculateOperation(num1,num2,operation));
        printNumberStack(&numStack);
        printOperationStack(&opStack);

        operation = operationStackPop(&opStack);
    }

    printf("\nResult:%d", numberStackPop(&numStack));
    return 0;
}
