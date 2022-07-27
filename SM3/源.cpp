#include<iostream>

using namespace std;
struct SM3_B
{
	unsigned int B[16] = { 0 };
	SM3_B* next = nullptr;
};
struct SM3_V
{
	unsigned int V[8] = { 0x7380166F,
	0x4914B2B9,
	0x172442D7,
	0xDA8A0600,
	0xA96F30BC,
	0x163138AA,
	0xE38DEE4D,
	0xB0FB0E4E };
};
struct SM3_W
{
	unsigned int W[68] = { 0 };
};
struct SM3_W_P
{
	unsigned int W_P[64] = { 0 };
};

void cycle_shift_left(unsigned int target, unsigned int step, unsigned int& re);

unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, unsigned int j);

unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, unsigned int j);

SM3_V main_cycle(SM3_B* CTX);

void CF(SM3_B& CTX, SM3_V& IV);

void exten(SM3_B& CTX, SM3_W& W, SM3_W_P& W_P);

unsigned int P0(unsigned int X);

unsigned int P1(unsigned int X);

unsigned int T(int i);

SM3_B* get_string_test(string str);




//循环左移函数
void cycle_shift_left(unsigned int target, unsigned int step, unsigned int& re)
{
	unsigned int head = target >> (32 - step);
	re = (target << step) | head;
}


//FF函数
unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, unsigned int j)
{

	if (j <= 15)
	{
		return X ^ Y ^ Z;
	}
	else
	{
		return (X & Y) | (X & Z) | (Y & Z);
	}
}
//GG函数
unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, unsigned int j)
{

	if (j <= 15)
	{
		return X ^ Y ^ Z;
	}
	else
	{
		return (X & Y) | (~X & Z);
	}
}

//迭代函数
SM3_V main_cycle(SM3_B* CTX)
{
	SM3_V IV;
	SM3_B* temp = CTX;
	while (temp != nullptr)
	{
		CF(*temp, IV);
		temp = temp->next;
	}
	cout << "杂凑值:";
	for (int i = 0; i < 8; i++)
	{
		cout << hex << IV.V[i] << "  ";
	}
	cout << endl;

	return IV;
}


//压缩函数
void CF(SM3_B& CTX, SM3_V& IV)
{
	SM3_W W;
	SM3_W_P W_P;
	exten(CTX, W, W_P);
	unsigned int Temp[8];

	/*
	A:0
	B:1
	C:2
	D:3
	E:4
	F:5
	G:6
	H:7

	*/
	for (int i = 0; i < 8; i++)
	{
		Temp[i] = IV.V[i];
	}

	for (int i = 0; i < 64; i++)
	{



		unsigned int SS1 = 0, SS2 = 0;
		unsigned int A12 = 0;
		unsigned int TI = 0;
		unsigned int T_back = 0;
		unsigned int su = 0;
		unsigned int TT1 = 0, TT2 = 0;
		unsigned int FF_back = 0;
		unsigned int GG_back = 0;

		cycle_shift_left(Temp[0], 12, A12);

		T_back = T(i);
		cycle_shift_left(T_back, i % 32, TI);

		su = (A12 + Temp[4] + TI) & 0xffffffff;
		cycle_shift_left(su, 7, SS1);

		SS2 = SS1 ^ A12;


		FF_back = FF(Temp[0], Temp[1], Temp[2], i);

		TT1 = (FF_back + Temp[3] + SS2 + W_P.W_P[i]) & 0xffffffff;

		GG_back = GG(Temp[4], Temp[5], Temp[6], i);
		TT2 = (GG_back + Temp[7] + SS1 + W.W[i]) & 0xffffffff;
		Temp[3] = Temp[2];
		cycle_shift_left(Temp[1], 9, Temp[2]);
		Temp[1] = Temp[0];
		Temp[0] = TT1;
		Temp[7] = Temp[6];
		cycle_shift_left(Temp[5], 19, Temp[6]);
		Temp[5] = Temp[4];

		Temp[4] = P0(TT2);

	}
	for (int i = 0; i < 8; i++)
	{
		IV.V[i] = Temp[i] ^ IV.V[i];
	}
}


//消息扩展
void exten(SM3_B& CTX, SM3_W& W, SM3_W_P& W_P)
{
	for (int i = 0; i < 16; i++)
	{
		W.W[i] = CTX.B[i];
	}
	for (int i = 16; i < 68; i++)
	{
		unsigned int W15 = 0, W7 = 0;
		cycle_shift_left(W.W[i - 3], 15, W15);
		cycle_shift_left(W.W[i - 13], 7, W7);
		W.W[i] = P1(W.W[i - 16] ^ W.W[i - 9] ^ W15) ^ W7 ^ W.W[i - 6];
	}
	for (int i = 0; i < 64; i++)
	{
		W_P.W_P[i] = W.W[i] ^ W.W[i + 4];
	}
	/*cout << "扩展后的消息W：";
	for (int i = 0; i < 68; i++)
	{
		cout << hex << W.W[i] << "   ";
	}
	cout << endl;
	cout << "扩展后的消息W_P：";
	for (int i = 0; i < 64; i++)
	{
		cout << hex << W_P.W_P[i] << "   ";
	}
	cout << endl;*/

}

//置换函数P0
unsigned int P0(unsigned int X)
{
	unsigned int X9 = 0, X17 = 0;
	cycle_shift_left(X, 9, X9);
	cycle_shift_left(X, 17, X17);
	return X ^ X9 ^ X17;
}

//置换函数P1
unsigned int P1(unsigned int X)
{
	unsigned int X15 = 0, X23 = 0;
	cycle_shift_left(X, 15, X15);
	cycle_shift_left(X, 23, X23);
	return X ^ X15 ^ X23;
}

unsigned int T(int i)
{

	if (i >= 0 && i <= 15)
	{
		return 0x79CC4519U;
	}
	else
		return 0x7A879D8AU;
}


//获取字符串转换为数字
SM3_B* get_string_test(string str)
{
	unsigned long long len = str.length();
	unsigned int p = 0;
	SM3_B* head = new SM3_B;
	SM3_B* temp = head;
	do
	{
		for (int i = 0; i < 16; i++)
		{
			temp->B[i] = (int)str[p];
			p++;
			for (int k = 0; k < 3; k++)
			{

				if (p < len)
				{
					temp->B[i] = (temp->B[i] << 8) | (unsigned int)str[p];
				}
				else
				{
					if (p == len)
					{
						temp->B[i] = (temp->B[i] << 8) | 1U << 7;
					}
					else
						temp->B[i] = temp->B[i] << 8;
				}
				p++;
			}

			if (p > len)break;
		}

		if ((p <= len) || ((len << 3) % 512) > 448)
		{
			temp->next = new SM3_B;
			temp = temp->next;
			if (p == len)
			{
				temp->B[0] = 0x80000000U;
			}
		}


	} while (p < len);
	len = len << 3;
	temp->B[14] = len >> 32;
	temp->B[15] = len & 0xffffffff;
	/*temp = head;
	cout << "填充之后的消息：";
	while (temp!=nullptr)
	{
		for (int i = 0; i < 16; i++)
		{
			cout << hex << temp->B[i] << " ";
		}
		cout << endl;
		temp = temp->next;
	}*/


	return head;
}

int main()
{
	string target = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
	SM3_B* B = get_string_test(target);
	SM3_V IV = main_cycle(B);

}
