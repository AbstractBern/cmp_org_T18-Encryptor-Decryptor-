// DecryptData.cpp
//
// THis file uses the input data and key information to decrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to decrypt the data as specified by the project assignment
int decryptData(char *data, int dataLength)
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
			call stepA					//	C

			call stepB					//	D

			call stepE					//	E

			call stepD					//	B

			call stepC					//	A
			pop edx						// restore edx

			mov byte ptr[edi + ebx], dl	// data[ebx] = edx
			add ebx, 1					// ebx++
			cmp ebx, ecx				// if(ebx > ecx) end loop
			ja lbl_EXIT_END				//
			jmp lbl_LOOP				// else loop

stepA:									//									A
			push ebp					// Step A - Swap even/odd bits											|	Example:// EncryptData.cpp
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

stepB:
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
			mov ebp,esp		//move stack pointer into base pointer e.g. 0xA6 -> CodeTable[0xA6]
			push eax		//push eax register for use
			mov al, byte ptr[ebp+8]	//move data byte located at [ebp+8] into al
			lea esi, gEncodeTable[al]	//swaap al with al byte located and passed in by gEncodeTable[al]
			mov al, esi			//mov result of swapped bytes back into al
			mov byte ptr[ebp+8], esi	//move swapped byte back into [ebp+8] on the stack
			pop eax				//pop eax
			pop ebp			//pop ebp
			ret			//return
stepE:
			push ebp		// Step E - Reverse Bit Order
			mov ebp,esp		// move stack pointer to base pointer e.g. 0xCA -> 0x53
			push eax		//push register onto stack
			push edx		//push edx register for use
			push ecx		//push ecx register to use
			mov ecx, 8		//move the number 8 to the counter ecx register
			mov al, byte ptr[ebp+8]	//move byte of data located at [ebp+8] into al 
			cmp ecx, 0		//compare if ecx (eight) to 0
			jg REPEAT		//jump to header if ecx is greater than 0
			mov al, dl		//after jump is done, move data in dl back into al
			move byte ptr[ebp+8], al	//move al to the byte locate at [ebp+8]
			pop ecx			//push (ecx) registers in opposite order
			pop edx			//pop edx
			pop eax			//pop eax
			pop ebp			//pop ebp
			ret			//return 
		REPEAT:					//REPEAT LOOP should loop 8 times
			shr al, 1			//shifts right the data in al and is given a carryflag
			rcl dl, 1			//carry flag is carried left of dl by one
			dec ecx, 1			//ecx is decremented by one 

lbl_EXIT_ZERO_LENGTH :
			sub ebx, 1		// decrement ebx to -1 to return failure
			jmp lbl_EXIT	//

lbl_EXIT_END :
			xor ebx, ebx	// ebx = 0, correctly executed

lbl_EXIT :
			mov resulti, ebx
	}

	return resulti;
} // decryptData
			push eax					// save old eax value													|			
