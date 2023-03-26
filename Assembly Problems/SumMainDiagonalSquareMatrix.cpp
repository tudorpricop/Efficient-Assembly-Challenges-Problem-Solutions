#include <iostream>

using namespace std;

//Sa se determine suma elementelor pe de diagonala principala a unei matrice patratice.

/*
	for (int i=0; i<n; i++)
		sum+=m[i][i];
*/

int suma(int** mat, int dimensiune)
{
	_asm
	{
		// in cazul nostru ebx = i = j 

		mov esi, [ebp+8]	/// esi = mat
		mov edi, [ebp+12]	/// edi = dimensiune = i = j

		mov ebx, 0 /// ebx = 0 contor pt linii si coloane
		mov eax, 0

		_bucla:
			cmp ebx, edi  /// i=j < dimensiune
			je _final
			mov edx, [esi + 4 * ebx]	/// edx = m[i]
			mov edx, [edx + 4 * ebx]	/// edx = m[i][i]
			add eax, edx				// sum += m[i][i]
			inc ebx						// i++
			jmp _bucla
		_final:

	}
}


int main()
{
	int** mat;
	int i, j, k = 0, dimensiune = 4;

	mat = new int* [dimensiune];

	for (i = 0; i < dimensiune; i++)
		mat[i] = new int[dimensiune];

	for (i = 0; i < dimensiune; i++)
		for (j = 0; j < dimensiune; j++)
			mat[i][j] = ++k;

	int sum = 0;

	sum = suma(mat, dimensiune);

	printf("Suma elementelor de pe diagonala principala: %d", sum);
	//Ar trebui sa afiseze "Suma elementelor de pe diagonala principala este 15"

	return 0;
}