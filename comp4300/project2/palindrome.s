.data
is_palin_msg: .asciiz "The string is a palindrome.\n"
not_palin_msg: .asciiz "The string is not a palindrome.\n"
string_space: .space 1024

.text
main:

la $10, string_space
li $11, 1024
li $20, 8
syscall

la $01, string_space
la $02, string_space

length_loop:
lb $03, ($02)
beqz $03, end_length_loop

addi $2, $2, 1
b length_loop

end_length_loop:
subi $2, $2, 2

test_loop:
bge $01, $2, is_palin

lb $3, ($1)
lb $4, ($2)
bne $3, $4, not_palin

addi $1, $1, 1
subi $2, $2, 1
b test_loop

is_palin:

la $10, is_palin_msg
li $20, 4
syscall
b exit

not_palin:
la $10, not_palin_msg
li $20, 4
syscall
b exit

exit:
li $20, 10
syscall