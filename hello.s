  .data
  .globl x_1
x_1:
  .zero 4
  .globl y_1
y_1:
  .zero 4
  .globl sum_1
sum_1:
  .zero 4

  .text
  .globl sum
sum:
  addi sp, sp, -72
  li t0, 1
  sw t0, 68(sp)
  j b0
b0:
  lw t0, 68(sp)
  sw t0, 64(sp)
  sw a0, 60(sp)
  lw t0, 60(sp)
  sw t0, 56(sp)
  lw t0, 64(sp)
  lw t1, 56(sp)
  sgt t0, t0, t1
  seqz t0, t0
  sw t0, 52(sp)
  lw t0, 52(sp)
  bnez t0, b1
  j b2
b1:
  li t0, 1
  sw t0, 48(sp)
  j b3
b2:
  addi sp, sp, 72
  ret
b3:
  lw t0, 48(sp)
  sw t0, 44(sp)
  lw t0, 60(sp)
  sw t0, 40(sp)
  lw t0, 44(sp)
  lw t1, 40(sp)
  sgt t0, t0, t1
  seqz t0, t0
  sw t0, 36(sp)
  lw t0, 36(sp)
  bnez t0, b4
  j b5
b4:
  la t0, sum_1
  lw t0, 0(t0)
  sw t0, 32(sp)
  la t0, x_1
  lw t0, 0(t0)
  sw t0, 28(sp)
  lw t0, 32(sp)
  lw t1, 28(sp)
  add t0, t0, t1
  sw t0, 24(sp)
  la t0, y_1
  lw t0, 0(t0)
  sw t0, 20(sp)
  lw t0, 24(sp)
  lw t1, 20(sp)
  mul t0, t0, t1
  sw t0, 16(sp)
  lw t0, 16(sp)
  la t1, sum_1
  sw t0, 0(t1)
  lw t0, 48(sp)
  sw t0, 12(sp)
  lw t0, 12(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 8(sp)
  lw t0, 8(sp)
  sw t0, 48(sp)
  j b3
b5:
  lw t0, 68(sp)
  sw t0, 4(sp)
  lw t0, 4(sp)
  li t1, 1
  add t0, t0, t1
  sw t0, 0(sp)
  lw t0, 0(sp)
  sw t0, 68(sp)
  j b0
  .text
  .globl main
main:
  addi sp, sp, -4
  sw ra, 0(sp)
  li t0, 1
  la t1, x_1
  sw t0, 0(t1)
  li t0, 2
  la t1, y_1
  sw t0, 0(t1)
  li a0, 999
  call sum
  li a0, 0
  lw ra, 0(sp)
  addi sp, sp, 4
  ret
