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
		//
		//	Calculate starting index for keyfile starting_index = gPasswordHash[0] * 256 + gPasswordHash[1]
		//
		mov esi, gptrPasswordHash	// put address of gPasswordHas into esi
		xor eax, eax				//
		mov al, byte ptr[esi]		// store gPassword[0] in al
		mov bl, 256					//
		mul bl						// multiply al by 256
		add al, byte ptr[esi + 1]	// add gPassword[1] to al, al is now starting index for keyfile
		mov gdebug1,al				// al is now our value to xor with the data
		

		//
        //	Ensure length isnt <=0
        //
		xor ebx,ebx					// ebx will be loop control
        mov ecx,dataLength			// ecx will be length
        cmp ecx,0					// check that length is not <= 0
		sub ecx,1					// decrement ecx since non-inclusive length
        jbe lbl_EXIT_ZERO_LENGTH
        
        //
		//	Set up Loop
		//
        mov edi,data				// put address of first byte of data in edi
        mov esi,gptrKey				// put address of gKey into esi

        //
		// LOOP THROUGH ENTIRE DATA[] BYTE BY BYTE
		//
lbl_LOOP:
        mov dl,byte ptr[edi + ebx]	// dl is now value of data[ebx]
        xor dl,byte ptr[esi+eax]	// xor dl with the keyfile byte
        mov byte ptr[edi + ebx],dl	// copy dl back into data[ebx]
		
		//
		//	5 STEPS
		//
		push edx		//	
		call stepC		//	C
		add esp,4		//   
		// D
		push edx		//	
		call stepD		//	D
		add esp,4		//	
		// E
		push edx		//
		call stepE		//	E
		add esp,4		//
		// B
		push edx		//
		call stepB		//	B
		add esp,4		//
		// A
		push edx		//
		call stepA		//	A
		add esp, 4		//

		//
		//	LOOP LOGIC
		//
        add ebx,1					// ebx++
        cmp ebx,ecx					// if(ebx > ecx) end loop
        ja lbl_EXIT_END				//
        jmp lbl_LOOP				// else loop
        

stepA:
		push ebp					//
		mov ebp,esp					//set up stack frame

		pop ebp						//restore stack frame
		ret

stepB:
		push ebp					//
		mov ebp,esp					//set up stack frame

		pop ebp						//restore stack frame
		ret

stepC:
		push ebp					//
		mov ebp,esp					//set up stack frame

		pop ebp						//restore stack frame
		ret

stepD:
		push ebp					//
		mov ebp,esp					//set up stack frame

		pop ebp						//restore stack frame
		ret

stepD :
		push ebp					//
		mov ebp,esp					//set up stack frame

		pop ebp						//restore stack frame
		ret

lbl_EXIT_ZERO_LENGTH:
        sub ebx,1					// decrement ebx to -1 to return failure
        jmp lbl_EXIT

lbl_EXIT_END:	//
lbl_EXIT:		//	Currently does nothing but supports returning something via resilti
	
	}

	return resulti;
} // encryptData