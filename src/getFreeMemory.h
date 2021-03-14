#ifndef _GETFREEMEMORY_H_
#define _GETFREEMEMORY_H_

extern int  __bss_end;
extern int  *__brkval;

int getFreeMemory()
{    
    int free_memory;
    if((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
}

#endif
