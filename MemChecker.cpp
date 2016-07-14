#include "MemChecker.h"

MemChecker mch;
bool MemChecker::Ready = false;

void* operator new(size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if (MemChecker::Ready)
		mch.Add(p, file, line);
	return p;
}
void operator delete(void* p)
{
	if (MemChecker::Ready)
		mch.Remove(p);
	free(p);
}
void operator delete[](void* p)
{
	if (MemChecker::Ready)
		mch.Remove(p);
	free(p);
}

MemChecker::MemChecker()
{
	Ready = true;
}
MemChecker::~MemChecker()
{
	Dump();
	Ready = false;
}
void MemChecker::Add(void* pointer, const char* file, long line)
{
	pointer_map_[pointer] = Entry(file, line);
}
void MemChecker::Remove(void* pointer)
{
	auto iter = pointer_map_.find(pointer);
	if (iter != pointer_map_.end())
	{
		pointer_map_.erase(iter);
	}
}
void MemChecker::Dump()
{
	if (!pointer_map_.empty())
	{
		cout << "内存泄露" << endl;
		for (auto iter = pointer_map_.begin(); iter != pointer_map_.end(); ++iter)
		{
			const char* file = iter->second.File();
			long line = iter->second.Line();
			cout << "在" << file << "\t" << line << "出现内存泄露" << endl;
		}
	}
}

