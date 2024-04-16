        lw      0       1       result  result
        lw      0       2       mcand   x
        lw      0       3       mplier  y
        lw      0       4       one     1
        lw      0       5       mtwo    num
        lw      0       6       bitnum  15
loop    nor     3       5       7       j = nor(mplier, num)
        beq     0       7       sum     if(j==0) goto notsum, else sum
        beq     0       0       notsum  goto notsum
sum     add     1       2       1       result = result + mcand
notsum  add     2       2       2       mcand = mcand << 1
        add     5       5       5       num = num << 1
        add     5       4       5       num = num + 1
        lw      0       7       none    -1
        add     6       7       6       bitnum = bitnum - 1
        beq     0       6       done    if(bitnum==0) goto done, else loop
        beq     0       0       loop    goto loop
done    halt
mtwo    .fill   -2
none    .fill   -1
one     .fill   1
result  .fill   0
bitnum  .fill   15
mcand   .fill   32766
mplier  .fill   12328
