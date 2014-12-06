.data
d32: .float 1.65
d34: .float -0.57
df17: .float 0.0
df19: .float 0.80
df20: .float 0.0
df21: .float -0.92

.text
main:
ld $f16, d32
ld $f18, d34
ld $f17, d17
ld $f19, d19
ld $f21, d21
ld $f20, d36
li $1, 0
li $2, 99

loop:
nop
nop
fsub $f24, $f17, $f18
fmul $f22, $f16, $f24
fsub $f24, $f20, $f21
fmul $f23, $f19, $f24
ld $f18, $f17
ld $f17, $f23 
ld $f21, $f20 
ld $f20, $f23
sd $f22
sd $f23
add $1, $1, 1
bne $5, 0 loop
nop
nop
nop
nop