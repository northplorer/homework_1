#include <stdio.h>
#include <stdarg.h>
#include <Strsafe.h>
#include <stdlib.h>
#include <memory>
#include "windows.h"
#include <locale.h>

typedef unsigned int uint32_t;

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
		return false;
	else
		return true;
}

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}


/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool create_bob_txt()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	
	// 파일에 데이터 쓰기
	DWORD bytes_written = 0;
	char string_bufa[1024] = { 0, };

	wchar_t string_buf[1024];
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세. Welcome to the Hello world")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	// 와이드바이트 문자열타입에서 utf8 인코딩된 문자열로
	// 6번째 인자값이 0일 경우 사이즈 반환
	int nLen = WideCharToMultiByte(CP_UTF8, 0, string_buf, lstrlenW(string_buf), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, string_buf, lstrlenW(string_buf), string_bufa, nLen, NULL, NULL);

	// wcslen(sting_buf) : string_buf의 실제 글자길이 (바이트 단위아님)
	// 와이드바이트 문자열 타입으로 파일에 실제 글자길이 만큼 쓰기
	if (!WriteFile(file_handle, string_bufa, strlen(string_bufa), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}	
	
	// 파일 닫기
	CloseHandle(file_handle);

	return true;

}

bool copy_bob_txt()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t src_file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		src_file_name,
		sizeof(src_file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t dest_file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		dest_file_name,
		sizeof(dest_file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); 
	buf = NULL;

	CopyFileW(src_file_name, dest_file_name, false);
	return true;
}

bool delete_all_bob_file()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t src_file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		src_file_name,
		sizeof(src_file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t dest_file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		dest_file_name,
		sizeof(dest_file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); 
	buf = NULL;

	// bob.txt 삭제
	DeleteFileW(src_file_name);

	// bob2.txt 삭제
	DeleteFileW(dest_file_name);

	return true;
}

bool printUTFstr_file()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t dest_file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		dest_file_name,
		sizeof(dest_file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf);
	buf = NULL;

	HANDLE file_handle = CreateFileW(
		(LPCWSTR)dest_file_name,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (INVALID_HANDLE_VALUE == file_handle)
	{
		print("err, CreateFile(%ws) failed, gle = %u", dest_file_name, GetLastError());
		return false;
	}

	// check file size
	DWORD fileSize;

	// read "utf8 형식으로 인코딩된 문자열" from bob2.txt file.
	char string_bufa[1024] = { 0, };

	// 멀티바이트 문자열 타입(char 배열), 한글28, 34
	// fileSize : 118(char길이), sizeof(string_bufa)=1024
	ReadFile(file_handle, string_bufa, sizeof(string_bufa), &fileSize, NULL);
	
	wchar_t string_buf[1024];
	// nLen: 와이드바이트 기준으로 62
	int nLen = MultiByteToWideChar(CP_UTF8, 0, string_bufa, fileSize, NULL, NULL);
	// UTF8 -> Uni Code
	MultiByteToWideChar(CP_UTF8, 0, string_bufa, fileSize, string_buf, nLen);

	char newstr[1024] = { 0, };
	// Uni Code -> Multi byte
	WideCharToMultiByte(CP_ACP, 0, string_buf, nLen, newstr, fileSize, NULL, NULL);

	// 멀티바이트 -> UTF-8
	// 멀티바이트 -> 유니코드 -> UTF8
	//
	// UTF-8 -> 멀티바이트
	// UTF-8 -> 유니코드 -> 멀티바이트
	printf("%s\n", newstr);


	// 파일 닫기
	CloseHandle(file_handle);
	return true;
}

bool printUTFstr_io()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t dest_file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		dest_file_name,
		sizeof(dest_file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf);
	buf = NULL;

	HANDLE file_handle = CreateFileW(
		(LPCWSTR)dest_file_name,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (INVALID_HANDLE_VALUE == file_handle)
	{
		print("err, CreateFile(%ws) failed, gle = %u", dest_file_name, GetLastError());
		return false;
	}

	LARGE_INTEGER fileSize;
	if (TRUE != GetFileSizeEx(file_handle, &fileSize))
	{
		print("err, GetFileSizeEx(%ws) failed, gle = %u", dest_file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	DWORD file_size = (DWORD)fileSize.QuadPart;
	HANDLE file_map = CreateFileMapping(
		file_handle,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
		);
	if (NULL == file_map)
	{
		print("err, CreateFileMapping(%ws) failed, gle = %u", dest_file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	PCHAR file_view = (PCHAR)MapViewOfFile(
		file_map,
		FILE_MAP_READ,
		0,
		0,
		0
		);
	if (file_view == NULL)
	{
		print("err, MapViewOfFile(%ws) failed, gle = %u", dest_file_name, GetLastError());

		CloseHandle(file_map);
		CloseHandle(file_handle);
		return false;
	}


	wchar_t string_buf[1024];
	// nLen: 와이드바이트 기준으로 62
	int nLen = MultiByteToWideChar(CP_UTF8, 0, file_view, file_size, NULL, NULL);
	// UTF8 -> Uni Code
	MultiByteToWideChar(CP_UTF8, 0, file_view, file_size, string_buf, nLen);

	char newstr[1024] = { 0, };
	// Uni Code -> Multi byte
	WideCharToMultiByte(CP_ACP, 0, string_buf, nLen, newstr, file_size, NULL, NULL);

	// 멀티바이트 -> UTF-8
	// 멀티바이트 -> 유니코드 -> UTF8
	//
	// UTF-8 -> 멀티바이트
	// UTF-8 -> 유니코드 -> 멀티바이트
	printf("%s\n", newstr);


	// close all
	UnmapViewOfFile(file_view);
	CloseHandle(file_map);
	CloseHandle(file_handle);

	return true;
}

void main()
{
	printf("create bob.txt");
	create_bob_txt();
	printf("\ncopy bob2.txt\n");
	copy_bob_txt();

	printf("\print text from bob2.txt using File I/O\n");
	printUTFstr_file();

	printf("\print text from bob2.txt using Memory mapped I/O\n");
	printUTFstr_io();

	printf("\delete all files.\n");
	delete_all_bob_file();

	return;
}