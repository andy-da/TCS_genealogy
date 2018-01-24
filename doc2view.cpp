// Read the TCS Genealogical Database file and display its tree structure
// using indentation, with names ordered by Ph.D. date.
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <set>
#include "earlier.h"
using namespace std;

typedef string Name;
typedef string Year;
typedef string Place;
typedef map<string, string> PropertyMap;
typedef multiset<string, earlier> DateOrderedMset;
typedef map<string, DateOrderedMset> AcademicRelation;
typedef DateOrderedMset::iterator IterMset;
typedef AcademicRelation::iterator IterRel;

// �������
void welcome()
{
	cout << "************ ��״��ʾSIGACT ���ۼ������ѧ����ϵͼ*************\n";
}


// ��ȡ����Դ�ļ��������ifs����
void getDataSource(ifstream& ifs)
{
	// Դ�����ļ�����
	string file_name;
	// �����ļ�����
	cout << "����������������ݵ��ļ�����: " ;
	cin >> file_name;
	// ���ļ�
	ifs.open(file_name.c_str());
	// �����ʧ�ܣ������쳣���˳�
	if (!ifs.is_open()) 
	{
		cout << "�쳣���ļ�"<< file_name << "û���ҵ� " 
			<< endl;
		exit(1);
	}
}

// �������ݹ�ϵģ��
void modelView(AcademicRelation& advisors, 
			   AcademicRelation& students,
			   ifstream& ifs)
{
	string name, advisor, place, year;
	// ��ȡ����Դ���������ʽṹ
	while(ifs)
		// ���δ���ļ�ĩβ
	{
		// ����ע��
		if (ifs.peek() == '#')
		{
			getline(ifs,name,'\n');
			continue;
		}
		// ���ж�ȡ4����
		getline(ifs, name, '\t');
		getline(ifs, advisor, '\t');
		getline(ifs, place, '\t');
		getline(ifs, year ,'\n');
			// һ�н����������Ϊ���з�
		// ���첩ʿ��ҵʱ�䡢�ص����Խṹ
		earlier::placeProp[name] = place;
		earlier::dateProp[name] = year;
		// ��ȷ���ĵ�ʦ��Ϣͳһ��"---"��ʾ
		if (advisor == "?")
			advisor = "---";
		// ���쵼ʦ--ѧ����ϵ�ṹ
		students[advisor].insert(name);
		advisors[name].insert(advisor);
	}
	// �ر�Դ�����ļ�
	ifs.close();
}

// ��������ɭ�ֵĸ��ڵ㼯��
// û�е�ʦ��Ϣ�Ŀ�ѧ��Ϊ���ڵ�
DateOrderedMset& setupRootSet( AcademicRelation& advisors, 
							   AcademicRelation& students)
{
	// ��ʼ�����ڵ㼯��Ϊ��ʦ��ϢΪ"---"��ѧ��
	DateOrderedMset& roots = students["---"];
	// ���������ʦӳ��ĵ�����
	IterRel iter_rel;
	// �����������ѧ����Ӧ��ʦ���ϵĵ�����
	IterMset iter_set; 
	// �Ƿ���ڵ�ʦ��Ϣ��
	bool existAdvisor;
	// ������ʦӳ��
	for (iter_rel = advisors.begin(); iter_rel != advisors.end(); ++iter_rel) 
	{
		existAdvisor = false;
		// ��ǰѧ���ĵ�ʦ����������ʼ������
		IterMset iter_set_b = iter_rel->second.begin(); 
		// ��ǰѧ���ĵ�ʦ������������������
		IterMset iter_set_e = iter_rel->second.end(); 
		for (iter_set = iter_set_b; iter_set != iter_set_e; ++iter_set)
		{
			// �����ѧ���ĵ�ʦΪ---��δ֪�������ʼ��ʱ�Ѿ��ڸ�������
			// �����ѧ���ĵ�ʦ�����ݿ��в��ҳ��������ѧ���϶������ڸ�����
			// ������������£���existAdvisor��Ϊtrue�������������
			if (*iter_set == string("---") || 
				advisors.find(*iter_set) != advisors.end())
				existAdvisor = true;
		}
		// ��������ڵ�ʦ����Ϣ��
		if (!existAdvisor) 
		{
			string first_advisor = *(iter_rel->second.begin());
			// ������Ƿ��Ѿ������ڸ�������:
			//   ��Ϊmultiset roots������name��������������
			//   ������find����������ʱ�临�Ӷ�
			//   ���ڵ�ǰ�����ģ���ⲻ��һ���鷳 
			if ( find(roots.begin(), roots.end(), first_advisor)
				 == roots.end())
			roots.insert(first_advisor);
				// ������ڸ������У�����������
		}
	}
	return roots;
}

// ��ӡ��root_nameΪ����ѧ����
// �Ժ��ʵ�������ʽ��ʾʦ�й�ϵ
// ��ʾ�������������к��ʦ��
// �ݹ����
void tree_view(const string& root_name, 
			   AcademicRelation& students,
			   PropertyMap& placeProp, 
			   PropertyMap& dateProp,
			   ostream& out,
			   int indentation_level = 0)
{
	// �������������ʾ���ʵ���������
	for (int k = 0; k != indentation_level; ++k)
		out << "      ";
	// ��ӡѧ���Ĳ�ʿѧλ��Ϣ
	// ��ʽ�磺 Brian Kernighan (Princeton 1969��
	out << root_name << " (" << placeProp[root_name] << " " 
		<< dateProp[root_name]<< ")" << endl;
	DateOrderedMset& L = students[root_name];
	DateOrderedMset::const_iterator j;
	// ��ӡ�ÿ�ѧ�ҵ�����ѧ����Ϣ
	// ������������ѧ������
	// ��������˳�򣬲�����һ������
	for (j = L.begin(); j != L.end(); ++j)
		// �ݹ����
		tree_view (*j, students, placeProp, dateProp, out,
					indentation_level+1);
}

// ��ʾ���ڵ㼯�ϴ���ĸ�����
void forest_view(const DateOrderedMset& roots,
				 AcademicRelation& students)
{
	DateOrderedMset::const_iterator iter;
	//�������ڵ㼯�ϣ������������Ӧ��
	cout << "��ѡ�����SIGACT ���ۼ������ѧ����ϵͼ" << endl
		<< "1. �������Ļ\n" << "2. ������ļ�\n"
		<< "������1��2ѡ��";
	int selector;
	cin >> selector;
	if (selector == 1)
		// �������Ļ
		for (iter = roots.begin(); iter != roots.end(); ++iter)	
		{
			tree_view(*iter, students, earlier::placeProp, earlier::dateProp, cout);
			system("pause");
			cout << endl;
		}
	else
		//������ļ�
	{
		cout << "���������ļ���" << endl;
		string viewName;
		cin >> viewName;
		ofstream ofs(viewName.c_str());
		cout << "�ļ�������..." << endl;
		for (iter = roots.begin(); iter != roots.end(); ++iter)
			// ������ļ�
			tree_view(*iter, students, earlier::placeProp, earlier::dateProp, ofs);
		// �ر��ļ�
		ofs.close();
		cout << "�ļ������ɹ���" << endl;
	}	
}


int main()
{
	// ���������棬˵��������
	welcome();
	// ��ȡ����Դ�ļ����
	ifstream ifs;
	getDataSource(ifs);	 
	// ����ʦ����ϵӳ�����
	AcademicRelation advisors, students;
	// ������Դ��������ѧ�ҹ�ϵ������
	modelView(advisors,students,ifs);
	// ��ʼ�����ڵ㼯��Ϊ��ʦ��ϢΪ"---"��ѧ��
	DateOrderedMset roots = setupRootSet(advisors,students);
	// ��ʾ�����������
	forest_view(roots,students);
	return 0;
}
