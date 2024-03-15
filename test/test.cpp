#include <iostream> // cout, endl, cin
#include <string> // string, to_string, stoi
#include <vector> // vector
#include <algorithm> // min, max, swap, sort, reverse, lower_bound, upper_bound
#include <utility> // pair, make_pair
#include <tuple> // tuple, make_tuple
#include <cstdint> // int64_t, int*_t
#include <cstdio> // printf
#include <map> // map
#include <queue> // queue, priority_queue
#include <set> // set
#include <stack> // stack
#include <deque> // deque
#include <unordered_map> // unordered_map
#include <unordered_set> // unordered_set
#include <bitset> // bitset
#include <cctype> // isupper, islower, isdigit, toupper, tolower

#include <windows.h>
#include <stdio.h>
#include <winhttp.h>

#pragma comment (lib, "winhttp.lib")

std::string UTF8toSjis(std::string srcUTF8) {
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
	char* bufShiftJis = new char[lengthSJis];
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);
	std::string strSJis(bufShiftJis);
	delete bufUnicode;
	delete bufShiftJis;
	return strSJis;
}
std::string SjistoUTF8(std::string srcSjis) {
	int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);
	int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
	char* bufUTF8 = new char[lengthUTF8];
	WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);
	std::string strUTF8(bufUTF8);
	delete bufUnicode;
	delete bufUTF8;
	return strUTF8;
}

std::string converter_gpt(std::string input) {

	WCHAR* url = (WCHAR*)L"https://api.openai.com/v1/chat/completions";
	HINTERNET hSession, hConnect, hRequest;
	URL_COMPONENTS urlComponents = { 0 };
	WCHAR szHostName[256], szUrlPath[2048];
	WCHAR* header;
	DWORD ret = 0;
	DWORD dwSize;
	DWORD dwStatusCode;

	// WinHTTPの初期化
	hSession = WinHttpOpen(L"UserAgent/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession == NULL) {
		return "エラーが発生しました";
	}


	DWORD dwValue = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
	if (!WinHttpSetOption(
		hSession,
		WINHTTP_OPTION_SECURE_PROTOCOLS,
		&dwValue,
		sizeof(dwValue)))
	{
		return "エラーが発生しました";
	}

	// URL解析
	urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
	urlComponents.lpszHostName = szHostName;
	urlComponents.dwHostNameLength = sizeof(szHostName) / sizeof(WCHAR);
	urlComponents.lpszUrlPath = szUrlPath;
	urlComponents.dwUrlPathLength = sizeof(szUrlPath) / sizeof(WCHAR);
	if (!WinHttpCrackUrl(url, wcslen(url), 0, &urlComponents)) {
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}

	// HTTPの開始
	hConnect = WinHttpConnect(hSession, szHostName, 443, 0);
	if (hConnect == NULL) {
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}
	hRequest = WinHttpOpenRequest(hConnect, L"POST", szUrlPath,
		NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
		WINHTTP_FLAG_SECURE);
	if (hRequest == NULL) {
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}

	// POSTデータの送信
	std::string pronpt = "{\"model\": \"gpt-4-0125-preview\", \"messages\": [{\"role\":\"system\",\"content\":\"あなたは辞書ですが、通常の辞書と異なり意味から見出し語を探す辞書です。これから意味が与えられるので、元の見出し語を推測して、元の見出し語のみを出力してください。\"},{\"role\":\"user\",\"content\":\"" + input + "\"}]}";
	pronpt = SjistoUTF8(pronpt);
	char* sendData = (char*)pronpt.c_str();

	if (WinHttpSendRequest(hRequest, L"Content-Type: application/json\r\nAuthorization: Bearer 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\r\n", -1,
		sendData, strlen(sendData), strlen(sendData), 0) == FALSE) {
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}
	if (WinHttpReceiveResponse(hRequest, NULL) == FALSE) {
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}

	// ヘッダー取得
	dwSize = 0;
	if (WinHttpQueryHeaders(hRequest,
		WINHTTP_QUERY_RAW_HEADERS_CRLF,
		WINHTTP_HEADER_NAME_BY_INDEX,
		WINHTTP_NO_OUTPUT_BUFFER, &dwSize,
		WINHTTP_NO_HEADER_INDEX) == FALSE) {
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			WinHttpCloseHandle(hRequest);
			WinHttpCloseHandle(hConnect);
			WinHttpCloseHandle(hSession);
			return "エラーが発生しました";
		}
	}
	header = (WCHAR*)HeapAlloc(GetProcessHeap(), 0, dwSize);
	if (header == NULL) {
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}
	if (WinHttpQueryHeaders(hRequest,
		WINHTTP_QUERY_RAW_HEADERS_CRLF,
		WINHTTP_HEADER_NAME_BY_INDEX,
		header, &dwSize, WINHTTP_NO_HEADER_INDEX) == FALSE) {
		HeapFree(GetProcessHeap(), 0, header);
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return "エラーが発生しました";
	}
	//wprintf(L"%s", header);
	HeapFree(GetProcessHeap(), 0, header);

	// レスポンスコードの確認
	dwSize = sizeof(DWORD);
	WinHttpQueryHeaders(hRequest,
		WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
		WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwSize,
		WINHTTP_NO_HEADER_INDEX);
	if (dwStatusCode != HTTP_STATUS_OK) {
		std::cout << "エラーが発生しました。エラーコード : " + std::to_string(dwStatusCode) << std::endl;
	}

	std::string res = "";

	// 本文の受信
	while (1) {
		DWORD dwSize = 0;
		if (WinHttpQueryDataAvailable(hRequest, &dwSize) == FALSE) {
			ret = -1;
			break;
		}
		if (dwSize == 0) {
			break;
		}
		BYTE* buf = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, (dwSize + 1) * sizeof(BYTE));
		if (buf == NULL) {
			ret = -1;
			break;
		}
		if (WinHttpReadData(hRequest, buf, dwSize, NULL) == FALSE) {
			HeapFree(GetProcessHeap(), 0, buf);
			ret = -1;
			break;
		}
		buf[dwSize] = '\0';
		for (int i = 0; i < dwSize; i++) {
			res += buf[i];
		}
		HeapFree(GetProcessHeap(), 0, buf);
	}

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
	res = UTF8toSjis(res);
	return res.substr(res.find("content") + 11, (res.find("logprobs") - 18) - (res.find("content") + 11));
}

int main(){
	std::cout << "テキストを入力 : ";
	std::string input;
	std::cin >> input;
	std::cout << converter_gpt(input) << std::endl;
}