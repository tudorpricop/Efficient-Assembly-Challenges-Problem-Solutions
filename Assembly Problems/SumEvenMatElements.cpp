#include <iostream>

using namespace std;

//Sa se determine suma elementelor PARE unei matrici.

float suma(int **v, int nr_linii, int nr_coloane)
{
	_asm
	{
		mov eax, 0
		cvtsi2ss xmm1, eax

		mov esi, [ebp+8]	// esi = v
		mov edi, [ebp+12]	// edi = nr_linii 
		mov edx, [ebp+16]	// edx = nr_coloane

		mov ebx, 0 /// ebx = i
		

		_bucla_linii:
			//cmp ebx, edi				/// i<nr_linii
			 cmp ebx, [ebp+12]			// echivalent mai sus
			je _final
			mov ecx, 0					/// ecx = j

			_bucla_coloane:	
				//cmp ecx, edx			/// j<nr_coloane
				cmp ecx, [ebp +16]  // echivalent linie de mai sus
				je _sfarsit_bucla_coloane

				mov eax, [esi + ebx * 4]             //eax = adresa liniei ebx
				mov eax, [eax + ecx * 4]             //eax = elementul de pe linia ebx si coloana ecx 

	
				test eax, 1     //instructiunea test face AND intre eax si 1, dar nu modifica eax, doar seteaza flagurile
				jnz _creste_contor    //daca flag-ul zero este setat (adica operatia AND intre eax si 1 nu a avut rezultatul 0), sari
				

				cvtsi2ss xmm0, eax
				addss xmm1, xmm0
				

				_creste_contor:
				inc ecx					/// j++
				jmp _bucla_coloane
		
			_sfarsit_bucla_coloane:
			inc ebx						/// i++

		jmp _bucla_linii
		
		_final:
			sub esp, 4 // aloc 4 octeti pe stiva programului 
			movss [esp], xmm1
			fld [esp]
			add esp, 4

		/*
		movss aux, xmm1
		fld aux
		*/
	}
}


int main()
{

	int** v;
	int k = 0;
	int nr_linii = 3;
	int nr_coloane = 2;

	v = new int* [nr_linii];

	for (int i = 0; i < nr_linii; i++)
		v[i] = new int[nr_coloane];

	for (int i = 0; i < nr_linii; i++)
		for (int j = 0; j < nr_coloane; j++)
			v[i][j] = ++k;
	for (int i = 0; i < nr_linii; i++)
	{

		for (int j = 0; j < nr_coloane; j++)
			cout << v[i][j] << " ";
		cout << endl;
	}



	float suma_elemente;
	

	//suma_elemente = suma(v, nr_linii, nr_coloane);

	
	_asm
	{
		///lea esi, v este gresit
		mov esi, v
		mov ecx, nr_linii
		mov edx, nr_coloane

		push edx 
		push ecx
		push esi

		call suma

		add esp, 12
		fstp suma_elemente
	}

	printf("Suma elementelor din matrice este %f", suma_elemente);
	//Ar trebui sa afiseze "Suma elementelor din matrice este 20"

	return 0;
}