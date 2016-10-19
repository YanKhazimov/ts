#pragma once
#include <string>

class CWriteLogFile
{
public:
	void WriteLog (const char* pszText);

	void WriteLog (const std::string& str);

	CWriteLogFile& operator << (const char* pszText);
	CWriteLogFile& operator << (const std::string& sText);
	CWriteLogFile& operator << (int nNumber);
	void Write ();

private:
	std::string m_sText;
};
