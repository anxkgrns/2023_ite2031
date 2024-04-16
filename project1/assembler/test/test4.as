        lw      0       2       one     //nor test
        lw      0       3       two
        nor     2       3       5       // reg5 = nor(reg2,reg3)
        sw      0       5       result  // result = reg5(-4)
        halt
one     .fill   1
two     .fill   2
result  .fill   0
