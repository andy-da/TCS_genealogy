// FileName:earlier.h
// �����˺�������earlier�����ڹ���multiset�ṹ
#ifndef  EARLIER_H
#define  EARLIER_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <set>
using namespace std;

typedef map<string, string> PropertyMap;

struct earlier: binary_function<string, string, bool> 
{
	bool operator()(const string& name_lhs,
		const string& name_rhs) const 
	{
		return dateProp[name_lhs] < dateProp[name_rhs];
	}
	// dateProp[name] holds year in which Ph.D. degree was
	// granted to name
	static PropertyMap dateProp;
	// placeProp[name] holds institution that granted Ph.D.
	static PropertyMap placeProp;
};

// ��̬��Ա�����ĳ�ʼ��
PropertyMap earlier::dateProp;
PropertyMap earlier::placeProp;

#endif