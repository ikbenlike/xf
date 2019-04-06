#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdbool.h>

#include "types.h"
#include "read.h"
#include "wordlist.h"
#include "compile.h"
#include "init_kernel.h"
#include "run.h"

//void execute(Metadata *md){
    /*while(true){
        Word **w = md->running;
        if(w == NULL || *w == NULL || (*w)->fn == &nothing){
            return;
        }
        (*w)->interpreter(md->running, md);
    }*/
//    for(Word **w = md->running++; (*w)->fn != &none; (*w)->interpreter(*w, md));
//}


/*
   (IP) -> W  fetch memory pointed by IP into "W" register
              ...W now holds address of the Code Field
   IP+2 -> IP advance IP, just like a program counter
              (assuming 2-byte addresses in the thread)
   (W) ->  X  fetch memory pointed by W into "X" register
              ...X now holds address of the machine code 
   JP (X)     jump to the address in the X register
*/

//or

/*
   (IP) -> W   fetch memory pointed by IP into "W" register
   IP+2 -> IP  advance IP (assuming 2-byte addresses)
   JP (W)      jump to the address in the W register
*/

/*void execute(Metadata *md){
    Word *w = *md->running++;
    w->interpreter(w, md);
}*/