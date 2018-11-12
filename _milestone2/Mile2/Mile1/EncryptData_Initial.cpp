// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to encrypt the data as specified by the project assignment
int encryptData(char *data, int dataLength)
{
	int resulti = 0;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but should use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

	__asm {
mov esi, gptrPasswordHash	// put address of gPasswordHas into esi
			xor eax, eax				//
			mov al, byte ptr[esi]		// store gPassword[0] in al
			mov bl, 256					//
			mul bl						// multiply al by 256
			add al, byte ptr[esi + 1]	// add gPassword[1] to al, al is now starting index for keyfile
										// al = starting_index = gPasswordHash[0] * 256 + gPasswordHash[1]

			xor ebx, ebx				// ebx = control variable (loop)
			mov ecx, dataLength			// ecx = length
			cmp ecx, 0					// check that length is not <= 0
			sub ecx, 1					// ecx-- (file length is 1 less than what we had)
			jbe lbl_EXIT_ZERO_LENGTH	//

			mov edi, data				// put address of first byte of data in edi
			mov esi, gptrKey			// put address of gKey into esi

		//
		// LOOP THROUGH ENTIRE data[] BYTE BY BYTE
		//
lbl_LOOP :
			mov dl, byte ptr[edi + ebx]	//
			xor dl, byte ptr[esi + eax]	// edx = data[ebx] ^ keyfile[starting_index]
			// mov byte ptr[edi + ebx], dl	moved to end

			push edx					//	push edx so the functions can use it
			call stepC					//	C

			//call stepD					//	D

			//call stepE					//	E

			call stepB					//	B

			call stepA					//	A
			pop edx						// restore edx

			mov byte ptr[edi + ebx], dl	// data[ebx] = edx
			add ebx, 1					// ebx++
			cmp ebx, ecx				// if(ebx > ecx) end loop
			ja lbl_EXIT_END				//
			jmp lbl_LOOP				// else loop

stepA:									//									A
			push ebp					// Step A - Swap even/odd bits											|	Example:
			mov ebp,esp					// e.g.  0xA9 -> 0x56													|			0xA9	=	1010 1001
			push eax					// save old eax value													|			
			mov al,byte ptr[ebp+8]		// eax = edx, using 8 b/c we have pushed esp 4 times(edx,ret,ebp,eax)	|	0xAA	=	1010 1010			
			push ecx					// save old ecx value													|	0x55	=	0101 0101	
			mov ch,0xAA					// 0xAA has all even bits 1 and odd 0,									|	
										// bitwise and with this value will result in showing all even bits		|		1010 1001		1010 1001
			and ch,al					// ch is now all the even bits of our byte								|	&	1010 1010	&	0101 0101
			mov cl,0x55					// 0x55 has all even bits 0 and odd 1,									|	_____________	_____________
										// bitwise and with this value will result in showing all odd bits		|		1010 1000		0000 0001
			and cl,al					// cl is not all the odd bits of our byte								|	
			shr ch,1					// shift all even bits right 1 time										|	>>	1010 1000 	<<	0000 0001
			shl cl,1					// shift all odd bits left 1 time										|	_____________	_____________
			or cl,ch					// combine them back together											|		0101 0100		0000 0010
			mov al,cl                   // al is now out byte with even and odd bits swapped					|						
			mov byte ptr[ebp+8],al		// move result back into edx (stack location)							|		0101 0100
			pop ecx						// restore ecx															|	|	0000 0010	
			pop eax						// restore eax															|	_____________
			pop ebp						// restore base pointer													|		0101 0110	=	0x5
			ret							// return																|			 0xA9	->  0x56

stepB:									//									B
			push ebp					// Step B - Invert middle 4 bits			|	Example:
			mov ebp,esp					// e.g. 0x56 -> 0x6A						|			0x56	=	0101 0110
			push eax					// save old eax value						|	1 ^ 1 = 0		
			mov al,byte ptr[ebp+8]		// eax = edx, 8b/c (see stepA)				|   1 ^ 0 = 1		XOR'ing any binary value with a 0 will result in no change
			xor al,00111100b			// this inverts the middle 4 bits			|	0 ^ 0 = 0		XOR'ing any binary value with a 1 will invert the bit
			mov byte ptr[ebp+8],al		// move result into edx (on the stack)		|	0 ^ 1 = 1		So, using XOR'ing our byte with 0011 1100 will invert
			pop eax						// restore eax								|					the middle 4 bits
			pop ebp						// restore base pointer						|		0101 0110
			ret							// return									|	^	0011 1100
										//											|	_____________
										//											|		0110 1010	=	0x6A	;	0x56 -> 0x 6A

stepC:
			push ebp					// Step C - Swap nibbles			|	Example:
			mov ebp,esp					// e.g. 0x6A -> 0xA6				|			0x6A	=	0110 1010
			push eax					// save old eax value				|		0110 1010		a nibble is just half the byte, to swap we just rotate the byte 4 times
			mov al,byte ptr[ebp+8]		// copy pushed value (edx)			|	ror 4				*note: can be rotated either right or left
			ror al,4					// rotate 4 to the right			|	_____________
			mov byte ptr[ebp+8],al		// move result into edx (on stack)	|		1010 0110	=	0xA6
			pop eax						// restore eax						|
			pop ebp						// restore base pointer				|			0x6A	->	0xA6
			ret							// return							|			

stepD:
			push ebp		// Step D - Code Table Swap				
			mov ebp,esp		// e.g. 0xA6 -> CodeTable[0xA6]		
			lea esi, gEncodeTable[esi]
			pop ebp			//remove esi
			ret			//return 

			/*		Using Registers in a Function
			Every Time we use a register in a method we want to save the value before we change it
			This is done by pushing the register onto the new stack frame we set up

			So, if we want to use the eax register, we first push the value.
						push eax
			then, we can use the register to do whatever we want
						mov al,byte ptr[ebp+8]
			after we finish using the register we restore it from the stack frame
						pop eax

			Keep in mind that if you want to use multiple registers you will need to push all the values and pop them in the reverse order
						push eax
						push ebx
						push ecx
						pop ecx
						pop ebx
						pop eax
			*/

			/*		Using arrays in Assembly
			Lets say you want to use the gEncodeTable unsigned char array. It is a global variable, so we can use its name to reference it in VisualStudio
			We have to use the load effective address (lea) command because we want the address of the variable not the first value of the array.
						lea esi,gEncodeTable
			This puts the address of the first value in the array into the esi register. So effectively, the value at the address stored in esi is the first value in the array
						[esi] = gEncodeTable[0]
			So if we want to access a value further along in the array we can simply us relative addresses.
						[esi+1] = gEncodeTable[1]
						[esi+200]  = gEncodeTable[200]
			However, there is a slight problem we must correct for:
			Since arrays indexes start at 0 if we try to use [esi+256], we are trying the get gEncodeTable[256], which is out of bounds.
			To correct for this we need to subtract 1 from the address that we are trying to access. 
						[esi+0xFF] // ERROR: there is no 257th value, out of bounds.
						[esi+0xFE] // CORRECT: this is the 256th value in the array.
			*/

stepE:
			push ebp		// Step E - Reverse Bit Order
			mov ebp,esp		// e.g. 0xCA -> 0x53
			push edx
			mov al, byte ptr[ebp+8]	//not too sure on this step
			XOR bx, bx        // Set bx to zero
       			mov cx, 16
		RLOOP:
       			rcr ax, 1		//    AX,1
       			rcl bx, 1		//    BX,1
       			LOOP   RLOOP        // Do 16 times
			//BSR al, al//? reverse bit operator 
			// <-math here
			pop ebp			//
			pop edx
			ret				//


lbl_EXIT_ZERO_LENGTH :
			sub ebx, 1		// decrement ebx to -1 to return failure
			jmp lbl_EXIT	//

lbl_EXIT_END :
			xor ebx, ebx	// ebx = 0, correctly executed

lbl_EXIT :
			mov resulti, ebx
	}

	return resulti;
} // encryptData
