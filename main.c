#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define STACKSIZE 50

//Part One: Stack Creation.
typedef union
{
    float fData;
    char cData;
} Item;

typedef struct
{
    int topIndex;
    Item items[STACKSIZE];
} Stack;

Stack* initialize();
Item pop (Stack *s);
void push(Stack *s,Item value);
Item top (Stack *s);
int isEmpty(Stack *s);
int isFull(Stack *s);

Stack* initialize()
{
    Stack *s = malloc(sizeof(Stack));
    s->topIndex = -1;
    if (s == NULL)
    {
        printf("Fatal error.\n");
        exit(-1);
    }
    return s;
}

Item pop(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Stack is empty.\n");
        exit(-1);
    }
    else
        return s->items[s->topIndex--];
}

void push(Stack *s, Item value)
{
    if (isFull(s))
        printf("Stack is full.\n");
    else
        s->items[++s->topIndex] = value;
}

Item top (Stack *s)
{
    return s->items[s->topIndex];
}

int isEmpty(Stack *s)
{
    return (s->topIndex == -1) ? 1 : 0;
}

int isFull(Stack *s)
{
    return (s->topIndex == STACKSIZE - 1) ? 1 : 0;
}

//Part Two: Infix to Postfix Conversion.
int precedence (char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        printf("Invalid input.\n");
        exit(-1);
    }
}

int isLowerPrecedence (char ch1, char ch2)
{
    return precedence(ch1) < precedence(ch2);
}

void infixToPostfix(char* infix, char* postfix)
{
    //First: separate numbers and operators in string.
    int i = 0;
    Stack* s = initialize();
    Item myItem;
    char temp[50];
    strcpy(temp, infix);
    char* token = strtok(temp, " ");
    while (token != NULL)
    {
        if(token[0] == '(' || token[0] == ')' || token[0] == '^'
                || token[0] == '*' || token[0] == '/' || token[0] == '+'
                || (token[0] == '-' && token[1] == '\0'))
            //if item is an operator, then we need to push it.
        {
            myItem.cData = token[0];
            if (isEmpty(s) || myItem.cData == '(')
                push(s, myItem);
            else if (myItem.cData == ')')
            {
                while (top(s).cData != '(')
                {
                    postfix[i++] = pop(s).cData;
                    postfix[i++] = ' ';
                }
                pop(s);
            }
            else
            {
                if (top(s).cData != '(')
                {
                    if (isLowerPrecedence(myItem.cData, top(s).cData))
                    {
                        postfix[i++] = pop(s).cData;
                        postfix[i++] = ' ';
                    }
                }
                push(s, myItem);
            }
        }
        else
            //if an item is a number instead:
        {
            int j;
            for(j = 0; j < strlen(token); j++)
                postfix[i++] = token[j];
            postfix[i++] = ' ';
        }
        token = strtok(NULL, " ");
    }
    while(!isEmpty(s))
    {
        postfix[i++] = pop(s).cData;
        postfix[i++] = ' ';
    }
    postfix[i] = '\0';
    free(s);
}

//Part Three: Postfix Evaluation
float evaluatePostfix(char* postfix)
{
    Stack* s = initialize();
    Item myItem;
    char temp[50];
    strcpy(temp, postfix);
    char* token = strtok(temp, " ");
    while (token != NULL)
    {
        if(token[0] == '^' || token[0] == '*' || token[0] == '/'
                || token[0] == '+'|| (token[0] == '-' && token[1] == '\0'))
        {
            float a, b;
            a = pop(s).fData;
            b = pop(s).fData;
            switch (token[0])
            {
            case '+':
                myItem.fData = b + a;
                push(s, myItem);
                break;
            case '-':
                myItem.fData = b - a;
                push(s, myItem);
                break;
            case '*':
                myItem.fData = b * a;
                push(s, myItem);
                break;
            case '/':
                myItem.fData = b / a;
                push(s, myItem);
                break;
            case '^':
                myItem.fData = pow(b, a);
                push(s, myItem);
                break;
            default:
                printf("Invalid input.");
                exit(-1);
                }
        }
        else
        {
            myItem.fData = atof(token);
            push(s, myItem);
        }
        token = strtok(NULL, " ");
    }

    return pop(s).fData;
}
int main()
{
    char infix[50], postfix[50];
    printf("Enter an infix expression: ");
    gets(infix);
    infixToPostfix(infix, postfix);
    printf("\nPostfix: %s\nValue: %.2f\n", postfix, evaluatePostfix(postfix));
    return 0;
}
