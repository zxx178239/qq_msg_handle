/*************************************************************************
	> File Name: main.cpp
	> Author: zxx
	> Mail: zxx178239@163.com 
	> Created Time: Sun 15 Mar 2015 09:30:34 PM CST
 ************************************************************************/

#include "handle_qq.h"
#include <iostream>

int main(int argc, char *argv[])
{
	Handleqq myHandle;
	myHandle.saveTxtToMap(FILENAME);
//	myHandle.printMapToTxt();
	myHandle.saveEveToTxt();
}
