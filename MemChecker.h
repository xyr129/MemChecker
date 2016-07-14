#ifndef MEMCHECKER_H_
#define MEMCHECKER_H_
#include <iostream>
#include <map>
using namespace std;

//全局操作符重载
void* operator new(size_t size, const char* file, long line);
void operator delete(void* p);
void operator delete[](void* p);


//通过MemChecker类实例化一个对象，程序中每次使用new都会连同文件名
//和行号保存在一个容器，delete则会将对应指针从该容器中删除，
//最后析构时检查容器中是否还有未释放的指针，若有则出现内存泄露

class MemChecker
{
private:
	//定义一个内部类
	class Entry
	{
	public:
		Entry(const char* file = nullptr, long line = 0)
			:file_(file)
			, line_(line)
		{}
		inline const char*File()const
		{
			return file_;
		}
		inline  long Line()const
		{
			return line_;
		}
	private:
		const char* file_;
		long line_;
	};
public:
	MemChecker();
	~MemChecker();
	void Add(void* pointer, const char* file, long line);
	void Remove(void* pointer);
	void Dump();
	static bool Ready;
private:
	map<void*, Entry> pointer_map_;
};

extern MemChecker mch;
	
#endif //!MEMCHECKER_H_