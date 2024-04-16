        lw      0       1       two     load reg1 with 2 //jalr test
        lw      0       2       zero    load reg2 with 0
        lw      0       3       one     load reg3 with 1
        lw      0       4       adaft   load reg4 with after
        lw      0       5       adbef   load reg5 with before
        beq     1       2       done    if reg3 == 0, goto done
        jalr    5       7               goto before
        jalr    4       7               goto after
        beq     1       2       done    if reg3 == 0, goto done
before  add     2       3       2       reg2 = reg2 + reg3
        jalr    7       5               goto before
after   add     2       3       2       reg2 = reg2 + reg3
        jalr    7       4               goto after
        noop
done    halt
zero    .fill   0
one     .fill   1
two     .fill   2
adaft   .fill   after
adbef   .fill   before
