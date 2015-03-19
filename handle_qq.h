#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

const std::string FILENAME = "./王道.txt";//"./王道.txt";
//用来存储导出来的个人信息的文件夹
const std::string DIRNAME = "./王道/";
const std::string STOPFILENAME = "./stop.txt";
typedef std::map<std::string, std::vector<std::string> > SVSMAP;
typedef std::map<std::string, std::set<std::string> > SSSMAP;
typedef std::map<std::string, std::map<std::string, int> > SMSIMAP;

//过滤掉这些词
//const std::string ignoreWords[] = {" ", "[", "]", "(", ")", "的", "阿", "呵呵", "哦", "奥", "好的", "好吧", "...", "......", "。", "，", ",", ".", "!"};

class Handleqq
{
public:
	//将文本内容保存至map中
	void saveTxtToMap(std::string filename);
	//将每个人的信息单独保存成文件
	void saveEveToTxt();
	//测试使用
	void printStopWords();
	void printMapToTxt();
private:
	//创建文件名字
	void getFilename(const std::string num, std::string &filename);
	//将qq号与qq昵称分开存储，并用形参返回qq号
	void saveQQNameNumToMap(std::string nameNumMerge, std::string &qqName);
	//得到所有单词的统计结果
	void countWords(std::string sentence);
	//打印前十
	void printTopTen(std::string filename, std::string qqNum);
	//判断是否为废话
	bool isUseWords(std::string words);
	//读取stop.txt中的内容存储在map中
	void saveStopToMap(std::string filename);
public:
	//保存文件中的内容
	SVSMAP txtMap;
	//保存qq号与qq昵称
	SSSMAP qqMap;
	//存储个人的词频信息
	SMSIMAP allPeopleWords;
	std::map<std::string, int> eveWords;
	//存储停用词的数据
	std::vector<std::string> stopWords;
};
