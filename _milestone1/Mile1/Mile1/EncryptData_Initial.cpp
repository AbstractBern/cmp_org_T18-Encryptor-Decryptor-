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
        //make sure length isnt <=0
        xor ebx,ebx                 //set ebx to 0, using as return and counter
        mov ecx,dataLength          //set ecx to length
        cmp ecx,0                   //check that length is not <= 0
        jbe lbl_EXIT_ZERO_LENGTH
        
        //calculate starting index for keyfile starting_index = gPasswordHash[0] * 256 + gPasswordHash[1]
        mov esi,gptrPasswordHash    // put address of gPasswordHas into esi
        xor eax,eax
        mov al,byte ptr[esi]        // store gPassword[0] in al
        mov dl,256
        mul dl                      // multiply al by 256
        add al,byte ptr[esi+1]      // add gPassword[1] to al, al is now starting index for keyfile
        
        //set up the loop
        mov edi,data                // put address of first byte of data in edi
        mov esi,gptrKey             // put address of gKey into esi
        add esi,eax                  // set esi to gKey[al]; al is starting_index

        //start looping through data
lbl_LOOP:
        mov dl,byte ptr[edi + 4 * ebx] //dl is now value of data[ebx]
        xor dl,byte ptr[esi]        // xor tdl with the keyfile byte
        mov byte ptr[edi + 4 * ebx],dl //copy dl back into data[ebx]
        add ebx,1                   // increment counter ebx
        cmp ebx,ecx                 // check havent hit end of data
        ja lbl_EXIT_END                     
        jmp lbl_LOOP                // otherwise keep looping
        
lbl_EXIT_ZERO_LENGTH:
        sub ebx,1                   // decrement ebx to -1 to return failure
        jmp lbl_EXIT
lbl_EXIT_END:
        xor ebx,ebx                 // set ebx back to 0
lbl_EXIT:
        mov resulti,ebx             // set returni value (0 worked, -1 length invalid)
        ret                         // return
	}

	return resulti;
} // encryptData