#include <cstdio>

/*
Write an assembly function defined as follows:
void removePalindrom(unsigned int *arr, unsigned int &arrSize);

The function receives as parameters a vector of natural numbers, as well as the number of its elements (by reference).
The function will remove the palindrome numbers from the vector (of course, the number of elements will also have to be decreased).

!!!ATTENTION: the number of elements in the vector is given by reference, so we will not receive the actual number of elements as a parameter,
but the address where this value is located.

!!!Example of accessing/modifying the value of arrSize (your program may differ):
_asm
{
mov eax, [ebp + 12] //eax will have the memory location where the number of elements in the array is located
mov ebx, [eax] //ebx has the number of elements in the array
mov [eax], 5 //we modify arrSize and give it the value 5
}

A number is a palindrome if it is identical when read from right to left and left to right (e.g.: 1221, 1111, 0, 12321, 123321).

To check if a number is a palindrome, an auxiliary function can be used:
int estePalindrom(unsigned int numar);

Example:
- if the vector is [1221, 2453, 1, 567765], after calling the removePalindrom function, the vector will be [2453]
- if the vector is [4837, 382, 878, 987, 11], after calling the removePalindrom function, the vector will be [4837, 382, 987]

RECOMMENDATION: first solve the palindrome check problem and then the vector element removal problem.
*/

/*
Observations:

To solve the problem, an auxiliary function to check if a number is a palindrome was used. This function builds the inverted number in a register and compares it with the original one. If they are equal, the number is a palindrome, otherwise it is not.

*/


int isPalindrome(unsigned int number)
{
	_asm
	{
		mov ebx, [ebp + 8]				//ebx = number
		mov esi, 10                     //esi = 10; we will need it to do multiplications / divisions by 10
		mov ecx, 0                      //ecx = 0

		_build_palindrome_loop:
		cmp ebx, 0                      //compare ebx with 0
			je _compare_numbers             //if ebx == 0, we finished building the palindrome of the number, jump to comparison
			mov eax, ebx                   //eax = ebx
			mov edx, 0                     //edx = 0; prepare for a division
			div esi                        //eax = eax / esi; edx = eax % esi; in other words, eax = eax without the last digit, edx = the last digit of eax
			mov ebx, eax                   //ebx = eax
			mov edi, edx                   //edi = edx (last digit)
			mov eax, ecx                   //eax = ecx (the palindrome we're building)
			mul esi                        //eax = ecx * 10
			add eax, edi                   //eax = ecx * 10 + last_digit
			mov ecx, eax                   //ecx = eax
			jmp _build_palindrome_loop     //reloop the building process

			_compare_numbers :
		mov ebx, [ebp + 8]					  //ebx = numar
			cmp ecx, ebx                      //compare the palindrome with the original number
			je _is_palindrome                 //if they are equal, then it's a palindrome, return 1
			mov eax, 0                        //otherwise, eax = 0 (i.e., return 0)
			jmp _end                          //jump to end

			_is_palindrome :
		mov eax, 1							  //the number is a palindrome, eax = 1 (i.e., return 1)

			_end :
	}
}


void removePalindrom(unsigned int* arr, unsigned int& arrSize)
{
	_asm
	{
		mov esi, [ebp + 8]                  //esi = the address of the vector
		mov ebx, [ebp + 12]                 //ebx = the address where the number of elements is located
		mov ebx, [ebx]                      //ebx = the number of elements
		mov ecx, 0                          //ecx = 0 - we will use it as an index to access the elements from the vector

		_loop_traversal:
		cmp ecx, ebx							//compare the index of the current element with the number of elements in the vector
			jae _end                            //if ecx (index) >= ebx (vector size), we have finished traversing, jump to the end
			mov eax, [esi + ecx * 4]            //eax = the current element from the vector (arr[ecx])
			push ebx                            //save ebx on the stack; it is not guaranteed to have the same value after the function call
			push ecx                            //save ecx on the stack; it is not guaranteed to have the same value after the function call
			push esi                            //save esi on the stack; it is not guaranteed to have the same value after the function call
			push eax                            //put eax (arr[i]) on the stack (function parameter)
			call isPalindrome                   //call the function
			add esp, 4                          //free the stack
			pop esi                             //restore esi
			pop ecx                             //restore ecx
			pop ebx                             //restore ebx
			cmp eax, 1                          //compare eax (the result returned by the function) with 1
			je _prepare_delete_element          //if eax == 1, the number is a palindrome, jump to the instructions for deleting from the vector
			inc ecx                             //otherwise, increment the counter (to access the next element)
			jmp _loop_traversal                 //jump to the loop for traversing the elements of the vector

			_prepare_delete_element :
		mov edx, ecx							//edx = ecx = the index of the element to be deleted
			mov edi, ebx                        //edi = ebx = the number of elements in the vector
			dec edi                             //edi = ebx - 1 (arrSize - 1; we will go up to here when we want to delete using the presented method)
			_loop_delete :
		cmp edx, edi							//compare edx (current index) with edi (arrSize - 1)
			je _end_delete                      //if they are equal, jump to _end_delete, we have deleted the element
			inc edx                             //edx = edx + 1
			mov eax, [esi + edx * 4]            //eax = arr[edx + 1]
			dec edx                             //edx = edx - 1
			mov[esi + edx * 4], eax             //arr[edx] = eax = arr[edx + 1]
			inc edx                             //edx = edx + 1
			jmp _loop_delete                    //repeat the deletion loop

			_end_delete :
		dec ebx                                 //ebx = ebx - 1; an element has been eliminated from the vector, so its size decreases
			//(in this case we do not increment the counter ecx because, following the deletion of an element, the index ecx will be a new element that needs to be checked and we will skip it if we increment ecx)
			jmp _loop_traversal				    //jump to the loop that iterates over the elements 

			_end :
		mov eax, [ebp + 12]                //eax = the address of the number of elements in the vector
			mov[eax], ebx                      //update the number of elements in the vector
	}
}


int main()
{
	unsigned int testArr1[] = { 1221, 2453, 1, 567765 }, testArrSize1 = 4;
	unsigned int testArr2[] = { 4837, 382, 878, 987, 11 }, testArrSize2 = 5;
	unsigned int i;

	/*
		after calling the function, the console should display
		First array: 2345
		Second array: 4837 382 987
	*/

	removePalindrom(testArr1, testArrSize1);
	removePalindrom(testArr2, testArrSize2);

	printf("First array: ");
	for (i = 0; i < testArrSize1; i++)
	{
		printf("%d ", testArr1[i]);
	}
	printf("\n");

	printf("Second array: ");
	for (i = 0; i < testArrSize2; i++)
	{
		printf("%d ", testArr2[i]);
	}

	return 0;
}