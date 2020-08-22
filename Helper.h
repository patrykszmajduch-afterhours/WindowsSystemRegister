#pragma once
#include "pch.h"
#include <map>
#include <string>

static class Helper
{
public:
	static std::map <CString, HKEY > HRootValue;
	Helper() {
		HRootValue.insert(std::make_pair("HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT));
		HRootValue.insert(std::make_pair("HKEY_CURRENT_USER", HKEY_CURRENT_USER));
		HRootValue.insert(std::make_pair("HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE));
		HRootValue.insert(std::make_pair("HKEY_USERS", HKEY_USERS));
		HRootValue.insert(std::make_pair("HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG));
	}

	
};

