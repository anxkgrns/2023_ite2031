        lw      0       1       two     load reg1 with 2 //sw test
        lw      0       2       zero    load reg2 with 0
        lw      0       3       one     load reg3 with 1
        sw      0       3       result  store 1 in save
        noop
done    halt
zero    .fill   0
one     .fill   1
two     .fill   2
result  .fill   0
