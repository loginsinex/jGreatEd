
/*
	A FDA Super Mario Bros 2 [J] [public version]
	Copyright (C) 2020 ALXR aka loginsin
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

class CSysString
{
	BOOL											m_fPrivate;
	std::map<HINSTANCE, std::map<DWORD, tstring>>	m_mStrList;
	std::map<HINSTANCE, std::map<DWORD, tstring>> &	m_lStrList;

	static BOOL CALLBACK	_EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);

	void		LoadInstanceStrings(HINSTANCE hInstance);

public:
	CSysString(BOOL fPrivate = FALSE);
	~CSysString();

	tstring		GetString(HINSTANCE hInstance, DWORD idStr);
	BOOL		GetStringsList(HINSTANCE hInstance, std::map<DWORD, tstring> & mStrList);
	BOOL		FreeStringsList(HINSTANCE hInstance);
};