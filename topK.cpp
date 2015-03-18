/*************************************************************************
	> File Name: topK.cpp
	> Author: zxx
	> Mail: zxx178239@163.com 
	> Created Time: Wed 18 Mar 2015 07:40:14 PM CST
 ************************************************************************/

#include "topK.h"

void TopK::buildHeap(SIPVEC vec, SIMAP wordMsg, int k)
{
	std::map<std::string, int>::iterator it = wordMsg.begin();
	int index = 0;
	for(; it != wordMsg.end() && index < k; ++ it, ++ index)
	{
		heapify(vec, *it, )
	}
}

void TopK::heapify(SIPVEC vec, std::pair<std::string, int> cur, int len)
{
	
}

void TopK::getTopK(SIPVEC vec, std::map<std::string, int> wordMsg, int k)
{

}
