  .text
  .globl half
half:
  addi sp, sp, -12
  sw a0, 12(sp)
  lw t0, 12(sp)
  sw t0, 4(sp)
  lw t0, 4(sp)
  li t0, 2
  div t0, t0, t1
  sw t0, 0(sp)
  lw a0, 0(sp)
  addi sp, sp, 12
  ret
  .text
  .globl f
f:
  ret
  .text
  .globl main
main:
  addi sp, sp, -8
  sw ra, 4(sp)
  call f
  li a0, 10
  call half
  sw a0, 0(sp)
  lw a0, 0(sp)
  lw ra, 4(sp)
  addi sp, sp, 8
  ret
