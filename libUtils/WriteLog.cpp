#include <stdio.h>
#include "WriteLog.h"

void CWriteLogFile::WriteLog (const char* pszText)
{
	if (!pszText || *pszText == '\0')
		pszText = m_sText.c_str ();

	printf ("\n%s", pszText);

	m_sText = "";
}

void CWriteLogFile::WriteLog (const std::string& str)
{
	WriteLog (str.c_str ());
}

CWriteLogFile& CWriteLogFile::operator << (const char* pszText)
{
	m_sText += pszText;
	return *this;
}

CWriteLogFile& CWriteLogFile::operator << (const std::string& sText)
{
	return operator <<(sText.c_str ());
}

CWriteLogFile& CWriteLogFile::operator << (int nNumber)
{
	char aszBuf[32];
	sprintf_s (aszBuf,"%d", nNumber);
	return operator << (aszBuf);
}

void CWriteLogFile::Write ()
{
	WriteLog (m_sText.c_str ());
}
