#include <iostream>
using namespace std;

/*
*	float sum;
*	float nr;
* 
	for (int i=0; i<nr; i++)
	{
		float aux = 1/v[i];
		sum += aux;
	}
	cout << nr / sum;
*/

float harmean(int* x, int nr)
{
	float aux;

	_asm
	{
		mov esi, [ebp + 8];  /// esi = adresa de inceput a vectorului x 
		mov ebx, [ebp + 12]; /// ebx = nr

		mov ecx, 0 /// contor

		_bucla: 
		cmp ecx, ebx /// cat timp ecx <= nr(ebx)
			je _sfarsit_bucla

			mov edx, [esi + ecx * 4] // edx = v[ecx], ecx contor
			cvtsi2ss xmm0, edx		/// (float) v[ecx]

			mov edi, 1 /// punem 1 pentru 1/v[ecx]
			cvtsi2ss xmm1, edi /// facem 1 float : xmm1=1 
			divss xmm1, xmm0 /// xmm1 = xmm1/xmm0 (calculez 1/v[ecx])

			addss xmm2, xmm1 /// sum+= 1/v[ecx]
			inc ecx /// ecx ++ 	

		jmp _bucla
		_sfarsit_bucla:

		/// calculam nr / sum 
		cvtsi2ss xmm3, ebx /// xmm3 = nr
		divss xmm3, xmm2 /// xmm3 = xmm3/xmm2 = nr / sum

		movss aux, xmm3 // nu putem combina instructiuni specifice FPU (fld) cu un registru din extensia SIMD
		fld aux;
	}
}

int main()
{
	int v[] = { 1,2,3,4,5 };
	cout << harmean(v, 5); /// se va afisa valoarea 2.18978
	return 0;
}
