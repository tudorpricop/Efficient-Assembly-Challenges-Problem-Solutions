#include <iostream>

using namespace std;

struct Circle {
	short int x, y;
	int radius;
};

float area(Circle* x, int nr)
{
	float aux = 0;

	_asm {
		mov ebx, 0
		cvtsi2ss xmm0, ebx  /// xmm0 = 0 initializam cea mai mare arie

		mov ebx, [ebp+8]	/// ebx = adresa vectorului x
		mov esi, 0			/// esi = 0 = contor

		_bucla:
			cmp esi, [ebp + 12]	/// esi < nr
			jge _final

			mov eax, [ebx + esi * 8 + 4]	/// eax = x[contor]
			///mov eax, [eax + 4]				/// eax = radius (aici e +8 sau +4) ?
			mul eax							/// eax = eax * eax = r^2

			fldpi							/// ST(0) = pi
			fstp aux						/// aux = pi | stiva e goala
			cvtsi2ss xmm1, eax				/// eax devine float
			movss xmm2, aux					/// xmm2 = aux
			mulss xmm2, xmm1				/// xmm2 = xmm2* xmm1 =
											///	aux * eax = pi * r^2

			comiss xmm2, xmm0				/// comparam valoarea curenta cu cea maxima
			jbe _skip
			movss xmm0, xmm2				/// max  = xmm0
			_skip:
			inc esi							/// esi++

		jmp _bucla

		
		_final:
		movss aux, xmm0						//aux = xmm0
		fld aux								//st(0) = aux = rezultat - returnarea unei valori in virgula mobila se face punand valoarea in st(0)

	}
}

int main()
{
	Circle c[] = { {1,4,8},{2,3,7},{4,2,12} };
	cout << area(c, 3); // se va afisa valoarea 452.389
	return 0;
}
