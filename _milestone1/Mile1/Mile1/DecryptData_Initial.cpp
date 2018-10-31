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
			mov gdebug1, al				// al = starting_index = gPasswordHash[0] * 256 + gPasswordHash[1]

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
			xor dl, byte ptr[esi + eax]	// data[ebx] = data[ebx] ^ keyfile[starting_index]
			mov byte ptr[edi + ebx], dl	//

			add ebx, 1					// ebx++
			cmp ebx, ecx				// if(ebx > ecx) end loop
			ja lbl_EXIT_END				//
			jmp lbl_LOOP				// else loop

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

