/*
* This program parses a string based on Context-Free Grammar
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FANOUT 100
#define MAX_STRING_SIZE 1000
#define MAX_STACK_SIZE 100

#define max(a,b) (a>b)?a:b

typedef char lhs; // lhs is a single char
typedef char rhs[MAX_FANOUT]; // rhs is a unitialized C String of size MAX_FANOUT
/*
* @brief production struct where LHS is a non-terminal and RHS is string over set of terminals and non-terminals
*/
struct production{
    lhs LHS;
    rhs RHS;
};

typedef struct production production;

// struct transition{
//     char p;
// };

// struct pda{
//     char* states;
//     char start_state;
//     char* final_states;
//     int* terminals;
//     int* non_terminals;
// };


struct stack
{
    char s[MAX_STACK_SIZE];
    int stack_size;
    int top;
};

typedef struct stack stack;


void initialize_stack(stack* st,int stack_size)
{
    st->stack_size = stack_size;
    st->top=-1;
}

bool isEmpty(stack *st) {
    return st->top == -1;  
}

bool isFull(stack *st) {
    return st->top == st->stack_size - 1;  
}

bool push(stack *st, char c) {
    if (isFull(st)) {
        printf("stack Overflow\n");
        return false;
    }
    st->s[++st->top] = c;
    return true;
}

char pop(stack *st) {
    if (isEmpty(st)) {
        printf("stack Underflow\n");
        return -1;
    }
    char popped = st->s[st->top];
    st->top--;
    return popped;
}

char peek(stack *st) {
    if (isEmpty(st)) {
        printf("stack is empty\n");
        return -1;
    }
    return st->s[st->top];
}

void print_stack(stack st)
{
    if(st.top == -1)
    {
        printf("The Stack is Empty\n");
        return;
    }
    for(int i=st.top;i>=0;i--)
    {
        printf("|  %c  |\n",st.s[i]);
        printf("|-----|\n");
    }
    
}

int get_production_index(char non_terminal,production productions[],int production_count)
{
    int l=0,r=production_count-1,m=-1;
    int index = -1;
    while(l<=r) {
        m = (l+r)/2;
        int rv = non_terminal - productions[m].LHS;
        if(rv == 0) {
            index = m;
            break;
        }
        else if(rv>0) {
            l = m + 1;
        }
        else {
            r = m - 1;
        }
    }
    if(index == -1) return -1;
    while(index > 0 && productions[index].LHS == productions[index-1].LHS) index--;
    return index;
}

/* 
* @brief Standard Comparator Function for qsort
* @param a production a
* @param b production b
* @return >0 if a.lhs is lexicographically greater than b.lhs. <= 0 otherwise
*/ 
int comp(const void* a,const void* b){
   return (*(const production*)a).LHS - (*(const production*)b).LHS;
}
/*
* @brief checks validness of production based on terminals array and non-terminals array
*
* @param production A struct of Production type
* @param terminals_array An array of size 26. If x is not a terminal then terminals_array[x-'a'] = 0
* @param non_terminals_array An array of size 26. If X is not a non-terminal then non_terminals_array[X-'A'] = 0
*
* @returns true if production is valid. false otherwise.
*/
bool isValidProduction(production production,int terminals_array[],int non_terminals_array[])
{
    if(non_terminals_array[production.LHS-'A'] == 0) return false;
    int len = strlen(production.RHS);
    if(len == 1 && production.RHS[0] == 'e') return true;
    for(int i=0;i<len;i++)
    {
        char c = production.RHS[i];
        if(c >= 'A' && c <= 'Z'){
            if(non_terminals_array[c-'A'] == 0) return false;
        }
        else if(c >= 'a' && c <= 'z'){
            if(terminals_array[c-'a'] == 0) return false;
        }
        else return false;
    }
    return true;
}

bool isTerminal(char c,int terminals_array[])
{
    if(c >= 'a' && c <= 'z' && terminals_array[c-'a'] != 0) return true;
    else return false;
}

bool isNonTerminal(char c,int non_terminals_array[])
{
    if(c >= 'A' && c <= 'Z' && non_terminals_array[c-'Z'] != 0) return true;
    else return false;
}

void deleteFirstChar(char str[])
{
    int n = strlen(str);
    if(n == 1)
    {
        str[0] = '\0';
        return;
    }
    for(int i=0;i<n-1;i++)
    {
        str[i] = str[i+1];
    }
    str[n-1] = '\0';
    
}

void addFirstChar(char str[],char c)
{
    int n = strlen(str);
    if(n == 0)
    {
        str[0] = c;
        str[1] = '\0';
        return;
    }
    for(int i=1;i<n;i++)
    {
        str[i] = str[i-1];
    }
    str[0] = c;
    str[n+1] = '\0';
    
}


int pushRHS(stack *st,char str[])
{
    int n = strlen(str);
    if(st->top + n >= st->stack_size) return -1;
    for(int i = n-1;i>=0;i--) {
        push(st,str[i]);
    }
    return n;
}

bool child_routine(stack *st,production productions[],int production_count,char unread_input[],int terminals_array[],int non_terminals_array[],int str_ptr)
{
    if(isEmpty(st) && strlen(unread_input) == str_ptr)
    {
        return true;
    }
    if(isEmpty(st)) return false;
    if((!isEmpty(st) && strlen(unread_input) == str_ptr) || (isEmpty(st) && strlen(unread_input) != str_ptr))
    {
        return false;
    }
    if(st->top > st->stack_size-1) return false;
    // for(int i=str_ptr;i<strlen(unread_input);i++)
    // {
    //     printf("%c",unread_input[i]);
    // }
    // printf("\n");
    char curr_symbol = pop(st);
    if(isTerminal(curr_symbol,terminals_array)) { // top of the stack is a terminal
        if(curr_symbol == unread_input[str_ptr]) {
            // deleteFirstChar(unread_input);
            str_ptr++;
            bool rv = child_routine(st,productions,production_count,unread_input,terminals_array,non_terminals_array,str_ptr);
            if(rv == true) return true;
            // addFirstChar(unread_input,curr_symbol);
            str_ptr--;
        }
        push(st,curr_symbol);
        return false;
    }
    else { // top of the stack is a non terminal
        int i = get_production_index(curr_symbol,productions,production_count);
        if(i == -1) return false;
        int n = 0;
        if(strcmp(productions[i].RHS,"e") != 0)
        {
            n = pushRHS(st,productions[i].RHS);
            if(n==-1) {
                push(st,curr_symbol);
                return false;
            }
        }
        bool rv = child_routine(st,productions,production_count,unread_input,terminals_array,non_terminals_array,str_ptr);
        if(rv == true) return true;
        while(n--) pop(st);
        while(i<production_count-1 && productions[i].LHS == productions[i+1].LHS)
        {
            i++;
            n = 0;
            if(strcmp(productions[i].RHS,"e") != 0)
            {
                n = pushRHS(st,productions[i].RHS);
                if(n==-1) {
                    push(st,curr_symbol);
                    return false;
                }
            }
            rv = child_routine(st,productions,production_count,unread_input,terminals_array,non_terminals_array,str_ptr);
            if(rv == true) return true;
            while(n--) pop(st);
        }
        push(st,curr_symbol);
    }
    return false;
}


int main(void){
    int terminals_count = 0;
    printf("Enter Number of Terminals - ");
    scanf("%d",&terminals_count);

    if(terminals_count <= 0){
        printf("Error : Number of Terminal should be greater than 0. You entered : %d.\n",terminals_count);
        exit(EXIT_FAILURE);
    }

    int terminals_array[26] = {0};
    printf("Enter terminals (Please make sure that each terminal is UNIQUE and is a SINGLE LOWERCASE ALPHABET)\n");
    printf("Please do not use 'e' as terminal as it is reserved to denote empty string\n");
    for(int i=0;i<terminals_count;i++){
        char c;
        getchar();
        scanf("%c",&c);
        terminals_array[c-'a'] = 1;
    }

    int non_terminals_count = 0;
    printf("Enter Number of Non-Terminals - ");
    scanf("%d",&non_terminals_count);
    if(non_terminals_count <= 0){
        printf("Error : Number of Non-Terminal should be greater than 0. You entered : %d.\n",non_terminals_count);
        exit(EXIT_FAILURE);
    }

    int non_terminals_array[26] = {0};
    printf("Enter non-terminals (Please make sure that each terminal is UNIQUE and is a SINGLE UPPERCASE ALPHABET)\n");

    for(int i=0;i<non_terminals_count;i++){
        char c;
        getchar();
        scanf("%c",&c);
        non_terminals_array[c-'A'] = 1;
    }

    char starting_non_terminal = 0;
    printf("Enter Starting Non-terminal - ");
    getchar();
    scanf("%c",&starting_non_terminal);
    if(!(starting_non_terminal >= 'A' && starting_non_terminal <= 'Z' && non_terminals_array[starting_non_terminal-'A'] != 0)){
        printf("Error : Starting non-terminal is either not a uppercase alphabet or is not present in set of non-terminals\n");
        exit(EXIT_FAILURE);
    }

    int production_count = 0;
    int fanout = 0;
    printf("Enter Number of Production Rules - ");
    scanf("%d",&production_count);
    production productions[production_count];
    printf("Enter each production one by one.\nInput Format - A X. A is a SINGLE NON-TERMINAL and X is a COMBINATION OF TERMINAL AND NON-TERMINALS.\n");
    printf("Example - if production rule is S->aSb, input should be 'S aSb'\n");
    for(int i=0;i<production_count;i++)
    {
        getchar();
        scanf("%c %s",&productions[i].LHS,&productions[i].RHS);
        fanout = max(fanout,strlen(productions[i].RHS));
        if(!isValidProduction(productions[i],terminals_array,non_terminals_array))
        {
            printf("Error : Above Production in invalid.\n");
            exit(EXIT_FAILURE);
        }
    }

    qsort((void*)productions,production_count,sizeof(productions[0]),comp); // sort the productions acc. to their LHS. This will help when searching for production based on their LHS

    char input_str[MAX_STRING_SIZE];
    printf("Enter input string (if the input string is empty, please enter \"e\") - ");
    scanf("%s",&input_str);
    if(strcmp(input_str,"e") == 0) input_str[0] = '\0';
    int input_len = strlen(input_str);
    for(int i=0;i<input_len;i++)
    {
        if(terminals_array[input_str[i]-'a'] == 0)
        {
            printf("Error : The input string has character(s) other than terminals.\n");
            exit(EXIT_FAILURE);
        }
    }
    if(strlen(input_str) == 0) {
        int index = get_production_index(starting_non_terminal,productions,production_count);
        if(strcmp(productions[index].RHS,"e") == 0)
        {
            printf("ACCEPTED\n");
            return 0;
        }
        else {
            while(index < production_count-1 && productions[index+1].LHS == productions[index].LHS)
            {
                index++;
                if(strcmp(productions[index].RHS,"e") == 0)
                {
                    printf("ACCEPTED\n");
                    return 0;
                }
            }
        }
        printf("REJECTED\n");
    }
    stack st;
    initialize_stack(&st,fanout*strlen(input_str));
    push(&st,starting_non_terminal);
    bool is_accepted = child_routine(&st,productions,production_count,input_str,terminals_array,non_terminals_array,0);
    if(is_accepted) printf("ACCPETED\n");
    else printf("REJECTED\n");
    return 0;
}