# helloWorld3.s 
# "hello world" program using the write() system call 
# one character at a time. 
# Bob Plantz - 12 June 2009 
 
# Useful constants 
        .equ    STDOUT,1 
# Stack frame 
       .equ    aString,-8 
        .equ    localSize,-16 
# Read only data 
        .section  .rodata 
theString: 
        .string "Hello world.\n" 
# Code 
        .text 
        .globl  main 
        .type   main, @function 
main: 
        pushq   %rbp        # save base pointer 
        movq    %rsp, %rbp  # set new base pointer 
        addq    $localSize, %rsp  # for local var. 
 
        movl    $theString, %esi 
        movl    %esi, aString(%rbp) # *aString = "Hello World.\n"; 
whileLoop: 
        movl    aString(%rbp), %esi # current char in string 
       cmpb    $0, (%esi)  # null character? 
        je      allDone     # yes, all done 
 
        movl    $1, %edx    # one character 
        movl    $STDOUT, %edi  # standard out 
        call    write       # invoke write function 
 
        incl    aString(%rbp)  # aString++; 
        jmp     whileLoop   # back to top 
allDone: 
        movl    $0, %eax    # return 0; 
        movq    %rbp, %rsp  # restore stack pointer 
        popq    %rbp        # restore base pointer 
        ret
