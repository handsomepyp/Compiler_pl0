#include<fstream>
#include<iostream>
#include<string>
#include<stack>
#include<sstream> //istringstream 
using namespace std;

fstream ga_in;
fstream ga_out;

struct Unit
{
	string value; //�ַ���
	string key; //����
	int row;
	int column;
};
/*���ű�*/
struct SymTable
{
	string name;   //��ʶ������
	int type;		//��ʶ������ 
	/*
		0 ������
		1 �������
		2 �������
	*/
	int value;		//��ʶ����ֵ
	int level;		//��ʶ�������Ĳ��
	int addr;    //��ʶ�����������ƫ����
	int size;       //��ʶ����С��Ĭ��Ϊ4  ����ʶ��Ϊ���̶����ʶ�����˴���¼�βθ���
};
SymTable symtable[100]; // ���ű�����
int tx = 0; // ���ű��±�ָ��
int dx = 0; // ��Ա����̻���ַ��ƫ����
int lev; // ��ǰ��Σ�ȫ�ֱ���
int number; // �������
/*pcode*/
struct Instr
{
	string instruction;// ����ָ��
	int len;//���
	int par;
}; 
Instr ins[200]; // ���pcode
int cx; // pcode���±�ָ��

void errortype(int type);//��������
void OPENFILE();
void Readline();
void factor();
void term();
void condecl();
void exp();
void lexp();
void block();
void vardecl();
void proc();
void statement();
void body();
void CLOSEFILE();

void entervar(string name, int level, int dx); // ���ű������������ʶ��
void enterconst(string name, int level, int value, int dx); // ���ű������볣����ʶ��
void enterproc(string name, int level, int dx); // ���ű����������
int position_insymtable(string name); // �����ڷ��ű��е�λ��
int searchProc();
bool isPreDefine(string name, int level); // �鿴��ʶ���Ƿ��Ѷ��壬ͬ������
bool isExistDefine(string name, int level); // �Ƿ�֮ǰ�Ѷ������ͬ��ε�
void gen(string instruction, int l, int a);//����pcode

Unit unit; // ��ǰɨ��
Unit last_unit; // ��һ��ɨ�� 
int errorStack[1000] = { -1 };//��Ŵ������ 
int* errorType = errorStack;//ָ����� 
bool error = false;
/*
	* spell error ƴд����
	* Missing identifier ȱ�ٱ�ʶ��
	*
*/
void errortype(int type)//�������� 
{
	error = true;
	*errorType = type;//�洢��ǰ�Ĵ��� 
	errorType++;//ָ����һ������ 
	switch (type)
	{
	case 0:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Spell error \"program\"" << endl;
		break;
	case 1:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"program\"" << endl;
		break;
	case 2:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "]" << "Missing \";\" after \"id\"" << endl;
		break;
	case 3:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Cannot resolve error" << endl;
		break;
	case 4:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"begin\"" << endl;
		break;
	case 5:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"end\"" << endl;
		break;
	case 6:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing assignment operation" << endl;
		break;
	case 7:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"then\"" << endl;
		break;
	case 8:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"do\"" << endl;
		break;
	case 9:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier in \"call\"" << endl;
		break;
	case 10:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses in \"call\"" << endl;
		break;
	case 11:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier in \"read\"" << endl;
		break;
	case 12:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses in \"read\"" << endl;
		break;
	case 13:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses in \"write\"" << endl;
		break;
	case 14:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Wrong factor" << endl;
		break;
	case 15:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses" << endl;
		break;
	case 16:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"const\"" << endl;
		break;
	case 17:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing operator" << endl;
		break;
	case 18:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing \";\" in the end" << endl;
		break;
	case 19:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing integer" << endl;
		break;
	case 20:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing the compare operator" << endl;
		break;
	case 21:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"var\"" << endl;
		break;
	case 22:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"procedure\"" << endl;
		break;
	case 23:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses" << endl;
		break;
	case 24:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing \";\" in the end" << endl;
		break;
	case 25:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing comma" << endl;
		break;
	case 26:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing \";\"" << endl;
		break;
	case 27:cout << "[Grammar error] " << " [" << last_unit.row << "," << last_unit.column << "] " << "Extra semicolon \";\" before \"end\"" << endl;
		break;
	default:cout << "[Grammar ERROR] " << " [" << unit.row << "," << unit.column << "] " << "Unknown error" << endl;
		break;
	}
}
void OPENFILE()//���ļ����� 
{
	//res1:�ʷ������Ľ��,��result.txt�洢�﷨�����Ľ�� 
	ga_in.open("res1.txt");
	if (!ga_in)
	{
		cout << "Can't open res1.txt" << "\n";
		exit(0);
	}
	ga_out.open("out.txt");
	if (!ga_out)
	{
		cout << "Can't open out.txt" << "\n";
		exit(0);
	}
}
string line;
void Readline()//��һ�����ݵĲ��� 
{
	getline(ga_in, line);
	istringstream iss(line);//���Ѷ���һ���г�ȡ����Ԫ�� 
	last_unit = unit;
	iss >> unit.value;//����ĵ��� 
	iss >> unit.key;//�������� 
	iss >> unit.row;
	iss >> unit.column;
}
/*
	* <factor> -> <id>|<integer>|(<exp>)
*/
void factor()
{
	if (unit.key == "id")
	{
		string name = unit.value;
		int i = position_insymtable(name);
		if (symtable[i].type == 1) // ����
			gen("lod", lev - symtable[i].level, symtable[i].addr);
		else if (symtable[i].type == 0) //����
			gen("lit", 0, symtable[i].value);	

		Readline();
	}
	else if (unit.key == "int")
	{
		//cout << unit.value << endl;
		int sum = 0;
		for (int ii = 0; ii < unit.value.size(); ii++)
		{
			sum = sum * 10 + unit.value[ii] - '0';
		}
		gen("lit", 0, sum);
		Readline();
	}
	else if ((unit.value == "(" && unit.key == "...") || *errorType == 14)
	{
		if (*errorType == 14) errorType++;
		Readline();
		exp();
		if ((unit.value == ")" && unit.key == "...") || *errorType == 15)
		{
			if (*errorType == 15) errorType++;
			else Readline();
		}
		else errortype(15);//ȱ���� 
	}
	else errortype(14);//�����factor 
}
/*
	* <term> -> <factor>{<mop><factor>}
*/
void term()
{
	factor();
	while (unit.value == "*" || unit.value == "/")
	{
		string str_temp = unit.value;
		Readline();
		factor();
		if (str_temp == "*")
		{
			gen("opr", 0, 4);
		}
		else if (str_temp == "/")
		{
			gen("opr", 0, 5);
		}
	}
}
/*
	*<condecl> -> const <const> {,<const>}
*/
void condecl()
{
	string name;
	int value;
	if (unit.key == "id" || *errorType == 16)
	{
		name = unit.value;
		if (*errorType == 16) errorType++;
		else Readline();
		if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
		{
			if (*errorType == 17) errorType++;
			else Readline();
			if (unit.key == "int" || *errorType == 19)
			{
				int sum = 0;
				for (int ii = 0; ii < unit.value.size(); ii++) // ��һ���ַ���ת��Ϊ����
				{
					sum = sum * 10 + unit.value[ii] - '0';
				}
				value = sum;
				enterconst(name, lev, value, dx); // �������뵽���ű���
				//dx++;
				if (*errorType == 19)
				{
					errorType++;
					/*while (unit.value != "," && unit.value != ";")
					{
						Readline();
						if (unit.key == "id") break;//ȱ��
					}*/
				}
				else Readline();
				while (unit.value == "," && unit.key == "...")
				{
					Readline();
					if (unit.key == "id" || *errorType == 16)
					{
						name = unit.value;
						if (*errorType == 16) errorType++;
						else Readline();
						if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
						{
							if (*errorType == 17) errorType++;
							else Readline();
							if (unit.key == "int" || *errorType == 19)
							{					
								int sum = 0;
								for (int ii = 0; ii < unit.value.size(); ii++) // ��һ���ַ���ת��Ϊ����
								{
									sum = sum * 10 + unit.value[ii] - '0';
								}
								value = sum;
								enterconst(name, lev, value, dx); // �������뵽���ű���
								// dx++;
								if (*errorType == 19)
								{
									errorType++;
									/*while (unit.value != "," && unit.value != ";")
									{
										Readline();
										if (unit.key == "id") break;//ȱ�� ��
									}*/
								}
								else Readline();
							}
							else errortype(19);//ȱ��<const>����><id>:=<integer> integer	
						}
						else errortype(17); //ȱ��<const>���� > <id>: = <integer> : =
					}
					else errortype(16); //ȱ��<const>���� > <id>: = <integer> id
				}
				if ((unit.value == ";" && unit.key == "...") || *errorType == 18)
				{
					if (*errorType == 18) errorType++;
					else Readline();
				}
				else errortype(18);//ȱ�ٷֺ�
			}
			else errortype(19);//ȱ��<const>����><id>:=<integer> integer	
		}
		else errortype(17);//ȱ��<const>����><id>:=<integer> :=
	}
	else errortype(16);// ȱ��<const>����><id>:=<integer> id
}
/*
	* <exp> -> [+|-] <term>{<aop><term>}
*/
void exp()
{
	string str_temp = unit.value;
	if (unit.value == "+" || unit.value == "-")
	{
		Readline();
	}
	term();
	if (str_temp == "-")
	{
		gen("opr", 0, 1);
	}
	while ((unit.value == "+" || unit.value == "-") && unit.key == "aop")
	{
		str_temp = unit.value;
		Readline();
		term();
		if (str_temp == "+") gen("opr", 0, 2);
		else if (str_temp == "-") gen("opr", 0, 3);
	}
}
/*
	* <lexp> -> <exp><lop><exp>   |  odd <exp>
*/
void lexp()
{
	if (unit.value == "odd" && unit.key == "keyword")
	{
		Readline();
		exp();
		gen("opr", 0, 6);
	}
	else
	{
		exp();
		if (((unit.value == "=" || unit.value == "<>" || unit.value == "<" || unit.value == "<=" || unit.value == ">" || unit.value == ">=") && unit.key == "lop") || *errorType == 20)
		{
			string str_temp = unit.value;
			if (*errorType == 20)
			{
				errorType++;
			}
			else Readline();
			exp();
			if (str_temp == "=") gen("opr", 0, 8);
			else if(str_temp == "<>") gen("opr", 0, 9);
			else if(str_temp == "<") gen("opr", 0, 10);
			else if(str_temp == "<=") gen("opr", 0, 13);
			else if(str_temp == ">") gen("opr", 0, 12);
			else if (str_temp == ">=") gen("opr", 0, 11);
		}
		else errortype(20);//ȱ�ٱȽϷ��� 
	}
}
/*
	*<block> -> [<condecl>][<vardecl>][<proc>]<body>
*/
void block()
{
	int dx0 = dx;            // ��¼��ǰ�Ĳ�Σ��Ա�ָ�ʱʹ��
	int tx_wait = tx + 1;        // ��¼���ű�ǰ�ȴ������λ��
	int n = 0;
	if (tx_wait > 1)
	{
		n = searchProc();                 //Ѱ�ҹ��̱�ʶ��
		tx_wait = tx_wait - symtable[n].size;     //��ȥ�βεĸ������ָ�ʱ�β�Ҳ������
	}
	if (tx_wait == 1)
		dx = 3;    // dx��ʼ������̬������̬�������ص�ַ
	else
		dx = 3 + symtable[n].size;   // + 3 :�β����ھ�̬������̬�������ص�ַ�ͱ���������֮ǰ
	int cx_temp;
	cx_temp = cx;    // ��¼jmp��ǰ��λ��(��ǰ�±�)
	gen("jmp", 0, 0);
	if (unit.value == "const" && unit.key == "keyword")
	{
		Readline();
		condecl();
	}
	/*else if(unit.key == "id")
	{
		Readline();
		errortype(3);//�޷��жϴ�������
	}*/
	if (unit.value == "var" && unit.key == "keyword")
	{
		Readline();
		vardecl();
	}
	if (unit.value == "procedure" && unit.key == "keyword")
	{
		Readline();
		proc();
		lev--; // ��Ƕ�ף���� -1
	}
	if (tx_wait > 1)
	{
		int i;
		n = searchProc(); // ��Ѱ�ҵ��ù���
		for (i = 0; i < symtable[n].size; i++) // ��������
		{
			gen("sto", 0, symtable[n].size + 3 - 1 - i);   // ��ʵ�ε�ֵ�����β�
		}
	}
	ins[cx_temp].par = cx;      // ����jmpָ��
	gen("int", 0, dx);        // ����dx���ռ�
	if (tx != 1)
		symtable[n].value = cx - 1 - symtable[n].size;    //��������ڵ�ַ������̶����value���Ա�callָ��ʹ��
	/*for (int i = 0; i <= cx; i++)
		cout << symtable[i].name << " " << symtable[i].level << endl;*/
	//cout << symtable[6].name << endl;
	body();
	gen("opr", 0, 0);
	tx = tx_wait;                       //����ű������ű�ָ����ǰ����tx0��
	dx = dx0;
}
/*
	*<prog> -> program<id>;<block>
*/
void prog()
{
	Readline();
	if ((unit.value == "program" && unit.key == "keyword") || *errorType == 0)
	{
		if (*errorType == 0)  errorType++;
		Readline();
		if (unit.key == "id" || *errorType == 1)
		{
			if (*errorType == 1) errorType++;
			else Readline();
			if ((unit.value == ";" && unit.key == "...") || *errorType == 2)
			{
				if (*errorType == 2 && unit.value != ";") errorType++;
				else Readline();
				block();
			}
			else errortype(2);// ȱ�� ";" 
		}
		else errortype(1);//����id��ȱ�ٱ�ʶ��
	}
	else errortype(0);//programƴд���� 
}
/*
	*<vardecl> -> var <id> {,<id>};
*/
void vardecl()
{
	string name;
	if (unit.key == "id" || *errorType == 21)
	{
		name = unit.value;
		int value;
		if (*errorType == 21) errorType++;
		else Readline();
		entervar(name, lev, dx);
		dx++;
		while ((unit.value == "," && unit.key == "...") || *errorType == 25)
		{
			if (*errorType == 25) errorType++;
			else Readline();
			if (unit.key == "id" || *errorType == 21)
			{
				if (unit.key == "id")
				{
					// cout << unit.value << endl;
					name = unit.value;
					entervar(name, lev, dx);
					dx++;
				}
				if (*errorType == 21) errorType++;
				else Readline();
			}
			else errortype(21); //ȱ��id 
		}
		if (unit.key == "id") errortype(25);//ȱ�ٶ��� 
		if ((unit.value == ";" && unit.key == "...") || *errorType == 26) //ȱ�ٷֺ�
		{
			if (*errorType == 26) errorType++;
			else Readline();
		}
		else errortype(26);
	}
	else errortype(21);
}
/*
	* <proc> -> procedure <id> ([<id>{,<id>}]);<block>{;<proc>}
*/
void proc()
{
	int count = 0; // ��¼�βθ���
	int tx_temp;
	if (unit.key == "id" || *errorType == 22)
	{
		string name = unit.value;
		if (isExistDefine(name, lev))
		{

		}
		tx_temp = tx + 1; // ���ű�ָ��+1
		enterproc(name, lev, dx); // �����̴�����ű�
		lev++; // ����+1
		if (*errorType == 22) errorType++;
		else Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			if (unit.key == "id")
			{
				entervar(unit.value, lev, 3 + count);  
				count++;                             
				symtable[tx_temp].size = count; // ������¼�βθ���
				Readline();
				while (unit.key == "..." && unit.value == ",")
				{
					Readline();
					if (unit.key == "id")
					{
						entervar(unit.value, lev, 3 + count); // ������������ű� 
						count++;                             // �βθ�����+1
						symtable[tx_temp].size = count; // ���ű����¸�ֵ
						Readline();
					}
					//else errortype(22);
				}
			}
			if ((unit.value == ")" && unit.key == "...") || *errorType == 23)
			{
				if (*errorType == 23) errorType++;
				else Readline();
				if ((unit.value == ";" && unit.key == "...") || *errorType == 24)
				{
					if (*errorType == 24) errorType++;
					else Readline();
					block();
					while (unit.value == ";" && unit.key == "...")
					{
						Readline();
						proc();
					}
				}
				else errortype(24);
			}
			else errortype(23);//ȱ��������
		}
	}
	else errortype(22);//ȱ��id
}
/*
	*<statement> ->  <id> := <exp>
									|if <lexp> then <statement> [else <statement>]
									|while <lexp> do <statement>
									|call <id> ([<exp>{,<exp>}])
									|<body>
									|read (<id>{,<id>})
									|write(<exp>{,<exp>})
*/
void statement()
{
	if (unit.key == "id")
	{// <id> := <exp>
		string name = unit.value;
		Readline();
		if ((unit.value == ":=" && unit.key == "...") || *errorType == 6)
		{
			if (*errorType == 6) errorType++;
			else Readline();
			exp();
			/*if (!isPreDefine(name,lev))
			{
				//cout << "ok" << endl;
				return;
			}
			else
			{	*/
				//cout << name << "\n";
			int i = position_insymtable(name);
			if (symtable[i].type == 1)
			{
				gen("sto", lev - symtable[i].level, symtable[i].addr);
			}
			//}
		}
		else errortype(6);// ȱ�ٸ�ֵ����� 
	}
	else if (unit.value == "if" && unit.key == "keyword")
	{// if <lexp> then <statement> [else <statement>]
		int cx_temp1;
		Readline();
		lexp();
		if (unit.key == "int") Readline();
		if ((unit.value == "then" && unit.key == "keyword") || *errorType == 7)
		{
			cx_temp1 = cx;
			gen("jpc", 0, 0);
			if (*errorType == 7) errorType++;
			else Readline();
			statement();
			int cx_temp2 = cx;
			gen("jmp", 0, 0);
			ins[cx_temp1].par = cx;
			ins[cx_temp2].par = cx;
			if (unit.value == "else" && unit.key == "keyword")
			{
				Readline();
				statement();
				ins[cx_temp2].par = cx;
			}
		}
		else errortype(7); //ȱ��then 
	}
	else if (unit.value == "while" && unit.key == "keyword")
	{// while <lexp> do <statement>
		Readline();
		int cx_temp1 = cx;
		lexp();
		if ((unit.value == "do" && unit.key == "keyword") || *errorType == 8)
		{
			int cx_temp2 = cx;
			gen("jpc", 0, 0);
			if (*errorType == 8) errorType++;
			else Readline();
			statement();
			gen("jmp", 0, cx_temp1);
			ins[cx_temp2].par = cx;
		}
		else errortype(8);//ȱ��do 
	}
	else if (unit.value == "call" && unit.key == "keyword")
	{// call <id> ([<exp>{,<exp>}])
		Readline();
		int i;
		int count = 0;
		if (unit.key == "id" || *errorType == 9)
		{			
			i = position_insymtable(unit.value);
			if (symtable[i].type == 2)
			{

			}
			else
			{

			}
			if (*errorType == 9) errorType++;
			else Readline();
			if (unit.value == "(" && unit.key == "...")
			{
				Readline();
				if (unit.value == ")" && unit.key == "...")
				{
					Readline();
					gen("cal", lev - symtable[i].level, symtable[i].value);
				}
				else
				{
					exp();
					count++;
					while (unit.key == "..." && unit.value == ",")
					{
						Readline();
						exp();
						count++;
					}
					if (count != symtable[i].size)
					{

					}
					gen("cal", lev - symtable[i].level, symtable[i].value);
					if ((unit.key == "..." && unit.value == ")") || *errorType == 10)
					{
						if (*errorType == 10) errorType++;
						else Readline();
					}
					else errortype(10);
				}	
			}
		}
		else errortype(9); // ȱ��call�ı�ʶ�� 
	}
	else if (unit.value == "read" && unit.key == "keyword")
	{// read (<id>{,<id>})
		Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			if (unit.key == "id" || *errorType == 11)
			{
				if (unit.key == "id")
				{
					int i = position_insymtable(unit.value);
					if (symtable[i].type == 1)
					{
						gen("opr", 0 , 16);
						gen("sto", lev - symtable[i].level, symtable[i].addr);
					}
				}
				if (*errorType == 11) errorType++;
				else Readline();
				while (unit.value == "," && unit.key == "...")
				{
					Readline();
					if (unit.key == "id")
					{
						int i = position_insymtable(unit.value);
						if (symtable[i].type == 1)
						{
							gen("opr", 0, 16);
							gen("sto", lev - symtable[i].level, symtable[i].addr);
						}
						Readline();
					}
				}
				if ((unit.value == ")" && unit.key == "...") || *errorType == 12)
				{
					if (*errorType == 12) errorType++;
					else Readline();
				}
				else errortype(12);
			}
			else errortype(11);
		}
	}
	else if (unit.value == "write" && unit.key == "keyword")
	{// write(<exp>{,<exp>})
		Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			exp();
			gen("wrt", 0, 0);
			while (unit.value == "," && unit.key == "...")
			{
				Readline();
				exp();
				gen("wrt", 0, 0);
			}
			gen("opr", 0, 15);
			if ((unit.value == ")" && unit.key == "...") || *errorType == 13)
			{
				if (*errorType == 13) errorType++;
				else Readline();
			}
			else errortype(13);
		}
	}
	else body();
}
/*
	*  <body> -> begin <statement>{;<statement>} end
*/
void body()
{
	if ((unit.key == "keyword" && unit.value == "begin") || *errorType == 4)
	{
		if (*errorType == 4) errorType++;
		else Readline();
		statement();
		while (unit.value == ";" && unit.key == "..." && !error)
		{
			Readline();
			if (unit.value == "end" && unit.key == "keyword")
			{
				if (*errorType != 27) errortype(27);//���һ���ֺ�
				break;
				return;
			}
			statement();
		}
		/*if(*errorType != 27)
		{
			errorType++;
			Readline();
		}*/
		if ((unit.value == "end" && unit.key == "keyword") || *errorType == 5 || error)
		{
			if (*errorType == 5) errorType++;
			Readline();
		}
		else errortype(5);//ȱ��end 
	}
	else errortype(4);//ȱ��begin 
}
void CLOSEFILE()//�ر��ļ����� 
{
	ga_in.close();
	ga_out.close();
}
int GAmeaning()
{
	OPENFILE();
	prog();
	CLOSEFILE();
	if (!error) cout << "���﷨����" << endl;
	while (error)
	{
		error = false;
		errorType = errorStack;
		OPENFILE();
		prog();
		CLOSEFILE();
	}
	fstream outfile;
	outfile.open("out.txt");
	for (int i = 0; i < cx; i++)
	{
		cout << ins[i].instruction << " " << ins[i].len << " " << ins[i].par << endl;
		outfile << ins[i].instruction << " " << ins[i].len << " " << ins[i].par << endl;
	}
	outfile.close();
	return 0;
}
/*���ű������������ʶ��������Ϊ 1 */
void entervar(string name,int level,int dx)
{
	tx++;
	symtable[tx].name = name;
	symtable[tx].type = 1;
	symtable[tx].level = level;
	symtable[tx].addr = dx;
}
/*���ű������볣����ʶ��*/
void enterconst(string name,int level,int value,int dx)
{
	tx++;
	symtable[tx].name = name;
	symtable[tx].type = 0; // 0������
	symtable[tx].value = value;
	symtable[tx].level = level;
	symtable[tx].addr = dx;
}
/*���ű���������̱�ʶ��*/
void enterproc(string name, int level, int dx)
{
	tx++;
	symtable[tx].name = name;
	symtable[tx].type = 2;
	symtable[tx].level = level;
	symtable[tx].addr = dx;
}
/*���ҷ��ű��б�ʶ����λ��*/
int position_insymtable(string name)
{
	int i;
	for (i = tx; i > 0; i--)
	{
		//cout << symtable[i].name << endl;
		if (name == symtable[i].name) break;
	}
	return i;
}
/*���ҹ��̶���ı�ʶ��*/
int searchProc()
{
	int i;
	//cout << tx << endl;
	for (i = tx; i >= 1; i--)
	{
		//cout << symtable[i].type << endl;
		if (symtable[i].type == 2)
		{
			return i;
		}
	}
	return -1;
}
/*���ұ�ʶ���Ƿ��ڱ������㶨���*/
bool isPreDefine(string name, int level)
{
	int i;
	for (i = 1; i <= tx; i++)
	{
		if (symtable[i].name == name)
		{
			if (symtable[i].level == level) return true; // ���ұ����
		}
		else
		{
			do
			{
				level--;
				if (symtable[i].level == level) return true; // ��������
			} while (level >= 0);
		}
	}
	return false;
}
/*�ֵܲ�*/
bool isExistDefine(string name, int level)
{
	int i;
	for (i = 1; i <= tx; i++)
	{
		if (symtable[i].name == name && symtable[i].level == level) return true;
	}
	return false;
}
/*����pcode*/
void gen(string f, int l, int a)
{
	ins[cx].instruction = f;
	ins[cx].len = l;
	ins[cx].par = a;
	cx++;
}
