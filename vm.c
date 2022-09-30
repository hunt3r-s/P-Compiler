// P-Machine
// Hunter Smith
// Simple virtual machine which executes p-code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 500

// Stores each instruction 
typedef struct Instruction{
    int op;
    int l;
    int m;
}Instruction;

// Declare process address space
int text[ARRAY_SIZE];
int stack[ARRAY_SIZE];

// Declare registers
Instruction instruction;
int bp,sp,pc;

// Other required variables
int halt, print, temp;

// Function declarations 
int base(int *text, int bp, int l);
void print_instruction(int pc, Instruction instruction);
void print_stack(int pc, int bp, int sp, int print, int *text, int *stack);

 
int main(int argc, char * argv[]){

    // Get instruction input file name
    char * inputFile;
    inputFile = malloc(strlen(argv[1]) + 1);

    // If malloc fails
    if (inputFile == NULL) {
        printf("Error finding input file from given string.\n");
        exit(0);
    }
    
    if(argc == 2){
        strcpy(inputFile, argv[1]);
    }
    else{
        printf("Invalid command line arguments given, please specify an output and input file.\n");
        exit(0);
    }
    
    FILE *in = fopen(inputFile, "r");
    
    // Read and format text from input file 
    if(in == NULL) {
        printf("Could not open file.\n");
        exit(0);
    }
    int i = 0;
	while (fscanf(in, "%d", &temp) != EOF)
	{
		text[i] = temp;
        i++;
	}

    // Initializing values
    Instruction *instruction = malloc(sizeof(Instruction));
    bp = i;
    sp = bp - 1;
    print = sp + 1;
    pc = 0;
    halt = 1;
    // One time print statements 
    printf("VM execution:\t\t\tPC\tBP\tSP\tstack\n");
    printf("Initial values:\t\t");

    // Start of loop ending by halt flag 
    while(halt)
    {

        print_stack(pc, bp, sp, print, text, stack);
        // Fetch
        instruction->op = text[pc];
        instruction->l = text[pc + 1];
        instruction->m = text[pc + 2];

        pc += 3;
        print_instruction(pc, *instruction);        

        // Execution
        switch(instruction->op)
        {
        case 1: // LIT  
            sp = sp + 1;
            text[sp] = instruction->m;      
            break;
        case 2:
            switch(instruction->m)
            {
            case 0: // RTN:
                sp = bp -1;
                bp = text[sp + 2];
                pc = text[sp + 3];
                break;
            case 1: // ADD
                text[sp - 1] = text[sp - 1] + text[sp];
                sp = sp - 1;
                break;
            case 2: // SUB
                text[sp - 1] = text[sp - 1] - text[sp];
                sp = sp - 1;
                break;
            case 3: // MUL 
                text[sp - 1]= text[sp - 1] * text[sp];
                sp = sp - 1;
                break;
            case 4: // DIV
                text[sp - 1] = text[sp - 1] / text[sp];
                sp = sp - 1;
                break;
            case 5: // EQL 
                text[sp - 1] = text[sp - 1] == text[sp];
                sp = sp - 1;
                break; 
            case 6: // NEQ
                text[sp - 1] = text[sp - 1] != text[sp];
                sp = sp - 1;
                break;
            case 7: // LSS
                text[sp - 1] = text[sp - 1] < text[sp];
                sp = sp - 1;
                break; 
            case 8: // LEQ
                text[sp - 1] = text[sp - 1] <= text[sp];
                sp = sp - 1;
                break;
            case 9: // GTR
                text[sp - 1] = text[sp - 1] > text[sp];
                sp = sp - 1;
                break;
            case 10: // GEQ
                text[sp - 1] = text[sp - 1] >= text[sp];
                sp = sp - 1;
                break; 
            }                               
            break;
        case 3: // LOD
            sp = sp + 1;
            text[sp] = text[base(text,bp,instruction->l) + instruction->m];
            break;
        case 4: // STO 
            text[base(text,bp,instruction->l) + instruction->m] = text[sp];
                sp = sp - 1;
                break;
        case 5: // CAL
            text[sp + 1] = base(text,bp,instruction->l);
            text[sp + 2] = bp;
            text[sp + 3] = pc;
            bp = sp + 1;
            pc = instruction->m;
            stack[sp] = 1;
            break;
        case 6: // INC
            sp = sp + instruction->m;
            break;
        case 7:// JMP 
            pc = instruction->m;
            break;
        case 8: // JPC
            if(text[sp] == 0){
                pc = instruction->m;
            }
            sp = sp - 1;
            break;
        case 9: // M
            switch(instruction->m){
            case 1: // WRT 
                printf("\nOutput: %d\t\t", text[sp]);
                sp = sp - 1;
                break;
            case 2: // RED 
                sp = sp + 1;
                printf("\nInput:\t");
                scanf("%d", &text[sp]);
                printf("\t\t\t");
                break;
            case 3: // HLT 
                halt  = 0;
                break;
            }
            break;
        default:
            printf("default catch");
            return 1;
            break;
        }
    }
    print_stack(pc, bp, sp, print, text, stack);
}

// Base from class
int base(int *pas, int BP, int L)
{
    int arb = BP; // arb = activation record base
    while ( L > 0)     //find base L levels down
        {
            arb = pas[arb];
            L--;
        }
    return arb;
}

// Print stack to format
void print_stack(int pc, int bp, int sp, int print, int *text, int *stack)
{
    int i;
    printf("\t%d\t%d\t%d\t", pc, bp, sp);
    for (i = print; i <= sp; i++)
        {
            if (stack[i] == 1){
            printf("| %d ", text[i]);
            }
            else
            printf("%d ", text[i]);
        }
    printf("\n");
}

// Print instructions to format
void print_instruction(int PC, Instruction instruction)
{
    char opname[4];
    switch (instruction.op)
    {
        case 1 : strcpy(opname, "LIT"); break;
        case 2 :
            switch (instruction.m)
            {
                case 0 : strcpy(opname, "RTN"); break;
                case 1 : strcpy(opname, "ADD"); break;
                case 2 : strcpy(opname, "SUB"); break;
                case 3 : strcpy(opname, "MUL"); break;
                case 4 : strcpy(opname, "DIV"); break;
                case 5 : strcpy(opname, "EQL"); break;
                case 6 : strcpy(opname, "NEQ"); break;
                case 7 : strcpy(opname, "LSS"); break;
                case 8 : strcpy(opname, "LEQ"); break;
                case 9 : strcpy(opname, "GTR"); break;
                case 10 : strcpy(opname, "GEQ"); break;
                default : strcpy(opname, "err"); break;
            }
        break;
        case 3 : strcpy(opname, "LOD"); break;
        case 4 : strcpy(opname, "STO"); break;
        case 5 : strcpy(opname, "CAL"); break;
        case 6 : strcpy(opname, "INC"); break;
        case 7 : strcpy(opname, "JMP"); break;
        case 8 : strcpy(opname, "JPC"); break;
        case 9 : 
        switch (instruction.m)
        {
            case 1 : strcpy(opname, "WRT"); break;
            case 2 : strcpy(opname, "RED"); break;
            case 3 : strcpy(opname, "HLT"); break;
            default : strcpy(opname, "err"); break;
        }
    break;
    default : strcpy(opname, "err"); break;
}
printf("%d\t%s\t%d\t%d", (PC - 3)/3, opname, instruction.l, instruction.m);
}
