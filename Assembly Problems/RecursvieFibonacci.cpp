#include <cstdio>

//Sa se scrie codul in limbaj de asamblare care calculeaza al n-lea numar Fibonacci recursiv.
//Pentru acest calcul va fi utlizata o functie; apelul acesteia, precum si returnarea rezultatului se va face in assembly

/*
	Aceasta rezolvare utilizeaza urmatoarea metoda:

	int fibo(int n)
	{
		if (n <= 1)
		{
			return n;
		}

		return fibo(n - 1) + fibo(n - 2);
	}
*/


int fibo(int n)
{
	_asm
	{
		mov eax, [ebp + 8]    //eax = [ebp + 8] = n
		cmp eax, 1            //compar eax cu 1
		jle _sfarsit          //daca eax <= 1, sar la sfarsit (se va returna valoarea din eax)

		mov eax, [ebp + 8]    //altfel, eax = [ebp + 8]; in continuare fac operatiile necesare pentru fibo(n - 1)
		dec eax               //eax = eax - 1
		push eax              //se pune eax (n - 1) pe stiva
		call fibo             //se apeleaza fibo
		add esp, 4            //se elibereaza stiva

		mov esi, eax          //esi = eax = fibo(n - 1)
		push esi              //salvam esi pe stiva

		mov eax, [ebp + 8]    //eax = [ebp + 8]; in continuare fac operatiile necesare pentru fibo(n - 2)
		sub eax, 2            //eax = eax - 2
		push eax              //se pune eax (n - 2) pe stiva
		call fibo             //se apeleaza fibo
		add esp, 4            //se elibereaza stiva

		mov edi, eax          //edi = eax = fibo(n - 2)

		pop esi               //luam esi de pe stiva
		add esi, edi          //esi = esi + edi = fibo(n - 1) + fibo(n - 2) 
		mov eax, esi          //eax = esi (valoarea pe care dorim sa o returnam trebuie pusa in eax sau edx (sau ambele)

		_sfarsit :
	}
}


int main()
{
	int number;
	int fiboRez;

	number = 15;

	_asm
	{
		mov edx, number     //edx = number
		push edx            //pun edx (number) pe stiva
		call fibo           //apelez functia
		add esp, 4          //eliberez stiva (am facut push la edx - 4 bytes, deci eliberez 4 bytes, adun 4 la esp)

		mov fiboRez, eax    //fiboRez = eax
	}

	printf("Fibonacci(%d) = %d", number, fiboRez);

	return 0;
}