.data
is_palin_msg: .asciiz "The string is a palindrome.\n"
not_palin_msg: .asciiz "The string is not a palindrome.\n"
string_space: .space 1024

.text
main:
la $4, string_space
li $5, 1024
li $2, 8
syscall

la $9, string_space
la $10, string_space

length_loop:
lb $11, ($10)
nop
beqz $11, end_length_loop
nop

addi $10, $10, 1
b length_loop
nop

end_length_loop:
subi $10, $10, 2

test_loop:
bge $9, $10, is_palin
nop

lb $11, ($9)
nop
lb $12, ($10)
nop
bne $11, $12, not_palin
nop

addi $9, $9, 1
subi $10, $10, 1
b test_loop
nop

is_palin:

la $4, is_palin_msg
li $2, 4
syscall
b exit
nop

not_palin:
la $4, not_palin_msg
li $2, 4
syscall
b exit
nop

exit:
li $2, 10
syscall