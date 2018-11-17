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

			xor eax,eax	//	result storer
			xor ebx,ebx //	starting index (only in scope of loop-rounds)
			xor ecx,ecx	//	"round" counter 0 -> gNumRounds
			
lbl_LOOP_ROUNDS:							//		BEGIN LOOP-ROUND

			push ecx						// parameter "round"
			call calculateStartingIndex		//		CALCULATE STARTING INDEX[ROUND]
			mov ebx,eax						// move result of call to ebx
			call calculateHopCount			//		CALCULATE HOP COUNT[ROUND]			note: uses same parameter as previous call
			mov edx,eax						// move result of call to edx
			pop ecx

			push ecx						// parameter "round"		[5th]
			push dataLength					// parameter dataLength		[4th]
			push data						// parameter data buffer	[3rd]
			push ebx						// index					[2nd]
			push edx						// hop count				[1st]
			call setupDataLoop				//		LOOP THROUGH ENTIRE DATA BUFFER BYTE by BYTE
			add esp,20

			cmp ecx,gNumRounds				//		LOOP-ROUND LOGIC
			je lbl_EXIT_ROUNDS				// end loop
			inc ecx							// rounds++
			jmp lbl_LOOP_ROUNDS				// loop again
			
calculateStartingIndex:						//		CALCULATE STARTING INDEX
			push ebp						//
			mov ebp,esp						//
			push ebx						//											note: not pushing eax here because we are returning result via eax
			push ecx						//
			push esi						//

			mov al,byte ptr[ebp+8]			// get parameter "rounds"
			mov esi,gptrPasswordHash		// copy passwordHash address
			mov bl,byte ptr[esi+eax*4]		// ebx = passwordHash[0 + "rounds" * 4]
			sal ebx,8						// ebx = ebx * 256
			inc esi							// increment passwordHash index
			mov cl,byte ptr[esi+eax*4]		// copy passwordHash byte into cl
			add bx,cx						// ebx + passwordHash[1 + "rounds" * 4]

			mov ax,bx						// copy result into ax to return
			pop esi							//
			pop ecx							//
			pop ebx							//
			pop ebp							//
			ret								//		END CALCULATE STARTING INDEX

calculateHopCount:							//		CALCULATE HOP COUNT
			push ebp						//
			mov ebp,esp						//
			push esi						//											note: not pushing eax here because we are returning result via eax
			push ebx						//
			push ecx						//

			xor eax,eax						//
			mov al,byte ptr[ebp+8]			// get parameter "rounds"
			mov esi,gptrPasswordHash		//
			add esi,2						//
			mov bl,byte ptr[esi+eax*4]		// ebx = passwordHash[2 + "rounds" * 4]
			sal ebx,8						// ebx = ebx * 256
			inc esi							//
			mov cl, byte ptr[esi + eax * 4]	// copy passwordHash byte into cl
			add bx,cx						// ebx = ebx + passwordHash[3 + "rounds" * 4]
			cmp ebx,0						//
			je fixHopCount					//
			jmp lbl_EXIT_HOP_COUNT			//

		fixHopCount:						//		FIX HOP COUNT
			mov bx,0xFFFF					// set "hop count" to 0

		lbl_EXIT_HOP_COUNT:					//		EXIT HOP COUNT
			mov ax,bx						// copy result into ex to return
			pop ecx							//
			pop ebx							//
			pop esi							//
			pop ebp							//
			ret								//		END CALCULATE HOP COUNT
			
setupDataLoop:								//		SETUP FOR DATA LOOP
			push ebp						//	stack frame quick reference:
			mov ebp,esp						//			ebp+8	=	hop count			eax	= returner
			push eax						//			ebp+12	=	index				ebx = general use
			push ebx						//			ebp+16	=	data buffer			ecx = counter
			push ecx						//			ebp+20	=	data length			edi = keyfile starting address
			push edi						//			ebp+24	=	round				esi = databuffer starting address
			push esi						//											edx = index
			push edx						//				

			xor edx,edx
			xor ecx,ecx						// counter for loop
			mov edx,[ebp+12]
			mov edi,gptrKey					// keyfile address
			mov esi,[ebp+16]				// data buffer address

lbl_DATA_LOOP:								//		LOOP THROUGH DATA BYTE BY BYTE
			
			call xorByte					// call xorByte
			mov byte ptr[esi+ecx],al		// store replace data byte

			add edx,[ebp+8]					// index += hop count

			cmp edx,65537					// if (index >= 65537)
			jae lbl_FIX_INDEX				// index -= 65537
			jmp lbl_DO_STEPS			    // nothing

		lbl_FIX_INDEX:						//		FIX THE INDEX
			sub edx,65537					//

		lbl_DO_STEPS:						//		DO 5 STEPS
			call stepC						//	C
			mov gdebug1,al
			call stepD						//	D
			mov gdebug1, al
			call stepE						//	E
			mov gdebug1,al
			call stepB						//	B
			mov gdebug1, al
			call stepA						//	A
			mov gdebug1,al
			mov byte ptr[esi+ecx],al		//

			cmp ecx,[ebp+20]				// compare counter with data length
			je lbl_EXIT_DATA_LOOP			// end loop if counter = data length
			inc ecx							// increment counter
			jmp lbl_DATA_LOOP				// else keep looping


lbl_EXIT_DATA_LOOP:							//		EXIT DATA LOOP
			pop edx
			pop esi
			pop edi
			pop ecx
			pop ebx
			pop eax
			pop ebp
			ret								//		END DATA LOOP
			
xorByte:
			push ebx
			push edx
			mov bl,byte ptr[esi+ecx]
			mov edx,[ebp+12]
			xor bl,byte ptr[edi+edx]
			mov al,bl
			pop edx
			pop ebx
			ret

stepA:									//									A																			
			push ecx					// save old ecx value															
			mov ch,0xAA					// 0xAA has all even bits 1 and odd 0,										
										// bitwise and with this value will result in showing all even bits		
			and ch,al					// ch is now all the even bits of our byte								
			mov cl,0x55					// 0x55 has all even bits 0 and odd 1,									
										// bitwise and with this value will result in showing all odd bits	
			and cl,al					// cl is not all the odd bits of our byte									
			shr ch,1					// shift all even bits right 1 time									
			shl cl,1					// shift all odd bits left 1 time								
			or cl,ch					// combine them back together									
			mov al,cl                   // al is now our byte with even and odd bits swapped											
			pop ecx						// restore ecx																															
			ret							// return															

stepB:									//									B										
			xor al,00111100b			// this inverts the middle 4 bits										
			ret							// return									

stepC:									//									C			
			ror al,4					// rotate 4 to the right							
			ret							// return									

stepD:
			push esi					// save old esi value
			push ebx					// save old ebx value
			push ecx					// save old ecx
			xor ecx,ecx					// set ecx to 0
			xor ebx,ebx					// set ebx to 0
			mov cl,al
			lea esi, gEncodeTable		// put the address of the first byte of gEncodeTable into esi
			mov bl, byte ptr[esi+ecx]	// copy the value at the index al from gEncodeTable (gEncodeTable[al])
			mov al,bl					// al is now the table swapped byte
			pop ecx						// resetore ecx
			pop ebx						// restore ebx
			pop esi						// restore esi
			ret							// return

stepE:
			push ebx					// save old ebx value
			push ecx					// save old ecx value
			xor ebx,ebx					// set ebx to 0, will be our counter
			xor ecx,ecx					// set ecx to 0, will be the new reversed value
			jmp lbl_ELOOP				// start looping
			
	lbl_ELOOP:
			shl al,1					// shift the right most bit into the carry
			rcr cl,1					// rotate the carry into cl
			cmp ebx,7					// compare counter to 7
			je lbl_EEND					// if counter is 7 end the loop
			inc ebx						// else increment count
			jmp lbl_ELOOP				// and keep looping

	lbl_EEND:
			mov al,cl		// move result into parameter
			pop ecx						// restore ecx
			pop ebx						// restore ebx
			ret							// return

lbl_EXIT_ROUNDS:	

	}

	return resulti;
} // encryptData
