#include <iostream>
#include <cmath>

using namespace std;

/*
	Sa se scrie in limbaj de asamblare o functie definita astfel:

	int Distanta(Point *p1, Point *p2);

	unde p1 si p2 sunt pointeri catre doua variabile de tipul Point.
	Functia returneaza distanta dintre cele doua puncte.

	Precizari:

	*structura Point este declarata in felul urmator:

	struct Point
	{
		int x, y;
	}

	*va puteti folosi de o functie auxiliara de calcul a radicalului
	unsigned int radical(int x)
	{
		return sqrt(x);
	}

	*calculul distantei se poate face cu formula sqrt((x2 - x1)^2 + (y2-y1)^2))
*/


unsigned int radical(int x)
{
	return sqrt(x);
}


struct Point
{
	int x, y;
};


int Distanta(Point* p1, Point* p2)
{
	_asm
	{
		mov esi, [ebp + 8]          //esi = adresa primei structuri
		mov edi, [ebp + 12]         //edi = adresa celei de-a doua structuri
		mov ebx, [edi]              //ebx = p2->x
		sub ebx, [esi]              //ebx = ebx - p1->x = p2->x - p1->x
		mov eax, ebx                //eax = ebx = p2->x - p1->x (echivalent cu x2 - x1 din formula)
		imul eax                    //eax = eax * eax (echivalent cu (x2 - x1)^2 din formula) 
		mov ecx, eax                //ecx = eax ( (x2 - x1)^2 )
		mov ebx, [edi + 4]          //ebx = p2->y
		sub ebx, [esi + 4]          //ebx = p2->y - p1->y (echivalent cu y2 - y1 din formula)
		mov eax, ebx                //eax = ebx = p2->y - p1->y
		imul eax                    //eax = eax * eax (echivalent cu (y2 - y1)^2)
		add ecx, eax                //ecx = ecx + eax (echivalent cu (x2 - x1)^2 + (y2 - y1)^2)
		push ecx                    //punem ecx pe stiva (e parametru pentru functia radical)
		call radical                //apelam radical
		add esp, 4                  //eliberam stiva

		//din moment ce rezultatul radicalului va fi returnat oricum prin registrul eax, iar noi ar trebui sa
		//mutam rezultatul prin punerea acestei valori in eax, nu mai avem nimic de facut
	}
}


int main()
{
	Point p1, p2;
	int distanta;

	p1.x = 5;
	p1.y = 6;
	p2.x = -7;
	p2.y = 11;

	_asm
	{
		lea esi, p1
		lea edi, p2
		push edi
		push esi
		call Distanta
		add esp, 8
		mov distanta, eax
	}

	cout << "Distanta dintre cele doua puncte este " << distanta;

	return 0;
}