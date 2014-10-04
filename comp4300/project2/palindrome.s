.data
is_palin_msg: .asciiz "The string is a palindrome.\n"
not_palin_msg: .asciiz "The string is not a palindrome.\n"
string_space: .space 1024

.text
main:

la $30, string_space
li $31, 1024
li $29, 8
syscall

la $1, string_space
la $2, string_space

length_loop:
lb $3, ($2)
beqz $3, end_length_loop

addi $2, $2, 1
b length_loop

end_length_loop:
subi $2, $2, 2

test_loop:
bge $1, $2, is_palin

lb $3, ($1)
lb $4, ($2)
bne $3, $4, not_palin

addi $1, $1, 1
subi $2, $2, 1
b test_loop

is_palin:

la $30, is_palin_msg
li $29, 4
syscall
b exit

not_palin:
la $30, not_palin_msg
li $29, 4
syscall
b exit

exit:
li $29, 10
syscall