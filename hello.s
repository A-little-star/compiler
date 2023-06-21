  .text
  .globl main
main:
  addi sp, sp, -48
  li t0, 1
  sw t0, 0(sp)
  lw t0, 0(sp)
  sw t0, 4(sp)
  lw t0, 4(sp)
  li t1, 2
  sgt t0, t0, t1
  sw t0, 8(sp)
  lw t0, 8(sp)
  bnez t0, b0
  j b4
b0:
  lw t0, 0(sp)
  sw t0, 12(sp)
  lw t0, 12(sp)
  li t1, 3
  sgt t0, t0, t1
  sw t0, 16(sp)
  lw t0, 16(sp)
  bnez t0, b1
  j b2
b1:
  lw t0, 0(sp)
  sw t0, 20(sp)
  lw a0, 20(sp)
  addi sp, sp, 48
  ret
b2:
  lw t0, 0(sp)
  sw t0, 24(sp)
  lw t0, 24(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 28(sp)
  lw a0, 28(sp)
  addi sp, sp, 48
  ret
b3:
  j b5
b4:
  lw t0, 0(sp)
  sw t0, 32(sp)
  lw t0, 32(sp)
  li t1, 2
  add t0, t0, t1
  sw t0, 36(sp)
  lw a0, 36(sp)
  addi sp, sp, 48
  ret
b5:
  lw t0, 0(sp)
  sw t0, 40(sp)
  lw t0, 40(sp)
  li t1, 3
  add t0, t0, t1
  sw t0, 44(sp)
  lw a0, 44(sp)
  addi sp, sp, 48
  ret
