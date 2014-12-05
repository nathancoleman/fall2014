.text
main:
ld $32, 1.65
ld $34, -0.57
ld $33, 0.0
ld $35, 0.80
ld $37, -0.92
ld $36, 0.0
ld $1, 0
ld $2, 99

loop:
fsub $40, $33, $34
fmul $38, $32, $40
fsub $40, $36, $37
fmul $39, $35, $40
ld $34, $33
ld $33, $39 
ld $37, $36 
ld $36, $39
sd $38
sd $39
add $1, $1, 1
bne $5, 0 loop