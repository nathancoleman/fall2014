.data
d32: .float 1.65
d34: .float -0.57
d33: .float 0.0
d35: .float 0.80
d36: .float 0.0
d37: .float -0.92

.text
main:
ld $f0, d32
ld $f2, d34
ld $f1, d33
ld $f3, d35
ld $f5, d37
ld $f4, d36
li $1, 0
li $2, 99

loop:
nop
nop
fsub $f8, $f1, $f2
fmul $f6, $f0, $f8
fsub $f8, $f4, $f5
fmul $f7, $f3, $f8
ld $f2, $f1
ld $f1, $f7 
ld $f5, $f4 
ld $f4, $f7
sd $f6
sd $f7
add $1, $1, 1
bne $5, 0 loop
nop
nop
nop
nop