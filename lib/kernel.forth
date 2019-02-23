: dup >r r@ r> ;
: rot >r swap r> swap ;
: over swap dup rot swap ;
: 2dup over over ;
: 2drop drop drop ;
: nip swap drop ;
: tuck swap over ;
: rdrop r> drop ;