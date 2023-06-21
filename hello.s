  .text
  .globl main
main:
  addi sp, sp, -72
  li t0, 0
  sw t0, 0(sp)
  li t0, 234
  sw t0, 4(sp)
  li t0, 100
  sw t0, 8(sp)
  li t0, 1
  sw t0, 12(sp)
  j b0
b0:
  lw t0, 12(sp)
  sw t0, 16(sp)
  lw t0, 16(sp)
  li t1, 10
  sgt t0, t0, t1
  seqz t0, t0
  sw t0, 20(sp)
  lw t0, 20(sp)
  bnez t0, b1
  j b2
b1:
  li t0, 1
  sw t0, 24(sp)
  j b3
b2:
  li a0, 0
  addi sp, sp, 72
  ret
b3:
  lw t0, 24(sp)
  sw t0, 28(sp)
  lw t0, 28(sp)
  li t1, 10
  sgt t0, t0, t1
  seqz t0, t0
  sw t0, 32(sp)
  lw t0, 32(sp)
  bnez t0, b4
  j b5
b4:
  lw t0, 0(sp)
  sw t0, 36(sp)
  lw t0, 36(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 40(sp)
  lw t0, 40(sp)
  sw t0, 0(sp)
  lw t0, 4(sp)
  sw t0, 44(sp)
  lw t0, 0(sp)
  sw t0, 48(sp)
  lw t0, 44(sp)
  lw t1, 48(sp)
  add t0, t0, t1
  sw t0, 52(sp)
  lw t0, 52(sp)
  sw t0, 4(sp)
  lw t0, 24(sp)
  sw t0, 56(sp)
  lw t0, 56(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 60(sp)
  lw t0, 60(sp)
  sw t0, 24(sp)
  j b3
b5:
  lw t0, 12(sp)
  sw t0, 64(sp)
  lw t0, 64(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 68(sp)
  lw t0, 68(sp)
  sw t0, 12(sp)
  j b0
