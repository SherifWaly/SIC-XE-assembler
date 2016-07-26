
. word
   ALPH   word    1,2, 3,
  BETA       word    1, -2, 3
BETA    word    -1,-2,-3
GAMMA   word
LALA    +word   1
HAHA     word    -1231
.

. bytes
1BB     byte
BB2     byte    1,-2,12
BB3     byte    C'a sd sd'
BB_4     byte   c'asa,asdas'
bB5    BYTE     X'FF as'
bB5    BYTE     X'Ffas'
bB5    BYTE     X'FFfd'
bB5    BYTE     X'Fa0'
bb6    BytE     X'012,4'
bB5    BYTE     x'F1a0',C'asa as'
bB5    BYTE     X'F1a0',x
bB5    BYTE     w'Fa0'
bB5    BYTE     c'
bB5    BYTE     'csdas'
bB5    BYTE     c"csdas"
bB5    BYTE     c'csdas

. resw
ww1    resw     1,2,3
ww2    RESW     -1
wW3    RESw     FA0
ww4    resw     x'
ww5    resw      c'asdas ''
ww6    resw     1001

. resb
ww1    resw     1,2,3
ww2    RESW     -1
wW3    RESw     FA0
ww4    resw     x'
ww5    resw      c'asdas ''
