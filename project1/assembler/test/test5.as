        lw      0       1       one     //reg1 = 1 //swap test case
        lw      0       2       two     //reg2 = 2
        lw      2       3       zero    //temp = reg2
        lw      1       2       zero    //reg2 = reg1
        lw      3       1       zero    //reg1 = temp
        sw      0       1       swreg1  // swreg1 = reg1
        sw      0       2       swreg2  // swreg2 = reg2
        halt
zero    .fill   0
one     .fill   1
two     .fill   2
swreg1   .fill   0
swreg2   .fill   0
