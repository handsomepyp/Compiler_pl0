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
	string value;
	string key;
	int row;
	int column;
};
Unit unit;
Unit last_unit;//��һ�� 
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
	//if(unit.value == "")
}
void exp();
/*
	* <factor> -> <id>|<integer>|(<exp>)
*/
void factor()
{
	if (unit.key == "id") Readline();
	else if (unit.key == "int")
	{
		//cout << unit.value;
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
		Readline();
		factor();
	}
}
/*
	*<condecl> -> const <const> {,<const>}
*/
void condecl()
{
	if (unit.key == "id" || *errorType == 16)
	{
		if (*errorType == 16) errorType++;
		else Readline();
		if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
		{
			if (*errorType == 17) errorType++; 
			else Readline();
			if (unit.key == "int" || *errorType == 19)
			{
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
				while (unit.value == "," && unit.key == "..." )
				{
					Readline();
					if (unit.key == "id" || *errorType == 16)
					{
						if (*errorType == 16) errorType++;
						else Readline();
						if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
						{
							if (*errorType == 17) errorType++;
							else Readline();
							if (unit.key == "int" || *errorType == 19)
							{
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
	if (unit.value == "+" || unit.value == "-")
	{
		Readline();
	}
	term();
	while ((unit.value == "+" || unit.value == "-") && unit.key == "aop")
	{
		Readline();
		term();
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
	}
	else
	{
		exp();
		if (((unit.value == "=" || unit.value == "<>" || unit.value == "<" || unit.value == "<=" || unit.value == ">" || unit.value == ">=") && unit.key == "lop") || *errorType == 20)
		{
			if (*errorType == 20)
			{
				errorType++;
			}
			else Readline();
			exp();
		}
		else errortype(20);//ȱ�ٱȽϷ��� 
	}
}
/*
	*<block> -> [<condecl>][<vardecl>][<proc>]<body>
*/
void vardecl();
void proc();
void body();
void block()
{
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
	}
	body();
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
	if (unit.key == "id" || *errorType == 21)
	{
		if (*errorType == 21) errorType++;
		else Readline();
		while ((unit.value == "," && unit.key == "..." )|| *errorType == 25)
		{
			if (*errorType == 25) errorType++;
			else Readline();
			if (unit.key == "id" || *errorType == 21)
			{
				if (*errorType == 21) errorType++;
				else Readline();
			}
			else errortype(21); //ȱ��id 
		}
		if (unit.key == "id") errortype(25);//ȱ�ٶ��� 
		if ((unit.value == ";" && unit.key == "..." )|| *errorType == 26)
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
	if (unit.key == "id"||*errorType == 22)
	{
		if (*errorType == 22) errorType++;
		else Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			if (unit.key == "id")
			{
				Readline();
				while (unit.key == "..." && unit.value == ",")
				{
					Readline();
					if (unit.key == "id")
					{
						Readline();
					}
					//else errortype(22);
				}
			}
			if ((unit.value == ")" && unit.key == "...")||*errorType == 23)
			{
				if (*errorType == 23) errorType++;
				else Readline();
				if ((unit.value == ";" && unit.key == "...")||*errorType == 24)
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
		Readline();
		if ((unit.value == ":=" && unit.key == "...") || *errorType == 6)
		{
			if (*errorType == 6) errorType++;
			else Readline();
			exp();
		}
		else errortype(6);// ȱ�ٸ�ֵ����� 
	}
	else if (unit.value == "if" && unit.key == "keyword")
	{// if <lexp> then <statement> [else <statement>]
		Readline();
		lexp();
		if (unit.key == "int") Readline();
		if ((unit.value == "then" && unit.key == "keyword") || *errorType == 7)
		{
			if (*errorType == 7) errorType++;
			else Readline();
			statement();
			if (unit.value == "else" && unit.key == "keyword")
			{
				Readline();
				statement();
			}
		}
		else errortype(7); //ȱ��then 
	}
	else if (unit.value == "while" && unit.key == "keyword")
	{// while <lexp> do <statement>
		Readline();

		lexp();
		if ((unit.value == "do" && unit.key == "keyword") || *errorType == 8)
		{
			if (*errorType == 8) errorType++;
			else Readline();
			statement();
		}
		else errortype(8);//ȱ��do 
	}
	else if (unit.value == "call" && unit.key == "keyword")
	{// call <id> ([<exp>{,<exp>}])
		Readline();
		if (unit.key == "id" || *errorType == 9)
		{
			if (*errorType == 9) errorType++;
			else Readline();
			if (unit.value == "(" && unit.key == "...")
			{
				Readline();
				exp();
				while (unit.key == "..." && unit.value == ",")
				{
					Readline();
					exp();
				}
				if ((unit.key == "..." && unit.value == ")") || *errorType == 10)
				{
					if (*errorType == 10) errorType++;
					else Readline();
				}
				else errortype(10);
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
				if (*errorType == 11) errorType++;
				else Readline();
				while (unit.value == "," && unit.key == "...")
				{
					Readline();
					if (unit.key == "id") Readline();
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
			while (unit.value == "," && unit.key == "...")
			{
				Readline();
				exp();
			}
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
				return ;
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
int GA()
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
	return 0;
} 
