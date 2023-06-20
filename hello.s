  .text
  .globl main
main:
  addi sp, sp, -12
  li t0, 1
  sw t0, 0(sp)
  li t0, 2
  sw t0, 0(sp)
  li t0, 3
  sw t0, 4(sp)
  li t0, 4
  sw t0, 4(sp)
  li t0, 5
  sw t0, 8(sp)
  li a0, 0
  addi sp, sp, 12
  ret
