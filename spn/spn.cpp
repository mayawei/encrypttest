#include<iostream>
#include<stdio.h>
#include<math.h>
#include<windows.h>
using namespace std;
int c[16][16];
unsigned short getbit(unsigned short a,int m)
{
	if ((a & (1 << m)) == 0) return 0;
	else return 1;
}
void bitcopy(unsigned short *a, unsigned short *b,int m,int n)
{
	if ((*a & (1 << m)) == 0) { (*b) &= (~(1 << n)); }
	else (*b) |= (1 << n);
};
unsigned short pis[] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
unsigned short unpis[] = { 14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5 };
int pip[] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
unsigned short k1 = 0x3a94;
unsigned short k2 = 0xa94d;
unsigned short k3 = 0x94d6;
unsigned short k4 = 0x4d63;
unsigned short k5 = 0xd63f;
unsigned short xtest[] = { 0xad21,0x5fff,0x6432 };
unsigned short ytest[] = { 0x91e9,0x58ee,0xeff4 };
void pishalf(unsigned short *a)
{
	*a = (pis[(*a >> 12)] << 12) + (pis[(*a >> 8) & 0x000f] << 8) + (pis[(*a >> 4) & 0x000f] << 4) + pis[*a & 0x000f];
}
void unpishalf(unsigned short *a)
{
	*a = (unpis[(*a >> 12)] << 12) + (unpis[(*a >> 8) & 0x000f] << 8) + (unpis[(*a >> 4) & 0x000f] << 4) + unpis[*a & 0x000f];
}
void piswitch(unsigned short *a)
{
	pishalf(a);
	unsigned short b;
	for (int i = 0; i <= 15; i++)
	{
		bitcopy(a, &b, 15 - i, 15 - pip[i]);
	};
	*a = b;
}
void unpiswitch(unsigned short *a)
{
	unsigned short b;
	for (int i = 0; i <= 15; i++)
	{
		bitcopy(a, &b, 15 - i, 15 - pip[i]);
	};
	*a = b;
	unpishalf(a);
}
unsigned short spn(unsigned short x)
{
	unsigned short x1 = x;
	x1 ^= k1;
	piswitch(&x1);
	x1 ^= k2;
	piswitch(&x1);
	x1 ^= k3;
	piswitch(&x1);
	x1 ^= k4;
	pishalf(&x1);
	return x1^k5;
}
unsigned short unspn(unsigned short x)
{
	unsigned short x1 = x;
	x1 ^= k5;
	unpishalf(&x1);
	x1 ^= k4;
	unpiswitch(&x1);
	x1 ^= k3;
	unpiswitch(&x1);
	x1 ^= k2;
	unpiswitch(&x1);
	return x1 ^ k1;
}
unsigned short lineratk()
{
	unsigned short y, v2, v4, u2, u4;
	unsigned short tL1 = 0, tL2 = 0, z = 0;
	unsigned short xmax = 65535;
	for (unsigned short x = 1; x <= 40000; x++)
	{
		y = spn(x);
		for (unsigned short L1 = 0; L1 <= 15; L1++)
		{
			for (unsigned short L2 = 0; L2 <= 15; L2++)
			{
				v2 = L1 ^ (0xf & (y >> 8));
				v4 = L2 ^ (y & 0xf);
				u2 = unpis[v2];
				u4 = unpis[v4];
				z = getbit(x, 11) ^ getbit(x, 9) ^ getbit(x, 8) ^ getbit(u2, 0) ^ getbit(u2, 2) ^ getbit(u4, 0) ^ getbit(u4, 2);
				if (z == 0) c[L1][L2]++;
			}
		}
	}
	int max = -1;
	for (unsigned short L1 = 0; L1 <= 15; L1++)
	{
		for (unsigned short L2 = 0; L2 <= 15; L2++)
		{
			c[L1][L2] = abs(c[L1][L2] - 20000);
			if (c[L1][L2] > max)
			{
				max = c[L1][L2];
				tL1 = L1; tL2 = L2;
			};
		}
	}
	return (tL1 << 8) + tL2;
}
unsigned int difatk()
{
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 15; j++)
		{
			c[i][j] = 0;
		}
	}
	unsigned int y1 = 0, y2 = 0;
	int i = 0;
	for (unsigned short x1 = 0; i <= 0xffff; x1++,i++)
	{
		unsigned short x2 = x1 ^ 0x0b00;
		y1 = spn(x1);
		y2 = spn(x2);
		if (((y1^y2) & 0xf0f0) != 0) continue;
		else for (unsigned short L1 = 0; L1 <= 15; L1++)
		{
			for (unsigned short L2 = 0; L2 <= 15; L2++)
			{
				unsigned short v2 = L1 ^ ((y1 >> 8) & 0xf);
				unsigned short v4 = L2 ^ (y1 & 0xf);
				unsigned short u2 = unpis[v2];
				unsigned short u4 = unpis[v4];
				unsigned short v22 = L1 ^ ((y2 >> 8) & 0xf);
				unsigned short v42 = L2 ^ (y2 & 0xf);
				unsigned short u22 = unpis[v22];
				unsigned short u42 = unpis[v42];
				if (((u2 ^ u22) == 6) && ((u4 ^ u42) == 6))
				{
					c[L1][L2]++;
				}
			}
		}
	}
	int max = -1;
	unsigned short tL1,tL2;
	for (unsigned short L1 = 0; L1 <= 15; L1++)
	{
		for (unsigned short L2 = 0; L2 <= 15; L2++)
		{
			if (c[L1][L2] > max)
			{
				max = c[L1][L2];
				tL1 = L1;
				tL2 = L2;
			}
		}
	}
	return ((tL1 << 8) + tL2);
};
unsigned int forceatk(unsigned short key)
{
	unsigned short uk1, uk2, uk3, uk4, uk5;
	int i = 0;

	for (int k1 = 0; k1 <= 15; k1++)
	{
		for (int k2 = 0; k2 <= 15; k2++)
		{
			for (int k3 = 0; k3 <= 15; k3++)
			{
				for (int k4 = 0; k4 <= 15; k4++)
				{
					for (int k5 = 0; k5 <= 15; k5++)
					{
						for (int k6 = 0; k6 <= 15; k6++)
						{
							int countnum[] = { 0,0,0 };
							uk1 = (k1 << 12) + (k2 << 8) + (k3 << 4) + k4;
							uk2 = (k2 << 12) + (k3 << 8) + (k4 << 4) + k5;
							uk3 = (k3 << 12) + (k4 << 8) + (k5 << 4) + (key >> 8);
							uk4 = (k4 << 12) + (k5 << 8) + ((key >> 8) << 4) + k6;
							uk5 = (k5 << 12) + key + (k6 << 4);
//#pragma omp for
							for (int x_1 = 0; x_1 <= 2; x_1++)
							{
								unsigned short x1 = xtest[x_1];
								x1 ^= uk1;
								piswitch(&x1);
								x1 ^= uk2;
								piswitch(&x1);
								x1 ^= uk3;
								piswitch(&x1);
								x1 ^= uk4;
								pishalf(&x1);
								if ((x1 ^ uk5) != ytest[x_1]) break;
								else countnum[x_1]++;
							}
							if(countnum[0]&&countnum[1]&&countnum[2]) return ((k1 << 28) + (k2 << 24) + (k3 << 20) + (k4 << 16) + (k5 << 12) + (k6 << 4) + key);
						}
					}
				}
			}
		}
	}
}
int main()
{
	//unsigned short x = 0x3131;
	//unsigned short y = spn(x);
	//x = unspn(y);
	unsigned short L = difatk();
	//unsigned int i = forceatk(L);
	return 0;
}
