  .text
  .globl main
main:
  addi sp, sp, -60
  li t0, 100
  sw t0, 0(sp)
  lw t0, 0(sp)
  sw t0, 8(sp)
  lw t0, 8(sp)
  li t1, 3
  mul t0, t0, t1
  sw t0, 12(sp)
  lw t0, 0(sp)
  sw t0, 16(sp)
  lw t0, 16(sp)
  li t1, 9
  rem t0, t0, t1
  sw t0, 20(sp)
  lw t0, 12(sp)
  lw t1, 20(sp)
  add t0, t0, t1
  sw t0, 24(sp)
  lw t0, 24(sp)
  sw t0, 4(sp)
  lw t0, 0(sp)
  sw t0, 32(sp)
  lw t0, 4(sp)
  sw t0, 36(sp)
  lw t0, 32(sp)
  lw t1, 36(sp)
  add t0, t0, t1
  sw t0, 40(sp)
  lw t0, 4(sp)
  sw t0, 44(sp)
  lw t0, 40(sp)
  lw t1, 44(sp)
  mul t0, t0, t1
  sw t0, 48(sp)
  lw t0, 48(sp)
  sw t0, 28(sp)
  lw t0, 28(sp)
  sw t0, 52(sp)
  lw t0, 52(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 56(sp)
  lw a0, 56(sp)
  addi sp, sp, 60
  ret
