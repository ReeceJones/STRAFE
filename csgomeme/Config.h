#pragma once
#include <ostream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <fileapi.h>

using namespace std;

class ConfigVector //because std::vector doesnt allow 3 contructors we use a ghetto fix by creating a class like this
{
public:
	ConfigVector(float v1, float v2, float v3) { this->f1 = v1; this->f2 = v2; this->f3 = v3; };
	~ConfigVector() {};
	float f1;
	float f2;
	float f3;
};

class CConfig
{
public:
	/* Handle configs */
	void InitializeConfigSystem();

	void CreateConfig(char Path[MAX_PATH], char FileName[MAX_PATH]);

	void DeleteConfig(char Path[MAX_PATH]);

	HANDLE OpenConfig(char Name[MAX_PATH], OFSTRUCT &ofstruct, UINT Action);

	void WriteCharToConfig(HANDLE file, const char *StringToWrite);

	std::vector<std::string> ReadConfig(char Path[MAX_PATH]); //return all data of file in one cstr

	void WriteBufferToFile(char Path[MAX_PATH], char FileName[MAX_PATH]);

	std::vector<std::string> GetAllConfigs(std::string Path);

	void LoadConfig(char Path[MAX_PATH]);

	/* Parse values from file */
	int ParseInt(std::string StringToParse);
	bool ParseBool(std::string StringToParse);
	float ParseFloat(std::string StringToParse);
	std::string ParseString(std::string StringToParse);
	ConfigVector ParseFloat3(std::string StringToParse);

	/* Write values to file */
	void WriteInt(std::string Name, int Value);
	void WriteBool(std::string Name, bool Value);
	void WriteFloat(std::string Name, float Value);
	void WriteString(std::string Name, std::string Value);
	void WriteFloat3(std::string Name, float Value[3]);

private:
	vector<string> Configs;
	std::string Buffer;
}; extern CConfig g_Config;