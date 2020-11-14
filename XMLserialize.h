#pragma once
#include<iostream>
#include<string>
#include"tinyxml2.h"
#include<list>
#include<set>
#include<map>
#include<memory>
#include<typeinfo>
using namespace std;
using namespace tinyxml2;
template<class T>
void serialize_xml(T& value, const char* name, const char* filepath);
template<class T>
void deserialize_xml(T& value, const char* name, const char* filepath);
namespace XMLserialize {
	class XMLser {
	public:
		XMLDocument doc;
		//arithmetic类型数据
		template<class T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
		void process(XMLElement* Node, T& value){
			XMLElement* data = doc.NewElement("data");
			data->SetAttribute("val", value);
			Node->InsertEndChild(data);
		}

		//string类型的数据
		void process(XMLElement* Node, string value) {
			XMLElement* data = doc.NewElement("string");
			data->SetAttribute("val",value.data());
			Node->InsertEndChild(data);
		}

		//pair类型
		template <class T1,class T2>
		void process(XMLElement* Node, const std::pair<T1, T2>& value) {
			XMLElement* pair = doc.NewElement("std_pair");
			Node->InsertEndChild(pair);
			XMLElement* first = doc.NewElement("first");
			first->SetAttribute("val", value.first);
			pair->InsertEndChild(first);
			XMLElement* second = doc.NewElement("second");
			second->SetAttribute("val", value.second);
			pair->InsertEndChild(second);
		}

		//vector类型
		template<class T>
		void process(XMLElement* Node, const std::vector<T>& value) {
			XMLElement* temp = doc.NewElement("std_vector");
			Node->InsertEndChild(temp);
			for (int i = 0; i < value.size(); i++) {
				process(temp, value[i]);
			}
		}

		//list 类型
		template<class T>
		void process(XMLElement* Node, std::list<T>& value) {
			XMLElement* temp = doc.NewElement("std_list");
			Node->InsertEndChild(temp);
			typename list<T>::iterator it;
			for (it = value.begin(); it != value.end(); it++) {
				process(temp, (*it));
			}
		}

		//set类型
		template<class T>
		void process(XMLElement* Node, std::set<T>& value) {
			XMLElement* temp = doc.NewElement("std_set");
			Node->InsertEndChild(temp);
			typename set<T>::iterator it;
			for (it = value.begin(); it != value.end(); it++) {
				process(temp, (*it));
			}
		}

		//map类型
		template<class T1,class T2>
		void process(XMLElement* Node, std::map<T1, T2>& value) {
			XMLElement* temp = doc.NewElement("std_map");
			Node->InsertEndChild(temp);
			typename map<T1, T2>::iterator it;
			for (it = value.begin(); it != value.end(); ++it)
			{
				process(temp,(*it).first);
				process(temp,(*it).second);
			}
		}
	};

	class XMLdeser {
		public:
		XMLDocument doc;
		//arithmetic类型数据
		template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
		void process(XMLElement* Node, T& value) {
			const char* temp = Node->FirstChildElement("data")->Attribute("val");
			if (strlen(temp) > 1) {
				if (strstr(temp, ".") != NULL) {
					value = atof(temp);

				}
				else {
					value = atoi(temp);
				}
			}
			else {
				if (isdigit(temp[0])) {
					value = temp[0]-'0';
				}
				else {
					value = temp[0];
				}
			}
		}
		//arithmetic类型的子查询使用
		template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
		void process(XMLElement* target, T& value, const char* childname) {
			const char* temp = target->FirstChildElement(childname)->Attribute("val");
			if (strlen(temp) > 1) {
				if (strstr(temp, ".") != NULL) {
					value = atof(temp);

				}
				else {
					value = atoi(temp);
				}
			}
			else {
				if (isdigit(temp[0])) {
					value = temp[0] - '0';
				}
				else {
					value = temp[0];
				}
			}
		}
		template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type * = nullptr>
		void process(XMLElement* target, T& value,int a) {
			const char* temp = target->Attribute("val");
			if (strlen(temp) > 1) {
				if (strstr(temp, ".") != NULL) {
					value = atof(temp);

				}
				else {
					value = atoi(temp);
				}
			}
			else {
				if (isdigit(temp[0])) {
					value = temp[0] - '0';
				}
				else {
					value = temp[0];
				}
			}
		}

		//string 类型
		void process(XMLElement* Node, string& value) {
			value = Node->FirstChildElement("string")->Attribute("val");
		}
		void process(XMLElement* Node, string& value, const char* childname) {
			value = Node->FirstChildElement(childname)->Attribute("val");
		}
		//string元素查询
		void process(XMLElement* Node, string& value,int a) {
			value = Node->Attribute("val");
		}
		

		//pair 类型
		template<class T1,class T2>
		void process(XMLElement* Node, std::pair<T1, T2>& value) {
			XMLElement* target = Node->FirstChildElement("std_pair");
			process(target, value.first, "first");
			process(target, value.second, "second");
		}

		//vector类型
		template<class T>
		void process(XMLElement* Node, std::vector<T>& value) {
			value.clear();
			XMLElement* target = Node->FirstChildElement("std_vector")->FirstChildElement();
			T temp;
			while (target) {
				process(target, temp,1);
				value.push_back(temp);
				target = target->NextSiblingElement();
			}
		}

		//list 类型
		template<class T>
		void process(XMLElement* Node, std::list<T>& value) {
			value.clear();
			XMLElement* target = Node->FirstChildElement("std_list")->FirstChildElement();
			T temp;
			while (target) {
				process(target, temp,1);
				value.push_back(temp);
				target = target->NextSiblingElement();
			}
		}

		//set 类型
		template<class T>
		void process(XMLElement* Node, std::set<T>& value) {
			value.clear();
			XMLElement* target = Node->FirstChildElement("std_set")->FirstChildElement();
			T temp;
			while (target) {
				process(target, temp,1);
				value.insert(temp);
				target = target->NextSiblingElement();
			}
		}

		//map类型
		template<class T1,class T2>
		void process(XMLElement* Node, std::map<T1, T2>& value) {
			value.clear();
			XMLElement* target = Node->FirstChildElement("std_map")->FirstChildElement();
			T1 key;
			T2 val;
			while (target) {
				process(target, key,1);
				target = target->NextSiblingElement();
				process(target, val,1);
				value.insert(std::pair<T1, T2>(key, val));
				target = target->NextSiblingElement();
			}

		}
	};
}
template<class T>
void serialize_xml(T& value, const char* filepath) {
	XMLserialize::XMLser a;
	//已存在该xml文件，插入即可
	if (a.doc.LoadFile(filepath) == 0) {
		XMLElement* root = a.doc.RootElement();
		a.process(root, value);
		a.doc.SaveFile(filepath);
	}
	else {
		XMLElement* root = a.doc.NewElement("serialization");
		a.doc.InsertEndChild(root);
		a.process(root, value);
		a.doc.SaveFile(filepath);
	}
}

template<class T>
void deserialize_xml(T& value, const char* filepath) {
	XMLserialize::XMLdeser b;
	if (b.doc.LoadFile(filepath) != 0) {
		cout << "加载文件失败！" << endl;
		return;                   
	}
	XMLElement* root = b.doc.RootElement();
	b.process(root, value); 
}