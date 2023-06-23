  .data
  .globl a_1
a_1:
  .word 10

  .text
  .globl inc
inc:
  addi sp, sp, -12
  la t0, a_1
  lw t0, 0(t0)
  sw t0, 8(sp)
  lw t0, 8(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 4(sp)
  lw t0, 4(sp)
  la t1, a_1
  sw t0, 0(t1)
  la t0, a_1
  lw t0, 0(t0)
  sw t0, 0(sp)
  lw a0, 0(sp)
  addi sp, sp, 12
  ret
  .text
  .globl print_a
print_a:
  addi sp, sp, -8
  sw ra, 4(sp)
  la t0, a_1
  lw t0, 0(t0)
  sw t0, 0(sp)
  lw a0, 0(sp)
  call putint
  li a0, 10
  call putch
  lw ra, 4(sp)
  addi sp, sp, 8
  ret
  .text
  .globl main
main:
  addi sp, sp, -44
  sw ra, 40(sp)
  li t0, 0
  sw t0, 36(sp)
  j b0
b0:
  lw t0, 36(sp)
  sw t0, 32(sp)
  lw t0, 32(sp)
  li t1, 10
  slt t0, t0, t1
  sw t0, 28(sp)
  lw t0, 28(sp)
  bnez t0, b1
  j b2
b1:
  call inc
  sw a0, 24(sp)
  li t0, 1
  sw t0, 20(sp)
  lw t0, 20(sp)
  sw t0, 16(sp)
  lw t0, 16(sp)
  li t1, 2
  add t0, t0, t1
  sw t0, 12(sp)
  lw t0, 12(sp)
  sw t0, 20(sp)
  lw t0, 20(sp)
  sw t0, 8(sp)
  lw a0, 8(sp)
  call putint
  li a0, 10
  call putch
  call print_a
  lw t0, 36(sp)
  sw t0, 4(sp)
  lw t0, 4(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 0(sp)
  lw t0, 0(sp)
  sw t0, 36(sp)
  j b0
b2:
  li a0, 0
  lw ra, 40(sp)
  addi sp, sp, 44
  ret
