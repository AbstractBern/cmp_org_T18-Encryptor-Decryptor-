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
        // get parameters
        mov esi,data
        mov edx,dataLength
        dec edx
        
        // set up main stack frame
        push ebp
        mov ebp,esp

        // store parameters in frame
        push edx    // store dataLength
        push esi    // store *data
        mov edx,gNumRounds
        dec edx
        push edx    // store gNumRounds
        mov esi,gptrPasswordHash    
        push esi    // store *password hash
        mov esi,gptrKey 
        push esi    // store *key
        xor eax,eax
        push eax    // reserve space on stack for starting index
        push eax    // reserve space on stack for hop count

        // set up for main loop
        xor eax,eax // result storer
        xor ebx,ebx // index
        xor ecx,ecx // round

lbl_LOOP_ROUNDS:
        call calculateStartingIndex
        mov [ebp-24],eax    // store result in the space we allocated previously
        call calculateHopCount
        mov [ebp-28],eax    // store result in the space we allocated previously

        push ecx    // save round loop counter
        xor ecx,ecx // reset for data loop counter

lbl_LOOP_DATA:
        // data encryption
        call xorByte
        call incrementIndex
        call stepC
        call stepD
        call stepE
        call stepB
        call stepA

        // data loop logic
        cmp ecx,[ebp-4]
        je lbl_EXIT_LOOP_DATA
        inc ecx
        jmp lbl_LOOP_DATA

lbl_EXIT_LOOP_DATA:
        pop ecx
        // end data loop

        // round loop logic
        cmp ecx,[ebp-12]
        je lbl_EXIT_LOOP_ROUNDS
        inc ecx
        jmp lbl_LOOP_ROUNDS
        
//                      FUNCTIONS
calculateStartingIndex:
        // calc starting index
        mov esi,[ebp-16]                    // get passHash
        xor eax,eax
        mov al,byte ptr[esi+ecx*4]          // eax = gPasswordHash[0+round*4]
        sal eax,8                           // eax*256
        inc esi
        mov bl, byte ptr[esi + ecx * 4]     // ebx = gPasswordHash[1+round*4]
        add eax,ebx
        ret
        // end calculateStartingIndex

calculateHopCount:
        // calc hop count
        mov esi,[ebp-16]
        add esi,2
        xor eax,eax
        mov al,byte ptr[esi+ecx*4]
        sal eax,8
        inc esi
        mov bl,byte ptr[esi+ecx*4]
        add eax,ebx
        // fix hop count if necessary
        cmp eax,0xFFFF
        je lbl_FIX_HOP  // fix
        ret             // dont fix

    lbl_FIX_HOP:
        mov eax,0xFFFF
        ret
        // end calculateHopCount

xorByte:
        push ebx
        xor ebx,ebx
        mov ebx,[ebp-24]    // get index from stack frame
        mov esi,[ebp-8]     // get *data from stack frame
        mov edi,[ebp-20]    // get *gKey from stack frame
        mov al,byte ptr[edi+ebx]   // al = gKey[index]
        xor al,byte ptr[esi+ecx]   // al = al ^ data[x]
        mov byte ptr[esi+ecx],al   // update data buffer byte
        pop ebx
        ret

incrementIndex:
        mov eax,[ebp-24]    // eax = index
        add eax,[ebp-28]    // index += hopcount
        // fix index if necessary
        cmp eax,65537
        jae lbl_FIX_INDEX   // fix
        mov [ebp-24],eax    // set new index
        ret                 // dont fix

    lbl_FIX_INDEX:
        sub eax,65537
        mov [ebp-24],eax    // set new index
        ret

stepA:		
        push ebx
        xor ebx,ebx
        mov esi,[ebp-8]             // get *data
		mov al,byte ptr[esi+ecx]	// get data[x]		
        xor ebx,ebx
		mov bh,0xAA					// 0xAA has all even bits 1 and odd 0,								
									// bitwise and with this value will result in showing all even bits	
		and bh,al					// bh is now all the even bits of our byte							
		mov bl,0x55					// 0x55 has all even bits 0 and odd 1,								
									// bitwise and with this value will result in showing all odd bits		
		and bl,al					// bl is not all the odd bits of our byte							
		shr bh,1					// shift all even bits right 1 time	
		shl bl,1					// shift all odd bits left 1 time										
		or bl,bh					// combine them back together									
		mov al,bl                   // al is now out byte with even and odd bits swapped
        mov byte ptr[esi+ecx],al    // update data buffer byte
        pop ebx
        ret

stepB:
        push ebx
        xor ebx,ebx
		mov esi,[ebp-8]             // get *data
        mov al, byte ptr[esi+ecx]	// get data[x]	
		xor al,00111100b			// this inverts the middle 4 bits			
		mov byte ptr[esi+ecx],al    // update data buffer byte
	    pop ebx
        ret

stepC:
		mov esi,[ebp-8]             // get *data
        mov al,byte ptr[esi+ecx]	// get data[x]
        ror al,4                    // rotate 4 to right
        mov byte ptr[esi+ecx],al    // update data buffer byte
        ret

stepD:
        mov esi,[ebp-8]             // get *data
        xor eax,eax
        push ebx
        xor ebx,ebx
        mov al,byte ptr[esi+ecx]	// get data[x]
		lea edi, gEncodeTable		// put the address of the first byte of gEncodeTable into esi
		mov bl, byte ptr[edi+eax]	// copy the value at the index al from gEncodeTable (gEncodeTable[al])
        mov byte ptr[esi + ecx],bl  // update data buffer byte
        pop ebx
        ret

stepE: 
		push ebx					// save old ebx value
		push edx					// save old ecx value
        xor eax,eax
        mov esi,[ebp-8]             // get *data
        mov al,byte ptr[esi+ecx]	// get data[x]
		xor ebx,ebx					// set ebx to 0, will be our counter
		xor edx,edx					// set ecx to 0, will be the new reversed value
		jmp lbl_ELOOP				// start looping
			
	lbl_ELOOP:
			shl al,1					// shift the right most bit into the carry
			rcr dl,1					// rotate the carry into dl
			cmp ebx,7					// compare counter to 7
			je lbl_EEND					// if counter is 7 end the loop
			inc ebx						// else increment count
			jmp lbl_ELOOP				// and keep looping

	lbl_EEND:
			mov byte ptr[esi + ecx],dl  // update data buffer byte
			pop edx						// restore ecx
			pop ebx						// restore ebx
			ret							// return

//                      END FUNCTIONS

lbl_EXIT_LOOP_ROUNDS:
        // end round loop, end assembly
        add esp,28
        pop ebp
	}

	return resulti;
} // encryptData
