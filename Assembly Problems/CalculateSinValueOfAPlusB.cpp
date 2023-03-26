#include <iostream>

using namespace std;

/*
	Sa se scrie in limbaj de asamblare o functie definita astfel:

	float computeFormula(float a, float b);
	in cadrul functie se poate utiliza variabila aux pentru a transfera valori intre registrii SIMD si stiva FPU

	Functia calculeaza si returneaza valoarea sin(a + b) utilizand urmatoarea formula:
	
	sin(a + b) = sin(a) * cos(b) + cos(a) * sin(b)
*/

float computeFormula(float a, float b)
{
	float aux;
	
	_asm
	{
		fld dword ptr [ebp + 8]  //st(0) = a
		fsin                     //st(0) = sin(st(0)) = sin(a)
		fstp aux                 //aux = st(0) = sin(a)
		movss xmm0, aux          //xmm0 = aux = sin(a)

		fld dword ptr [ebp + 12] //st(0) = b
		fcos					 //st(0) = cos(st(0)) = cos(b)
		fstp aux                 //aux = st(0) = cos(b)
		mulss xmm0, aux          //xmm0 = xmm0 * aux = sin(a) * cos(b)

		fld dword ptr [ebp + 8]  //st(0) = a
		fcos                     //st(0) = cos(st(0)) = cos(a)
		fstp aux                 //aux = st(0) = cos(a)
		movss xmm1, aux          //xmm1 = aux = cos(a)

		fld dword ptr [ebp + 12] //st(0) = a
		fsin                     //st(0) = sin(st(0)) = sin(b)
		fstp aux                 //aux = st(0) = sin(b)
		mulss xmm1, aux          //xmm1 = xmm1 * aux = cos(a) * sin(b)

		addss xmm0, xmm1         //xmm0 = xmm0 + xmm1 = sin(a) * cos(b) + cos(a) * sin(b)

		movss aux, xmm0          //aux = xmm0

		fld aux                  //st(0) = aux = rezultat - returnarea unei valori in virgula mobila se face punand valoarea in st(0)
	}
}


int main()
{
	float a = 0.7853, b = 0.7853; //pentru aceste valori, functia ar trebui sa returneze 1

	cout << "sin(a + b) = " << computeFormula(a, b);

	return 0;
}