#include <iostream>
using namespace std;

/*
	Sa se scrie in limbaj de asamblare o functie definita astfel:

	bool computeRoots(Equation* eq, Result* res);

	unde eq este un pointer (adresa) catre o structura de tipul Equation, iar
	res este un pointer (adresa) catre o structura de tipul Result.

	Structura de tip Equation modeleaza o ecuatie de gradul II.
	Daca ecuatia nu are solutii intregi (delta < 0), functia returneaza 0.
	Daca ecuatia are solutii intregi, acestea vor fi calculate si puse in structura res, iar functia returneaza 1.
*/

//modeleaza ax^2 + bx + c = 0
struct Equation
{
	float a;
	float b;
	float c;
};

struct Result
{
	float r1;
	float r2;
};

bool computeRoots(Equation* eq, Result* res)
{
	_asm
	{
		mov eax, [ebp + 8];
		mov ebx, [ebp + 12];

		//calculez sqrt(delta) = sqrt(b^2 - 4 * a * c) in xmm0
		movss xmm0, dword ptr[eax + 4] //xmm0 = eq->b
		mulss xmm0, xmm0    //xmm0 = xmm0 * xmm0 = eq->b * eq->b (calculez b^2)
		mov ecx, 4          //eax = 4
		cvtsi2ss xmm1, ecx  //xmm1 = ecx = 4
		mulss xmm1, dword ptr[eax]     //xmm1 = xmm1 * eq->a (calculez 4 * a)
		mulss xmm1, dword ptr[eax + 8] //xmm1 = xmm1 * eq->c (calculez 4 * a * c)
		subss xmm0, xmm1               //xmm0 = xmm0 - xmm1 = b^2 - 4 * a * c = delta


		mov ecx, 0                      //ecx = 0
		cvtsi2ss xmm1, ecx              //xmm1 = ecx = 0
		comiss xmm0, xmm1               //compar xmm0 (delta) cu xmm1 (0)
		jb _nu_radacini_rationale       //daca e mai mic, inseamna ca nu exista radacini rationale

		//calculez sqrt(delta) in xmm0
		sqrtss xmm0, xmm0               //xmm0 = sqrt(xmm0) = sqrt(b^2 - 4 * a * c) = sqrt(delta)

		//calculez -b in xmm1
		mov ecx, -1                     //ecx = -1
		cvtsi2ss xmm1, ecx              //xmm1 = ecx = -1
		mulss xmm1, dword ptr[eax + 4] //xmm1 = xmm1 * eq->b (calculez -b)

		//calculez 2 * a in xmm2
		mov ecx, 2                      //ecx = 2
		cvtsi2ss xmm2, ecx              //xmm2 = ecx = 2
		mulss xmm2, dword ptr[eax]     //xmm2 = xmm2 * eq->a (calculez 2 * a)



	}
}

int main()
{
	Equation eq = { 1, -2, 1 };
	Result res;

	/*
	_asm
	{
	/// apelam din assembly
		lea eax, eq
		lea ebx, res
		push ebx
		push eax
		call computeRoots
		add esp, 8
	}
	*/

	if (computeRoots(&eq, &res))
	{
		cout << "Prima radacina este: " << res.r1 << '\n';
		cout << "A doua radacina este: " << res.r2;
	}
	else
	{
		cout << "Nu exista solutie intreaga!";
	}

	return 0;
}
