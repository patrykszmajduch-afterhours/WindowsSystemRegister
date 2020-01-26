#pragma once
#include "CRejestr.h"

#include <atltypes.h>

#pragma region Przechowywanie po³o¿enia i rozmiaru okna
bool CzytajPolozenieIWielkoscOknaZRejestru(HWND uchwytOkna, const wchar_t* klucz)
{
	bool wynik = false;
	int left, top, width, height;
	CRejestr rejestr(HKEY_CURRENT_USER, KEY_READ, true);
	if (!rejestr.CzyKluczIstnieje(klucz)) return false;
	if (rejestr.OtworzKlucz(klucz))
	{
		try
		{
			left = rejestr.CzytajInt(L"Left");
			top = rejestr.CzytajInt(L"Top");
			width = rejestr.CzytajInt(L"Width");
			height = rejestr.CzytajInt(L"Height");
			MoveWindow(uchwytOkna, left, top, width, height, TRUE);
			wynik = true;
		}
		catch (...)
		{
			MessageBox(NULL, L"Odczytanie po³o¿enia i wartoœci okna nie powiod³o siê", L"B³¹d podczas odczytu po³o¿enia okna z rejestru", MB_OK);
			wynik = false;
		}
		rejestr.ZamknijKlucz();
	}
	return wynik;
}
void ZapiszPolozenieIWielkoscOknaWRejestrze(HWND uchwytOkna, const wchar_t* klucz)
{
	CRejestr rejestr(HKEY_CURRENT_USER, KEY_WRITE, true);
	if (!rejestr.CzyKluczIstnieje(klucz)) rejestr.UtworzKlucz(klucz);
	if (rejestr.OtworzKlucz(klucz))
	{
		CRect rect;
		GetWindowRect(uchwytOkna, &rect);
		rejestr.ZapiszInt(L"Left", rect.left);
		rejestr.ZapiszInt(L"Top", rect.top);
		rejestr.ZapiszInt(L"Width", rect.Width());
		rejestr.ZapiszInt(L"Height", rect.Height());
		rejestr.ZamknijKlucz();
	}
}
#pragma endregion
#pragma region Wyœwietlanie kluczy g³ównych

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey, CTreeCtrl &mainTree, HTREEITEM toAdd,CListCtrl &listValue)
{
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i, retCode;

    TCHAR  achValue[MAX_VALUE_NAME];
    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 

    // Enumerate the subkeys, until RegEnumKeyEx fails.

    if (cSubKeys)//
    {
        
        OutputDebugStringW(L"\nNumber of subkeys:" +cSubKeys);
        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS)
            {
                mainTree.InsertItem(achKey, toAdd);
                _tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
            }
        }
    }

    // Enumerate the key values. 

    if (cValues)
    {
        listValue.DeleteAllItems();
        //printf("\nNumber of values: %d\n", cValues);
        OutputDebugStringW(L"\nNumber of values:" + cValues);

        DWORD valueType;
        DWORD cbData;

        


        for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
        {




            DWORD valueType;
            DWORD cbData = cbMaxValueData + 1;//must be plus one cause '\0' one the end
            TCHAR byteArr[MAX_VALUE_NAME];

            for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
            {


                cchValue = MAX_VALUE_NAME;
                achValue[0] = '\0';
                retCode = RegEnumValue(hKey, i,
                    achValue,
                    &cchValue,
                    0,
                    &valueType,
                    (LPBYTE)byteArr,
                    &cbData);




                if (retCode == ERROR_SUCCESS)
                {
                    CString valueTypeString;
                    valueTypeString.Format(L"%u", valueType);
                    int nItem;
                    if (achValue[0] == '\0')
                    {
                        nItem = listValue.InsertItem(0, L"(Domyslna)");
                    }
                    else {
                        nItem = listValue.InsertItem(0, achValue);
                    }
                    listValue.SetItemText(nItem, 1, valueTypeString);
                                     

                    if (valueType == REG_SZ || valueType == REG_EXPAND_SZ)
                    {
                        listValue.SetItemText(nItem, 1,L"REG_SZ");
                       listValue.SetItemText(nItem,2, byteArr);
                    }
                   
                    else if (valueType == REG_DWORD || valueType == REG_DWORD_BIG_ENDIAN || valueType == REG_DWORD_LITTLE_ENDIAN)
                    {

                    int tablica;
                    DWORD rozmiar;

                    LONG numerBledu = RegQueryValueEx(hKey, achValue, NULL, NULL, (LPBYTE)&tablica, &rozmiar);
                                       
                    CString dataString;
                    dataString.Format(L"%d", tablica);
                    listValue.SetItemText(nItem, 1,L"REG_DWORD");
                    listValue.SetItemText(nItem, 2, dataString);
                    }
                    else if (valueType == REG_BINARY)
                    {
                        listValue.SetItemText(nItem, 1, L"REG_BINARY");
                    }
                    else if (valueType == REG_QWORD)
                    {
                        CString dataString;
                        dataString.Format(L"%u", byteArr);
                        listValue.SetItemText(nItem, 1, L"REG_QWORD");
                        listValue.SetItemText(nItem, 2, dataString);
                    }
                }

            }
            

        }
    }
    
}

#pragma endregion Wyœwietlanie kluczy g³ównych
#pragma region autostart

bool CzyIstniejeZapisAutostart(HKEY kluczGlowny, const wchar_t* nazwa);
bool ZapiszAutostart(HKEY kluczGlowny, const wchar_t* nazwa, const wchar_t* sciezkaPliku);
bool UsunAutostart(HKEY kluczGlowny, const wchar_t* nazwa);

#pragma endregion
#pragma region dodajRejestr

BOOL RegDelnode(HKEY hKeyRoot, LPCTSTR lpSubKey);
BOOL RegAddnode(HKEY hKeyRoot, LPCTSTR lpSubKey);

#pragma endregion