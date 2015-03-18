#pragma once

#include <vector>
#include <pair>

typedef std::vector<std::pair<std::string, int> > SIPVEC;

class TopK
{
public:
	//初始化堆，建立十个对象的数组
	void buildHeap(SIPVEC vec, SIMAP wordMsg, int k);
	//调整堆结构
	void heapify(SIPVEC vec, std::pair<std::string, int> cur, int len);
	//便历获取前k个
	void getTopK(SIPVEC vec, std::map<std::string, int> wordMsg, int k);
};

