#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <type_traits>
#include <fstream>
#include <cstdlib>
#include "XMLserialize.h"
#include "BinarySerialize.hpp"
using namespace std;
using namespace XMLserialize;

void string_test() {
	string k1 = "test";
	string k2;
	serialize_xml(k1, "string_test.xml");
	deserialize_xml(k2, "string_test.xml");
	cout << k2 << endl;
}
void arithmetic_test() {
	int k1 = 100;
	int k2;
	serialize_xml(k1, "arithmetic_test.xml");
	deserialize_xml(k2, "arithmetic_test.xml");
	cout << k2 << endl;
}
void pair_test() {
	pair<int, double> k1(2, 3.1);
	pair<int, double> k2;
	serialize_xml(k1, "pair_test.xml");
	deserialize_xml(k2, "pair_test.xml");
	cout << k2.first << endl;
	cout << k2.second << endl;
}
void vector_test() {
	vector<string> k1({ "aaa","bbb","ccc","ddd" });
	vector<string> k2;
	serialize_xml(k1, "vector_test.xml");
	deserialize_xml(k2, "vector_test.xml");
	for (int i = 0; i < k2.size(); i++) {
		cout << k2[i] << endl;
	}
}
void list_test() {
	list<int> k1(5, 1);
	list<int> k2;
	serialize_xml(k1, "list_test.xml");
	deserialize_xml(k2, "list_test.xml");
	list<int>::iterator i;
	for (i = k2.begin(); i != k2.end(); i++) {
		cout << (*i) << endl;
	}
}
void set_test() {
	set<int> k1, k2;
	k1.insert(3);
	k1.insert(2);
	k1.insert(4);
	k1.insert(1);
	serialize_xml(k1, "set_test.xml");
	deserialize_xml(k2, "set_test.xml");
	set<int>::iterator i;
	for (i = k2.begin(); i != k2.end(); i++) {
		cout << (*i) << endl;
	}
}
void map_test() {
	std::map<int, string> k1, k2;
	k1.insert(std::pair<int, string>(3, "test1"));
	k1.insert(std::pair<int, string>(2, "test2"));
	k1.insert(std::pair<int, string>(4, "test3"));
	k1.insert(std::pair<int, string>(1, "test4"));

	serialize_xml(k1, "map_test.xml");
	deserialize_xml(k2, "map_test.xml");
	map<int, string>::iterator i;
	for (i = k2.begin(); i != k2.end(); i++) {
		cout << (*i).first << endl;
		cout << (*i).second << endl;
	}
}
void user_defined_test() {
	//�û��Զ���ṹ��Ҫ�ڽṹ���ڴ������л��ͷ����л����������ں����ڽ�ÿ����Ա���л�/�����л�����
	//����ʱֻ������xml�ļ�·������
	struct user_define
	{
		int a;
		string b;
		vector<double> c;
		void serialize(const char* filepath) {
			serialize_xml(a, filepath);
			serialize_xml(b, filepath);
			serialize_xml(c, filepath);
		}
		void desrialize(const char* filepath) {
			deserialize_xml(a, filepath);
			deserialize_xml(b, filepath);
			deserialize_xml(c, filepath);
		}
	};
	struct user_define k1, k2;
	k1.a = 1;
	k1.b = "test";
	k1.c.push_back(3.14);
	k1.c.push_back(2.001);
	k1.serialize("user_test.xml");
	k2.desrialize("user_test.xml");
	cout << "k2.a=" << k2.a << endl;
	cout << "k2.b=" << k2.b << endl;
	for (int i = 0; i < k2.c.size(); i++) {
		cout << "k2.c[" << i + 1 << "]=" << k2.c[i] << endl;
	}
}


void stringtest_bin()
{
	string n0 = "test";
	string n1;
	serialize(n0, "stringtest.data");
	deserialize(n1, "stringtest.data");
	cout << n1 << endl;
}
void arithmetictest_bin()
{
	int n0 = 456;
	int n1;
	serialize(n0, "arithmetictest.data");
	deserialize(n1, "arithmetictest.data");
	cout << n1 << endl;
}
void pairtest_bin() {
	pair<int, double> k1(2, 3.1);
	pair<int, double> k2;
	serialize(k1, "pairtest.data");
	deserialize(k2, "pairtest.data");
	cout << k2.first << endl;
	cout << k2.second << endl;
}
void vectortest_bin() {
	vector<string> k1({ "aaa","bbb","ccc","ddd" });
	vector<string> k2;
	serialize(k1, "vectortest.data");
	deserialize(k2, "vectortest.data");
	for (int i = 0; i < k2.size(); i++) {
		cout << k2[i] << endl;
	}
}
void listtest_bin()
{
	list<int> n0(5, 1), n1;

	// serialize object n0 to a binary file n.data
	serialize(n0, "listtest.data");
	// reconstruct object n1 from the content of n.data
	deserialize(n1, "listtest.data");
	// now n0 == n1 should be true.

	list<int>::iterator it1, it2 = n1.begin();
	int i = 0;
	for (it1 = n0.begin(); it1 != n0.end(); it1++, it2++, i++)
	{
		cout << "n0[" << i << "] = " << (*it1) << endl;
		cout << "n1[" << i << "] = " << (*it2) << endl;
	}
}
void settest()
{
	std::set<int> n0, n1;
	n0.insert(3);
	n0.insert(2);
	n0.insert(4);
	n0.insert(1);

	// serialize object n0 to a binary file n.data
	serialize(n0, "settest.data");
	// reconstruct object n1 from the content of n.data
	deserialize(n1, "settest.data");
	// now n0 == n1 should be true.

	set<int>::iterator it1, it2 = n1.begin();
	int i = 0;
	for (it1 = n0.begin(); it1 != n0.end(); it1++, it2++, i++)
	{
		cout << "n0[" << i << "] = " << (*it1) << endl;
		cout << "n1[" << i << "] = " << (*it2) << endl;
	}
}
void maptest()
{
	std::map<int, string> n0, n1;
	n0.insert(std::pair<int, string>(3, "test1"));
	n0.insert(std::pair<int, string>(2, "test2"));
	n0.insert(std::pair<int, string>(4, "test3"));
	n0.insert(std::pair<int, string>(1, "test4"));

	// serialize object n0 to a binary file n.data
	serialize(n0, "maptest.data");
	// reconstruct object n1 from the content of n.data
	deserialize(n1, "maptest.data");
	// now n0 == n1 should be true.

	map<int, string>::iterator it1, it2;
	int i = 0;
	for (it1 = n0.begin(); it1 != n0.end(); it1++, i++)
	{
		cout << "n0[" << i << "].first = " << (*it1).first << endl;
		cout << "n0[" << i << "].second = " << (*it1).second << endl;
	}
	i = 0;
	for (it2 = n1.begin(); it2 != n1.end(); it2++, i++)
	{
		cout << "n1[" << i << "].first = " << (*it2).first << endl;
		cout << "n1[" << i << "].second = " << (*it2).second << endl;
	}
}
void structTest()
{
	struct Test
	{
		int a;
		int b;
		string c;
		vector<double> d;

		// �Զ���ṹ����Ҫ�ṩһ������������˵������Խ������л�
		void serialize(binseri::BinOutput archive)
		{
			archive.process(2, a, b);
			archive.process(c);
			archive.process(d);
		}
		void serialize(binseri::BinInput archive)
		{
			archive.process(2, &a, &b);
			archive.process(c);
			archive.process(d);
		}
	};

	Test k1, k2;
	k1.a = 1;
	k1.b = 2;
	k1.c = "test";
	k1.d.push_back(3.14);
	k1.d.push_back(2.001);

	serialize(k1, "structtest.data");
	deserialize(k2, "structtest.data");
	cout << "k2.a=" << k2.a << endl;
	cout << "k2.b=" << k2.b << endl;
	cout << "k2.c=" << k2.c << endl;
	for (int i = 0; i < k2.d.size(); i++) {
		cout << "k2.d[" << i + 1 << "]=" << k2.d[i] << endl;
	}
}

int main() {
	cout << "XML���ԣ�" << endl;
	cout << "--------------------" << endl << "string����   " << endl;
	string_test();
	cout << "--------------------" << endl << "arithmetic����   " << endl;
	arithmetic_test();
	cout << "--------------------" << endl << "pair����   " << endl;
	pair_test();
	cout << "--------------------" << endl << "vector����   " << endl;
	vector_test();
	cout << "--------------------" << endl << "list����   " << endl;
	list_test();
	cout << "--------------------" << endl << "set����   " << endl;
	set_test();
	cout << "--------------------" << endl << "map����   " << endl;
	map_test();
	cout << "--------------------" << endl << "�û��Զ���ṹ����   " << endl;
	user_defined_test();
	cout << "--------------------" << endl;

	cout << "\n\n�����Ʋ��ԣ�" << endl;
	cout << "--------------------" << endl << "string����   " << endl;
	stringtest_bin();
	cout << "--------------------" << endl << "arithmetic����   " << endl;
	arithmetictest_bin();
	cout << "--------------------" << endl << "pair����   " << endl;
	pairtest_bin();
	cout << "--------------------" << endl << "vector����   " << endl;
	vectortest_bin();
	cout << "--------------------" << endl << "list����   " << endl;
	listtest_bin();
	cout << "--------------------" << endl << "set����   " << endl;
	settest();
	cout << "--------------------" << endl << "map����   " << endl;
	maptest();
	cout << "--------------------" << endl << "�û��Զ���ṹ����   " << endl;
	structTest();
	cout << "--------------------" << endl;

	system("pause");
	return 0;
}