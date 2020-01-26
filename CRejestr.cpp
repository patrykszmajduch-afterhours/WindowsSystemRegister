
#include "pch.h"

#include "CRejestr.h"
#include <strsafe.h>

CRejestr::CRejestr(HKEY kluczGlowny, REGSAM trybDostepu, bool wyswietlajKomunikatyOBledach)
	:uchwytKlucza(NULL), kluczGlowny(kluczGlowny),
	trybDostepu(trybDostepu), wyswietlajKomunikatyOBledach(wyswietlajKomunikatyOBledach)
{
}
CRejestr::~CRejestr(void)
{
	if (uchwytKlucza != NULL) ZamknijKlucz();
}
void CRejestr::WyswietlKomunikatBledu(LONG numerBledu) const
{
	if (!wyswietlajKomunikatyOBledach) return;
	wchar_t* komunikat;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, numerBledu, 0, (LPWSTR)&komunikat, 0, NULL);
	MessageBox(NULL, komunikat, L"Błąd podczas operacji na rejestrach", MB_OK | MB_ICONERROR);
	LocalFree(komunikat);
}

bool CRejestr::UtworzKlucz(const wchar_t* klucz)
{
	LONG numerBledu = RegCreateKeyEx(kluczGlowny, klucz, NULL, 0, REG_OPTION_NON_VOLATILE, trybDostepu, NULL, &uchwytKlucza, NULL);
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	return numerBledu == ERROR_SUCCESS;
}
bool CRejestr::OtworzKlucz(const wchar_t* klucz)
{
	if (uchwytKlucza != NULL)
	{
		if (wyswietlajKomunikatyOBledach)
			MessageBox(NULL, L"Przed otwarciem nowego klucza zamknij klucz otwarty wcześniej",
				L"Błąd otwarcia klucza", MB_OK);
		return false;
	}
	LONG numerBledu = RegOpenKeyEx(kluczGlowny, klucz, 0, trybDostepu, &uchwytKlucza);
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	return numerBledu == ERROR_SUCCESS;
}
bool CRejestr::ZamknijKlucz()
{
	LONG numerBledu = RegCloseKey(uchwytKlucza);
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	else uchwytKlucza = NULL;
	return numerBledu == ERROR_SUCCESS;
}
bool CRejestr::UsunKlucz(const wchar_t* klucz)
{
	LONG numerBledu = RegDeleteKey(kluczGlowny, klucz);
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	else klucz = NULL;
	return numerBledu == ERROR_SUCCESS;
}
#pragma endregion

bool CRejestr::CzyKluczIstnieje(const wchar_t* klucz) const
{
	HKEY lokalnyUchwytKlucza;
	bool wynik = RegOpenKeyEx(kluczGlowny, klucz, 0, trybDostepu, &lokalnyUchwytKlucza) == ERROR_SUCCESS;
	RegCloseKey(lokalnyUchwytKlucza);
	return wynik;
}
bool CRejestr::OtworzKlucz(const wchar_t* klucz, bool utworzJezeliNieIstnieje)
{
	if (CzyKluczIstnieje(klucz)) return OtworzKlucz(klucz);
	else
	{
		if (!utworzJezeliNieIstnieje) return false;
		else return UtworzKlucz(klucz);
	}
}

#pragma region Metody dotyczace wartosci
#include <stdexcept>
bool CRejestr::CzyWartoscIstnieje(const wchar_t* nazwa) const
{
	DWORD rozmiar = 1024;
	unsigned char bufor[1024];
	LONG numerBledu = RegQueryValueEx(uchwytKlucza, nazwa, NULL, NULL, bufor, &rozmiar);
	if (numerBledu != ERROR_SUCCESS && numerBledu != ERROR_FILE_NOT_FOUND)
		WyswietlKomunikatBledu(numerBledu);
	return numerBledu == ERROR_SUCCESS;
}
bool CRejestr::UsunWartosc(const wchar_t* nazwa) const
{
	LONG numerBledu = RegDeleteValue(uchwytKlucza, nazwa);
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	return numerBledu == ERROR_SUCCESS;
}
int CRejestr::CzytajInt(const wchar_t* nazwa) const
{
	int wartosc;
	DWORD rozmiar;
	LONG numerBledu = RegQueryValueEx(uchwytKlucza, nazwa, NULL, NULL, (LPBYTE)&wartosc, &rozmiar);
	if (numerBledu != ERROR_SUCCESS)
	{
		WyswietlKomunikatBledu(numerBledu);
		throw std::exception("Odczyt wartości nie powiódł się");
	}
	return wartosc;
}
bool CRejestr::ZapiszInt(const wchar_t* nazwa, int wartosc) const
{
	LONG numerBledu =
		RegSetValueEx(uchwytKlucza, nazwa, 0, REG_DWORD, (LPBYTE)&wartosc, sizeof(wartosc));
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	return numerBledu == ERROR_SUCCESS;
}
wchar_t* CRejestr::CzytajString(const wchar_t* nazwa, wchar_t* wartosc) const
{
	DWORD rozmiar = 1024;
	LONG numerBledu = RegQueryValueEx(uchwytKlucza, nazwa, NULL, NULL, (LPBYTE)wartosc, &rozmiar);
	if (numerBledu != ERROR_SUCCESS)

		WyswietlKomunikatBledu(numerBledu);
	throw std::exception("Odczyt wartości nie powiódł się");

	return wartosc;
}
bool CRejestr::ZapiszString(const wchar_t* nazwa, const wchar_t* wartosc) const
{
	DWORD rozmiar = (wcslen(wartosc) + 1) * sizeof(wchar_t);
	LONG numerBledu = RegSetValueEx(uchwytKlucza, nazwa, 0, REG_SZ, (LPBYTE)wartosc, rozmiar);
	if (numerBledu != ERROR_SUCCESS) WyswietlKomunikatBledu(numerBledu);
	return numerBledu == ERROR_SUCCESS;
}
#pragma endregion
#pragma region autostart

enum CAutostartCzynnosci { acSprawdz = 0, acZapisz = 1, acUsun = 2 };
bool KonfigurujAutostart(HKEY kluczGlowny, const wchar_t* nazwa, const wchar_t* sciezkaPliku, CAutostartCzynnosci czynnosc)
{
	const wchar_t klucz[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	bool wynik = false;
	CRejestr rejestr(kluczGlowny, (czynnosc == acSprawdz) ? KEY_READ : KEY_WRITE);
	if (!rejestr.CzyKluczIstnieje(klucz))
	{
		MessageBox(NULL, L"Klucz nie istnieje", L"Błąd podczas operacji na rejestrach autostartu", MB_ICONERROR);
		return false;
	}
	if (rejestr.OtworzKlucz(klucz))
	{
		switch (czynnosc)
		{
		case acSprawdz: wynik = rejestr.CzyWartoscIstnieje(nazwa); break;
		case acZapisz: wynik = rejestr.ZapiszString(nazwa, sciezkaPliku); break;
		case acUsun: wynik = rejestr.UsunWartosc(nazwa); break;
		}
		rejestr.ZamknijKlucz();
		//if (czynnosc!=acSprawdz) wynik=true;
	}
	return wynik;
}
bool CzyIstniejeZapisAutostart(HKEY kluczGlowny, const wchar_t* nazwa)
{
	return KonfigurujAutostart(kluczGlowny, nazwa, L"", acSprawdz);
}
bool ZapiszAutostart(HKEY kluczGlowny, const wchar_t* nazwa, const wchar_t* sciezkaPliku)
{
	return KonfigurujAutostart(kluczGlowny, nazwa, sciezkaPliku, acZapisz);
}
bool UsunAutostart(HKEY kluczGlowny, const wchar_t* nazwa)
{
	return KonfigurujAutostart(kluczGlowny, nazwa, L"", acUsun);
}
#pragma endregion

BOOL RegDelnodeRecurse(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	LPTSTR lpEnd;
	LONG lResult;
	DWORD dwSize;
	TCHAR szName[MAX_PATH];
	HKEY hKey;
	FILETIME ftWrite;

	// First, see if we can delete the key without having
	// to recurse.

	lResult = RegDeleteKey(hKeyRoot, lpSubKey);

	if (lResult == ERROR_SUCCESS)
		return TRUE;

	lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return TRUE;
		}
		else {
			printf("Error opening key.\n");
			return FALSE;
		}
	}

	// Check for an ending slash and add one if it is missing.

	lpEnd = lpSubKey + lstrlen(lpSubKey);

	if (*(lpEnd - 1) != TEXT('\\'))
	{
		*lpEnd = TEXT('\\');
		lpEnd++;
		*lpEnd = TEXT('\0');
	}

	// Enumerate the keys

	dwSize = MAX_PATH;
	lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
		NULL, NULL, &ftWrite);

	if (lResult == ERROR_SUCCESS)
	{
		do {

			*lpEnd = TEXT('\0');
			StringCchCat(lpSubKey, MAX_PATH * 2, szName);

			if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
				break;
			}

			dwSize = MAX_PATH;

			lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
				NULL, NULL, &ftWrite);

		} while (lResult == ERROR_SUCCESS);
	}

	lpEnd--;
	*lpEnd = TEXT('\0');

	RegCloseKey(hKey);

	// Try again to delete the key.

	lResult = RegDeleteKey(hKeyRoot, lpSubKey);

	if (lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL RegDelnode(HKEY hKeyRoot, LPCTSTR lpSubKey)
{
	TCHAR szDelKey[MAX_PATH * 2];

	StringCchCopy(szDelKey, MAX_PATH * 2, lpSubKey);
	return RegDelnodeRecurse(hKeyRoot, szDelKey);

}

BOOL RegAddnode(HKEY hKeyRoot, LPCTSTR lpSubKey)
{
	HKEY hTestKey;
	LONG result = 0;

	if (RegCreateKeyExA(hKeyRoot, (LPCSTR)lpSubKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hTestKey, (LPDWORD)result) == ERROR_SUCCESS)
	{
		RegCloseKey(hTestKey);
	return true;
	}
	else return false;
	
}