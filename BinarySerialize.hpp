#pragma once
#ifndef __BINSERIALIZE_HPP__
#define __BINSERIALIZE_HPP__

#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <stdarg.h> 

using namespace std;

template<class T>
void serialize(T & value, const string& filepath);

template<class T>
void deserialize(T& value, const string& filepath);

namespace binseri
{

	// 判断数据类型中是否有成员变量的序列化/反序列化方法
	template <typename U, typename S>
	struct has_member {

		template <typename T, void(T::*)(S) = &T::serialize>
		static constexpr bool check(T*) { return true; };

		static constexpr bool check(...) { return false; };

		static constexpr bool ret = check(static_cast<U*>(0));
	};


	class BinOutput
	{
	public:
		// 输出类的构造函数，主要是记录输出文件流
		BinOutput(ostream & stream) : itsStream(stream)
		{ }

		// 多个参数的序列化方法, count为需要序列化参数的个数, 后面的参数类型需一致
		template <class T>
		void process(const int count, const T value, ...)
		{
			// 多个参数将这些参数逐一进行序列化
			va_list ap;
			va_start(ap, value);
			
			process(value);

			for (int i = 0; i < count - 1; i++)
			{
				process(va_arg(ap, const T));
			}

			va_end(ap);
		}

		// 自定义结构类型的序列化方法
		template <class T, typename std::enable_if<has_member<T, BinOutput>::ret>::type* = nullptr>
		void process(T& value)
		{
			value.serialize(*this);
		}

		// arithmetic类型数据的序列化方法
		template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		void process(const T& value)
		{
			// 该类数据可直接转化为二进制文件
			writeBinData((char*)&value, sizeof(T));
		}

		// string类型的序列化方法
		void process(const string& value)
		{
			int size = value.size();
			// 先将字符串的字节长度进行储存
			process(size);
			string tmp = value;
			char * str = &tmp[0];
			// 然后根据该长度储存字符串内容
			writeBinData(str, size);
		}

		// pair容器类型的序列化方法
		template <class T1, class T2>
		void process(const std::pair<T1, T2>& value)
		{
			// 分别对第一个变量和第二个变量进行序列化处理即可
			process(value.first);
			process(value.second);
		}

		template <class T>
		void process(const std::vector<T> & value)
		{
			int size = value.size();
			process(size);

			// 后依次将每个数据序列化
			for (int i=0; i < value.size(); i++)
			{
				process(value[i]);
			}
		}

		// list容器的序列化方法
		template <class T>
		void process(std::list<T> & value)
		{
			int size = value.size();
			process(size);
			typename list<T>::iterator it;
			// 后依次将每个数据序列化
			for (it = value.begin(); it != value.end(); ++it)
			{
				process((*it));
			}
		}

		// set容器的序列化方法
		template <class T>
		void process(std::set<T> & value)
		{
			int size = value.size();
			process(size);
			typename set<T>::iterator it;
			// 后依次将每个数据序列化
			for (it = value.begin(); it != value.end(); ++it)
			{
				process((*it));
			}
		}

		// map容器的序列化方法
		template <class T1, class T2>
		void process(std::map<T1, T2> & value)
		{
			int size = value.size();
			process(size);
			typename map<T1, T2>::iterator it;
			// 后依次将每个数据序列化
			for (it = value.begin(); it != value.end(); ++it)
			{
				process((*it).first);
				process((*it).second);
			}
		}

	private:
		ostream & itsStream;	// 该类的文件输出流

		// 给出变量首地址与其所占的字节长度，将其二进制写入文件流
		void writeBinData(char * value, int size)
		{
			itsStream.write(value, size);
		}
	};

	class BinInput
	{
	public:
		// 构造函数，记录输入文件流
		BinInput(istream & is): itsStream(is)
		{ }

		// 多个参数的反序列化方法, count为需要反序列化参数的个数, 后面的参数类型需一致
		template <class T>
		void process(const int count, T* value, ...)
		{
			// 多个参数将这些参数逐一进行序列化
			va_list ap;
			va_start(ap, value);

			process(*value);

			for (int i = 0; i < count - 1; i++)
			{
				process((*va_arg(ap, T*)));
			}

			va_end(ap);
		}

		// 自定义结构类型的反序列化方法
		template <class T, typename std::enable_if<has_member<T, BinInput>::ret>::type* = nullptr>
		void process(T& value)
		{
			value.serialize((*this));
		}

		// arithmetic类型数据的反序列化方法
		template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		void process(T& value)
		{
			// 该类数据可直接从二进制文件中读取对应字节的数据
			readBinData((char*)&value, sizeof(T));
		}

		// string类型的反序列化方法
		void process(string& value)
		{
			int size = 0;
			// 首先读取字符串的长度
			process(size);
			value.resize(size);
			char * tmp = &value[0];
			// 然后根据该长度读取字符串内容
			readBinData(tmp, size);
		}

		// pair容器类型的反序列化方法
		template <class T1, class T2>
		void process(std::pair<T1, T2>& value)
		{
			// 分别对第一个变量和第二个变量进行反序列化处理即可
			process(value.first);
			process(value.second);
		}

		// vector容器的反序列化方法
		template <class T>
		void process(std::vector<T> & value)
		{
			// 先读取数据的个数
			int size = 0;
			process(size);
			value.clear();
			value.resize(size);

			// 后依次将每个数据反序列化存进容器
			for (int i = 0; i < size; i++)
			{
				process(value[i]);
			}
		}

		// list容器的反序列化方法
		template <class T>
		void process(std::list<T> & value)
		{
			// 先读取数据的个数
			int size = 0;
			process(size);
			value.clear();

			// 后依次将每个数据反序列化存进容器
			for (int i = 0; i < size; i++)
			{
				T element;
				process(element);
				value.push_back(element);
			}
		}

		// set容器的反序列化方法
		template <class T>
		void process(std::set<T> & value)
		{
			// 先读取数据的个数
			int size = 0;
			process(size);
			value.clear();

			// 后依次将每个数据反序列化存进容器
			for (int i = 0; i < size; i++)
			{
				T element;
				process(element);
				value.insert(element);
			}
		}

		// map容器的反序列化方法
		template <class T1, class T2>
		void process(std::map<T1, T2> & value)
		{
			// 先读取数据的个数
			int size = 0;
			process(size);
			value.clear();

			// 后依次将每个数据反序列化存进容器
			for (int i = 0; i < size; i++)
			{
				T1 key;
				T2 val;
				process(key);
				process(val);
				value.insert(std::pair<T1, T2>(key, val));
			}
		}

		void readBinData(char * value, int size)
		{
			itsStream.read(value, size);
		}

	private:
		istream & itsStream;
	};
}


template<class T>
inline void serialize(T & value, const string& filepath)
{
	ofstream os(filepath, ios::binary);

	binseri::BinOutput bin(os);

	bin.process(value);

	os.close();
}

template<class T>
inline void deserialize(T& value, const string& filepath)
{
	ifstream is(filepath, ios::binary);

	binseri::BinInput bin(is);

	bin.process(value);

	is.close();
}

#endif // !__BINSERIALIZE_HPP__