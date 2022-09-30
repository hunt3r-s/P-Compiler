# Compiler Development  
## Hunter Smith  

This project shows the various steps needed to form a functioning compiler for portable machine code.

## P-Machine Implementation  
### Description:  
The [vm.c](vm.c) file is designed to emmulate a virtual machine which executes portable machine code. This machine contains a process address space which is divided into an execution text and an instruction stack. There are 3 space delimited fields in each instruction:  

- **OP** the operation code  
- **L** the lexical level  
- **M** a modifier operator  

The following are the possible operations of the modifier operator:  

- **LIT 0,M** push **M** onto stack  
- **OPR 0,M** peform operation to top of stack  
- **LOD L,M** load value from offset **M**, **L** levels down and push to top of stack  
- **STO L,M** store value from offset **M**, **L** levels down  
- **CAL L,M** call procedure at code index **M**  
- **INC 0,M** allocate **M** memory addresses  
- **JMP 0,M** jump to instruction **M**  
- **JPC 0,M** jump to instruction **M** if top of stack is 0  
- **WRT 0,1** write top of stack to output  
- **RED 0,2** Read input from user, store at top of stack  
- **HLT 0,3** End program  

### Compile:  
gcc vm.c  

### Run:  
./"outputfilename".out "inputfile".txt  
