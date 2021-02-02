# SchemeInterpreter
An Scheme programming language interpreter written in C++

Scheme Command compiler
Programmed by Jae Hyun Park
Written in C++ in Visual Studio, created in a Windows environment
Compiles with a Memory node tree 100 and Hash table of size 300
Note that it only accepts Scheme commands!

//////////////////////////////////////////////////////////////////////////////////////

How to use:

Enter your input scheme command after "Enter your code" prompt

Two different cases of an Output is printed after >
- If your command defines a function of a variable ("define") : prints the inputted command
- otherwise prints : "Eval result : " and the evaluated result
Previously defined values are saved into memory node tree, hence you can call previously defined variables
Once Memory node tree reaches its limit, garbage collection is executed and only previously defined variables are stored. Input your command again to execute the command


