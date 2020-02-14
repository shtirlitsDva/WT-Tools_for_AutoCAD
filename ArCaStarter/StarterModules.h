#pragma once

class StarterModules
{
protected:
	CString m_appPath;
public:
	StarterModules(void);
	~StarterModules(void);
	static void LoadModules();
	static bool LoadModule(CString& module, BOOL bForceDynamicLinkerLoad = false);
};
