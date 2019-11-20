# dm_hw2
## 1. Instruction
    [Input]
        propositional formula
    [Output]
        not : -
        and : "\n"
        or : " "

    
## 2. Manual

  ### How to build?
    make
   #### [makefile]
  ```c
    run : dnf.o
    gcc -o run dnf.o
    dnf.o : dnf.c
    gcc -c -o dnf.o dnf.c
  ```
  
  ### How to run?
    ./run (file name)
