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

void execute(Metadata *md){
    while(true){
        Func *tmp = pop(RStack)(md->rstack);
        push(RStack)(md->rstack, tmp + 1);
        if(*tmp == &none){
            return;
        }
        (*tmp)(md);
    }
}
