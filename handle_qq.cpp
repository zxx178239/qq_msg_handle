/*************************************************************************
	> File Name: handle_qq.cpp
	> Author: zxx
	> Mail: zxx178239@163.com 
	> Created Time: Sun 15 Mar 2015 09:10:33 PM CST
 ************************************************************************/

#include "handle_qq.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <algorithm>
#include "seg.h"

void Handleqq::saveTxtToMap(std::string filename)
{
	std::ifstream ifs(filename.c_str());
	//判断是否打开成功
	if(!ifs.good())
	{
		std::cout << "open file failed" << std::endl;
		return;
	}
	std::string head = "";
	std::string txt = "";
	std::string qqNameNum = "";
	std::string data = "";
	std::string time = "";
	std::string qqNum = "";
	while(getline(ifs, head))
	{
		//判空
		if(head == "")
			continue;
		//提取qq号与qq密码
		int len = head.size();
		int index = head.find_last_of(' ');
		//判断是否匹配到
		if(index != std::string::npos)
		{
			qqNameNum = head.substr(index + 1);
			std::istringstream iss(head);
			iss >> data >> time;
		}
		//将qq号与qq昵称分开
		saveQQNameNumToMap(qqNameNum, qqNum);
		//去掉那些上传的文件
		if(qqNum == "10000")
		{
			while(getline(ifs, txt) && txt != "")
				;
			head = qqNum = qqNameNum = txt = "";
			continue;
		}
		//接受输入的信息
		std::string tmp = "";
		while(getline(ifs, txt) && txt != "")
		{
			tmp += txt;
			txt = "";
		}
		//当该用户输入的内容为空时进行continue
		if(tmp == "")
			continue;
		txt = data + " " + time + " " + tmp;
		//判断若已经存在该用户，加入vector中，否则创建vector
		if(txtMap.find(qqNum) != txtMap.end())
		{
			(txtMap[qqNum]).push_back(txt);
		}else
		{
			std::vector<std::string> svec;
			svec.push_back(txt);
			txtMap.insert(std::make_pair(qqNum, svec));
		}
		head = qqNum = qqNameNum = txt = "";
	}
	ifs.close();
	saveStopToMap(STOPFILENAME);
//	printStopWords();
}
//测试使用，无任何用处
void Handleqq::printMapToTxt()
{
	SVSMAP::iterator it = txtMap.begin();
	std::ofstream newofs("save.txt", std::ios::trunc);
	for(; it != txtMap.end(); ++ it)
	{
		newofs << it->first << std::endl;
		for(int index = 0; index < (it->second).size(); ++ index)
		{
			newofs << (it->second)[index] << std::endl;
		}
		newofs << std::endl;
	}
	newofs.close();
}

void Handleqq::saveQQNameNumToMap(std::string nameNumMerge, std::string &qqNum)
{
	std::string qqName;
	//找到(开始的位置
	int index = nameNumMerge.find('(');
	//有些qq号是邮箱，所以是用<>括起来的
	if(index == std::string::npos)
		index = nameNumMerge.find('<');
	int len = nameNumMerge.size();
	qqName = nameNumMerge.substr(0, index);
	qqNum = nameNumMerge.substr(index + 1, len - qqName.size() - 2 );
	if(qqMap.find(qqNum) == qqMap.end())
	{
		std::set<std::string> sset;
		sset.insert(qqName);
		qqMap.insert(std::make_pair(qqNum, sset));
	}else
	{
		(qqMap[qqNum]).insert(qqName);
	}
}

//下面代码中的四行代码用于统计出现最多的k个词频需要，不需要的话可以删除
void Handleqq::saveEveToTxt()
{
	SVSMAP::iterator it = txtMap.begin();
	mkdir(DIRNAME.c_str(), 0775);
	std::string data;
	std::string data_cpy;
	std::string other_txt;
	for(; it != txtMap.end(); ++ it)
	{
		eveWords.clear();

		//添加判断语句，当该人说的话少于十句的话不进行统计
		int allSize = (it->second).size();
		if(allSize < 10)
		{
			continue;
		}

		std::string fileName;
		fileName += DIRNAME;
		getFilename(it->first, fileName);
		std::ofstream ofs(fileName.c_str(), std::ios::trunc);
		if(!ofs.good())
		{
			std::cout << "file create failed" << std::endl;
			return ;
		}
		//一个帐号一个帐号存储
		for(int index = 0; index < allSize; ++ index)
		{

			std::istringstream iss((it->second)[index]);
			data = "";
			//从字符串中截取data
			iss >> data;
			ofs << data << std::endl;
			ofs << "================================" << std::endl;
			other_txt = ((it->second)[index]).substr(data.size() + 1);
			ofs << other_txt << std::endl;
			//添加的功能,用于统计词频1
			countWords( other_txt);
			//处理当天全部的发送消息
			while(++ index && index < (it->second).size())
			{
				std::istringstream iss((it->second)[index]);
				data_cpy = "";
				iss >> data_cpy;
				if(data_cpy != data)
					break;
				other_txt = ((it->second)[index]).substr(data_cpy.size() + 1);
				ofs << other_txt << std::endl;
				//添加的功能,用于统计词频2
				countWords(other_txt);
			}
			ofs << std::endl;
		}
		ofs.close();
		//添加的功能,用于统计词频3
		allPeopleWords.insert(std::make_pair(it->first, eveWords));
		//添加的功能,用于统计词频4
		printTopTen(fileName, it->first);
	}
}

void Handleqq::getFilename(const std::string num, std::string &filename)
{
	filename += num;
	std::set<std::string>::iterator it = (qqMap[num]).begin();
	for(; it != (qqMap[num]).end(); ++ it)
	{
		filename.append("_");
		filename.append(*it);
	}
}

void Handleqq::countWords(std::string sentence)
{
    MixSegment seg(JIEBA_DICT_FILE, HMM_DICT_FILE, USER_DICT_FILE);
	std::vector<std::string> word;
	//由于接受的句子是时间+语句的形式，所以还需要处理一下
	int index = sentence.find_first_of(' ');
	std::string msg = sentence.substr(index + 1);

	if(msg == "")
		return;

	seg.cut(msg, word);
	std::vector<std::string>::iterator it_word = word.begin();
	for(; it_word != word.end(); ++ it_word)
	{
		if(*it_word == " ")
			continue;
		if(isUseWords(*it_word) == false)
		{//判断当前的为废话
			continue;
		}else
		{
			if(eveWords.find(*it_word) == eveWords.end())
			{//没有找到单词
				eveWords.insert(std::make_pair(*it_word, 1));
			}else
			{//找到单词
				eveWords[*it_word] ++;
			}
		}
	}
}

void Handleqq::printTopTen(std::string filename, std::string qqNum)
{
	//以补充的方式打开文件
	std::ofstream ofs(filename.c_str(), std::ios::app);
	if(!ofs.good())
	{
		std::cout << "file couldn't open" << std::endl;
		return;
	}
	std::multimap<int, std::string> isMap;
	std::map<std::string, int> siMap((allPeopleWords.find(qqNum))->second);
	std::map<std::string, int>::iterator it_old = siMap.begin();
	for(; it_old != siMap.end(); ++ it_old)
	{
		isMap.insert(std::make_pair(it_old->second, it_old->first));
	}
	//打印出该用户经常使用的前10的单词
	std::multimap<int, std::string>::reverse_iterator it = isMap.rbegin();
	int index = 0;
	for(; it != isMap.rend() && index < 10; ++ it, ++ index)
	{
		ofs << it->second << " " << it->first << std::endl;
	}
}

bool Handleqq::isUseWords(std::string words)
{
	int index;
	std::vector<std::string>::iterator it = stopWords.begin();
	for(; it != stopWords.end(); ++ it)
	{
		if(*it == words)
		{
			return false;
		}
	}
	return true;
}

void Handleqq::saveStopToMap(std::string filename)
{
	std::ifstream ifs(filename.c_str());
	if(!ifs.good())
	{
		std::cout << "stop file open fail" << std::endl;
		return;
	}
	std::string line;
	while(getline(ifs, line))
	{
		std::istringstream iss(line);
		std::string str;
		iss >> str;
		stopWords.push_back(str);
	}
}

void Handleqq::printStopWords()
{
	std::vector<std::string>::iterator it = stopWords.begin();
	for(; it != stopWords.end(); ++ it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}
