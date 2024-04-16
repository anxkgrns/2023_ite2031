        lw      0       2       one     // add test
        lw      0       3       two
        lw      0       4       three
        add     2       3       1
        add     1       4       1       reg1 is the result reg1 = 1 + 2 + 3
        halt
one     .fill   1
two     .fill   2
three   .fill   3
