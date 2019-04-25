# FnLoC - Counts Logical Lines of Code
Counts logical lines of code and functions in C and C++ source code files.

Copyright 2018-2019, Richard Romig

Licensed under the GNU General Public License, version 2.0.

### Description:
1. **FnLoC** is a program that runs from a command-line which counts logical lines of code in C and C++ source code files, disregarding comments and blank lines. It also counts and lists functions by name displays their respective lines of code counts. The program assumes that the code is written according to modern C coding standards as illustrated in _The C Programming Language, 2nd edition_ by Brian W. Kernighan & Dennis M. Ritchie. Comments and blank lines are not counted as lines of code. The programs takes into account both C and C++ style comments. Lines containing only opening or closing braces ({}) are not counted as lines of code.
2. **LLoC** is an accompanying program that simply counts logical lines of source code, disregarding blank lines and comments without the breakdown into functions.
3. FnLoC and LLoC are standalone programs with no known dependencies and were written using only standard C libraries.
4. Project source files:

| File | Notes |
|------|-------|
| fnloc.c | FnLoC source file
| fnloc.h | FnLoC header file
| lloc.c | LLoC source file
| lloc.h | LLoC header file

### Compiling from source:
The source code can be compiled with any C compiler or IDE on your system. I have compiled it using  the MinGW GCC compiler.

### Installation:
1. xtract the FnLoC_Windows.zip, this create the FnLoC_Windows folder containing all the files. Right-clicking the zipped file and selecing 'Extract All...' from the menu will extract the files to the folder.
2. Launch FnLoC-install.bat as administrator from inside the FnLoC_Windows folder
from within the either Windows File Explorer or from a command prompt (run as
administrator). The batch file will create a FnLoC folder under `C:\Program Files (x86)` and copy all of the files to the FnLoC folder.

### Program removal:
1. The FnLoC-uninstall.bat script will remove FnLoC if it was installed with
FnLoC-install.bat as explained above.
2. Open the FnLoC_Windows folder and click on FnLoC-uninstall.bat or
run the batch file from a command prompt.
3. Errors reported in this script just mean it could not find all of the FNLOC
files. This usually means FnLoC was not installed with FnLoC-install.bat and it
will have to be removed manually.
4. FnLoC can be removed manually by deleting the installed files from their respective directories.

### Syntax
1. Open a command prompt and navigate to the directory containing your source code files.
2. To display the lines of code data of a C source code file run the following command from a command prompt window
```
fnloc.exe sourcefile.c
```
3. The results can be redirected to a file by redirecting the output:
```
fnloc.exe source.c > loc.txt
```
4. To get help and view FnLoC or LLoC syntax, type the program name followed by either -h or --help.
```
fnloc.exe -h
fnloc.exe --help
lloc.exe -h
lloc.exe --help
```
6. If you don't include an argument or if the program fails to open the file passed as an argument it will also call up the help function.

### Program Limitations

1. For FnLoC functions are expected to be in the following style:
```c
        int function_name(int x, int y)
        {
            statements....
        }
```
 * This is the format recommended by Linus Torvalds in [Linux Kernel Coding Style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html) and is based on the style used by K&R in 'The C Programming Language, 2nd Edition'.
 * If the opening brace '{' is on the same line as the function name and parameters, it will not be seen as a function. The lines of code will be counted but as code outside of a function.
 * The program will properly count and display function headers that are split over two lines. Function headers should be limited to one or two lines of less than 128 characters each. The buffer limit is set to 128 characters. However, restraining function headers to a single line and 80 characters or less is a good practice.
2. Data structures should be in the following style:
```c
        struct {
           int len;
           char* str;
        } data;
```
 * This is the style used by Kernighan & Ritchie in _The C Programming Language, 2nd edition_.
 * The GNU C coding standard suggests placing the opening brackets are at the beginning of a line. However, FnLoC will incorrectly identify a structure written in this style as a function.
 * In data structure declarations such as arrays or enumerated types in which the data elements are delimited by commas, the elements inside the braces are not counted if they are not on the same line as a brace.
```c
        int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        /* counted as a single line of code. */

        int days[12] = { 31, 28, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31 };
        /* counted as two lines of code */            

        int days[12] = {
                31, 28, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31 };
        /* also counted as two lines of code (the first and last lines) */
```
3. Conditional statements, for loops and while loops without braces and only one statement following are counted as one line of code.
```c
        if ( condition )
                action;
        /* This is seen as a one logical line of code */

        for ( i=1; i < 10; i++ )
                if (condition)
                    action;
        /* This is seen as a one logical line of code */

        while ( condition )
                action;
        /* This is seen as a one logical line of code */

        do
                action;
        while ( condition );
        /* This is seen as two logical lines of code */

        if ( condition )
        {
            action1;
            action2;
        }
        else
            action3;
        /* This is seen as 4 logical lines of code: (1) if ( condition ), (2) action1,
           (3) action2, (4) else-action3 */
```
4. Conditional and loop statements (if, else, for, while, do-while) where the opening brace is the first non-whtespace character on the line immediately following are counted as a line of code. Conditional statements and loop constructs can have the opening brace at either the end of the line or on the following line as long as it is not the very first character of the line (buffer[0]).
```c
        while ( condition )
        {
                action1;
                action2;
        }

        while ( condition ) {
                action1;
                action2;
        }
```

5. LLoC is more lenient about coding style than FnLoC since it isn't concerned with the syntax for functions. However, data structure definitions, particularly those for arrays and enumerated types are counted exacly the same.
6. On Linux systems, the programs may not correctly count in source files that were created with DOS or Windows text editors because they may embed carriage returns throughout the file. This can be remedied by cleaning them out of the DOS/Windows source files using the following command:
```
sed -i.bak -e 's/\r//g' sourcefile
```
  * The -i.bak flag creates a backup of the original file.
  * There are other methods such as the tr command but I found that sed works quite well.

### Feedback:

Feel free to contact me with comments and suggestions for FnLoC. Also feel free to share any code or ideas that will help me improve this program. I can be reached through my blog, Twitter, and email.

* [GitHub](https://github.com/RickRomig/FnLoC)
* [Rick's Tech Stuff](https://ricktech.wordpress.com)
* [Twitter (@ludditegeek)](https://twitter.com/ludditegeek)
* Email: <rick.romig@gmail.com> or <rb_romig@twc.com>

Richard Romig
25 April 2019

### DISCLAIMER

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL I BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS AND SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
