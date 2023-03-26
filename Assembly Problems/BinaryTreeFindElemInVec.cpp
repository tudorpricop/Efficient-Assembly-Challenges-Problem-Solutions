#include <iostream>
using namespace std;

struct Tree {
	int value;
	Tree* left, * right;
};

/*
int find(int v, Tree* root)
{
	if (root == NULL)
		return false;

	if (root->value == v)
		return true;

	bool res1 = find(v, root->left);

	if (res1 != 0)
		return true;

	bool res2 = find(v, root->right);

	return res2;
}*/

int find(int v, Tree* root)
{
	_asm
	{
		mov esi, [ebp + 8]		// esi = v
		mov ebx, [ebp + 12]		// ebx= root address

		cmp ebx, NULL			// if (root == NULL)
		je _false				// return false;

		mov ecx, [ebx + 4]		// ecx = root->left
		mov edx, [ebx + 8]		// edx = root->right

		mov ebx, [ebx + 0]		// ebx = root->value

		cmp ebx, esi			// if (root->value == v)
		je _true				// return true;

		// save values on stack
		push ebx
		push ecx
		push edx

		// bool res1 = find(v, root->left);
		push ecx
		push esi
		call find
		add esp, 8

		mov edi, eax			/// edi = res1

		// take values from stack
		pop edx
		pop ecx
		pop ebx

		cmp edi, 0				/// if (res1 != 0)
		jne _true				/// return true;

		// save values on stack
		push ebx
		push ecx
		push edx

		// bool res2 = find(v, root->right);
		push edx
		push esi
		call find
		add esp, 8

		// take values from stack
		pop edx
		pop ecx
		pop ebx

		// since the function returns in eax, we don't need to do anything but jump to the end
		jmp _final


		_false :
		mov eax, 0
			jmp _final

			_true :
		mov eax, 1

			_final :

	}
}

int main()
{
	Tree* r;
	r = new Tree;
	r->value = 7;
	r->left = new Tree;
	r->left->value = 4;
	r->left->left = new Tree;
	r->left->left->value = 3;
	r->left->left->left = r->left->left->right = NULL;
	r->left->right = NULL;
	r->right = new Tree;
	r->right->value = 2;
	r->right->left = r->right->right = NULL;

	int rez;
	int y = 4;
	rez = find(y, r);

	if (rez == 0)
		cout << "Node " << y << " is not in the tree!" << endl;
	else
		cout << "Node " << y << " is in the tree!" << endl;

	return 0;
}
