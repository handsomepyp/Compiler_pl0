/*���ͳ���*/
#include<iostream>
#include<fstream>
using namespace std;
fstream infile_second;
struct Instruction
{
	string instruction;// ����ָ��
	int len;//���
	int par;
};
int Stack[1000];
/*���ݲ�Ѱ�ҷǾֲ�����*/
int getBase(int SP, int lev) // ��̬��˼�룬levΪ���
{
	int oldSP = SP;
	while (lev > 0)
	{
		oldSP = Stack[oldSP + 1];
		lev--;
	}
	return oldSP;
}
/*�������*/
int translator() // ����pcode
{
	Instruction ins[200];//���е�ָ������
	infile_second.open("out.txt");
	int index = 0;
	while (!infile_second.eof())
	{
		Instruction i;
		infile_second >> i.instruction >> i.len >> i.par;
		ins[index] = i;
		index++;
	}
	/*for (int i = 0; i < index; i++)
		cout << ins[i].instruction << " " << ins[i].len << " " << ins[i].par << endl;*/
	//cout << index;
	/*The length of interpretor is index*/
	int i = 0; // ָ��Ĵ���
	int P = 0; // �����һ��Ҫִ�е�ָ���ַ
	int B = 0; // ����ַ�Ĵ���
	int Stack_index = 0; //ջ��ָʾ���Ĵ���
	int temp;
	do
	{
		i = P;
		P++;
		if (ins[i].instruction == "lit")//ȡ����a����ջ��
		{
			//cout << ins[i].instruction << endl;
			Stack[Stack_index] = ins[i].par;
			Stack_index++;
		}
		else if(ins[i].instruction == "opr") //ִ������,a����ĳ������
		{
			//cout << ins[i].instruction << endl;
			if (ins[i].par == 0)// ���̵��ý�����,���ص��õ㲢��ջ
			{
				Stack_index = B;
				P = Stack[B + 2]; // ��һ��Ҫִ�е�ָ���ַ
				B = Stack[B];
			}
			else if (ins[i].par == 1)// ȡ��
			{
				Stack[Stack_index - 1] = Stack[Stack_index - 1] * (-1);
			}
			else if (ins[i].par == 2)// +
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 1] + Stack[Stack_index - 2];
				Stack_index--;
			}
			else if (ins[i].par == 3)//-
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 2] - Stack[Stack_index - 1];
				Stack_index--;
			}
			else if (ins[i].par == 4)// *
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 2] * Stack[Stack_index - 1];
				Stack_index--;
			}
			else if (ins[i].par == 5)// /
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 2] / Stack[Stack_index - 1];
				Stack_index--;
			}
			else if (ins[i].par == 6)// �ж���ż��
			{
				Stack[Stack_index - 1] = Stack[Stack_index - 1] % 2;
			}
			else if (ins[i].par == 7)
			{

			}
			else if (ins[i].par == 8)// �ж�ջ���ʹ�ջ���Ƿ����
			{
				if (Stack[Stack_index - 2] == Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 9)// �ж�ջ���ʹ�ջ���Ƿ񲻵�
			{
				if (Stack[Stack_index - 2] != Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 10)// ��ջ���Ƿ�С��ջ��
			{
				if (Stack[Stack_index - 2] < Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 11)// ��ջ���Ƿ���ڵ���ջ��
			{
				if (Stack[Stack_index - 2] >= Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 12)// ��ջ���Ƿ����ջ��
			{
				if (Stack[Stack_index - 2] > Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 13)// ��ջ���Ƿ�С�ڵ���ջ��
			{
				if (Stack[Stack_index - 2] <= Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 14)// ջ�����
			{
				cout << Stack[Stack_index - 1] << endl;
			}
			else if (ins[i].par == 15)// �������
			{
				cout << endl;
			}
			else if (ins[i].par == 16)// �����ж���һ����������ջ��
			{
				int temp;
				cin >> temp;
				Stack[Stack_index] = temp;
				Stack_index++;
			}
		}
		else if (ins[i].instruction == "lod")// ȡ����(��Ե�ַΪa,���ΪL)��������ջ��ջ��
		{
			//cout << ins[i].instruction << endl;
			Stack[Stack_index] = Stack[ins[i].par + getBase(B, ins[i].len)];
			Stack_index++;
		}
		else if (ins[i].instruction == "sto")// ������ջջ�������ݴ������
		{
			//cout << ins[i].instruction << endl;
			Stack[ins[i].par + getBase(B, ins[i].len)] = Stack[Stack_index - 1];
			Stack_index--;
		}
		else if (ins[i].instruction == "cal")// ���ù���
		{
			//cout << ins[i].instruction << endl;
			Stack[Stack_index] = B;//��̬����ֱ��������
			Stack[Stack_index + 1] = getBase(B, ins[i].len);	//��̬��������ǰ���й���
			Stack[Stack_index + 2] = P;		//���ص�ַ����һ��Ҫִ�е�
			B = Stack_index;
			P = ins[i].par;
		}
		else if (ins[i].instruction == "int") // ����ջջ��ָ������ a
		{
			//cout << ins[i].instruction << endl;
			Stack_index = Stack_index + ins[i].par;
		}
		else if (ins[i].instruction == "jmp")//������ת�Ƶ���ַΪa��ָ��
		{
			//cout << ins[i].instruction << endl;
			P = ins[i].par;
		}
		else if (ins[i].instruction == "jpc")// ��������ת����ַΪa��ָ��,ֻ�е�ѭ������Ϊ�ٵ�ʱ�������
		{
			//cout << ins[i].instruction << endl;
			if (Stack[Stack_index - 1] == 0) P = ins[i].par;
		}
		else if (ins[i].instruction == "red")// �����ж���һ������ջ��
		{
			//cout << ins[i].instruction << endl;
			cin >> temp;
			Stack[ins[i].par + getBase(B, ins[i].len)] = temp;
		}
		else if (ins[i].instruction == "wrt")// ջ���������Ļ��
		{
			//cout << ins[i].instruction << endl;
			cout << Stack[Stack_index - 1] << " ";
		}
	} while (P != 0);
	return 0;
}