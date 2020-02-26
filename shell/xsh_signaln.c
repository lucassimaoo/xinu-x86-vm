#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

shellcmd xsh_signaln(int nargs, char *args[])
{
    int numProcess = 1;
    if (nargs >= 2) {
         numProcess = atoi(args[1]);
    }
    
    signaln(midTermSem, numProcess);
    
    return 0;
}