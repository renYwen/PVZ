/* 游戏资源 */


#pragma once
#include<easyx.h>
#include<map>
#include<string>







class Resources
{
public:
	std::map<std::string, IMAGE*>imageRes;
	std::map<std::string, std::string>imagePath;
	
	Resources();
	~Resources();
	
	void Init();

	void Load(std::string name,std::string path,int wid,int hei);

	void Load(std::string name, std::string path);//加载媒体文件

	IMAGE* Fetch(std::string name);

	std::string FindPath(std::string name);
};