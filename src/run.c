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
        puts("execute");
        Word **w = pop(RStack)(md->rstack);
        printf("w = %"PRId64"\n", (Cell)w);
        push(RStack)(md->rstack, w + 1);
        if(w == (Word**)&none){
            return;
        }
        (*w)->interpreter(*w, md);
    }
}
