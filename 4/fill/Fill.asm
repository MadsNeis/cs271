// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.


(BEGIN)
    @SCREEN
    D=A
    @addr
    M=D

    @8192
    D=A
    @pixel  
    M=D

(INPUT)
    @pixel
    D=M
    @BEGIN
    D;JEQ

    @KBD
    D=M

    @BLACK_SCREEN
    D;JNE

    @WHITE_SCREEN
    D;JEQ

(BLACK_SCREEN)
    @addr
    A=M
    M=-1

    @addr
    M=M+1

    @pixel
    M=M-1

    @INPUT
    0;JMP

(WHITE_SCREEN)
    @addr
    A=M
    M=0

    @add
    M=M-1

    @pixel
    M=M-1

    @INPUT
    0;JMP



