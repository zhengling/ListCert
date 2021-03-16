﻿// ListCert.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "atlstr.h"

#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <cryptuiapi.h>
#include <string.h>
#include <string>
//using namespace std;

#include <tchar.h>
#include <atlconv.h>

#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "cryptui.lib")

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

void MyHandleError(const char* psz)
{
	_ftprintf(stderr, TEXT("An error occurred in the program. \n"));
	_ftprintf(stderr, TEXT("%s\n"), psz);
	_ftprintf(stderr, TEXT("Error number %x.\n"), GetLastError());
	_ftprintf(stderr, TEXT("Program terminating. \n"));
	exit(1);
} // End of MyHandleError.


int main(void)
{

	//-------------------------------------------------------------------
	// Copyright (C) Microsoft.  All rights reserved.
	// This program lists all of the certificates in a system certificate
	// store and all of the property identifier numbers of those 
	// certificates. It also demonstrates the use of two
	// UI functions. One, CryptUIDlgSelectCertificateFromStore, 
	// displays the certificates in a store
	// and allows the user to select one of them, 
	// The other, CryptUIDlgViewContext,
	// displays the contents of a single certificate.

	//-------------------------------------------------------------------
	// Declare and initialize variables.

	HCERTSTORE       hCertStore;
	PCCERT_CONTEXT   pCertContext = NULL;
	CString pszStoreName = "MY";// 
	//LPSTR pstStoreName;
	LPWSTR pswStoreName;
	LPWSTR pswNameString = NULL;
	DWORD cNameString = NULL;
	void* pvData;
	DWORD            cbData;
	DWORD            dwPropId = 0;
	// Zero must be used on the first
	// call to the function. After that,
	// the last returned property identifier is passed.

//-------------------------------------------------------------------
//  Begin processing and Get the name of the system certificate store 
//  to be enumerated. Output here is to stderr so that the program  
//  can be run from the command line and stdout can be redirected  
//  to a file.

	//fprintf(stderr, "Please enter the store name:");
	//gets_s(pszStoreName, sizeof(pszStoreName));
	//pszStoreName = "MY";//T2W((LPTSTR)pscStoreName);

	//pstStoreName = new CHAR[sizeof(pszStoreName) * 2 + 1];
	//memset(pstStoreName, 0x00, sizeof(pszStoreName) * 2 + 1);
#ifdef UNICODE

	//pswNameString = pszNameString.AllocSysString();
	pswStoreName = pszStoreName.AllocSysString();
	//nCStringToChar(pszStoreName, pstStoreName);
	//memcpy(pstStoreName, pszStoreName, pszStoreName.GetLength());
	//nCStringToChar((CString)pszStoreName.c_str(), pstStoreName);
#else
	memcpy(lpcContainName, m_ContainName, m_ContainName.GetLength());
#endif
	//pswStoreName =(LPWSTR)pstStoreName;
	//string a = (LPWSTR)pszStoreName;
	//a= (LPWSTR)pszStoreName;
	wprintf(L"The store name is %s.\n", pswStoreName);//LPCSTR(CStringA(pszStoreName))

	//-------------------------------------------------------------------
	// Open a system certificate store.

	if (hCertStore = CertOpenSystemStore(
		NULL,
		pswStoreName))
	{
		wprintf(L"The %s store has been opened. \n",
			pswStoreName);
	}
	else
	{
		// If the store was not opened, exit to an error routine.
		MyHandleError("The store was not opened.");
	}

	//-------------------------------------------------------------------
	// Use CertEnumCertificatesInStore to get the certificates 
	// from the open store. pCertContext must be reset to
	// NULL to retrieve the first certificate in the store.

	// pCertContext = NULL;
	while (pCertContext = CertEnumCertificatesInStore(
		hCertStore,
		pCertContext))
	{
		//-------------------------------------------------------------------
		// A certificate was retrieved. Continue.
		//-------------------------------------------------------------------
		//  Display the certificate.

		if (cNameString = CertGetNameString(
			pCertContext,
			CERT_NAME_RDN_TYPE,//CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			NULL,
			0))
		{
			pswNameString = (LPWSTR)malloc(sizeof(wchar_t) * cNameString);
			//wprintf(L"\nCertificate for %s \n", pswNameString);
		}
		else
			fprintf(stderr, "CertGetName failed. \n");
		if (CertGetNameString(
			pCertContext,
			CERT_NAME_RDN_TYPE,//CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			pswNameString,
			cNameString))
		{
			//pswNameString = (LPWSTR)malloc(cNameString);
			wprintf(L"\nCertificate for %s \n", pswNameString);
		}
		else
			fprintf(stderr, "CertGetName failed. \n");
		free(pswNameString);

		if (CryptUIDlgViewContext(CERT_STORE_CERTIFICATE_CONTEXT,pCertContext,NULL,NULL,0,NULL))
		{
			//     printf("OK\n");
		}
		else
		{
			MyHandleError("UI failed.");
		}

		//-------------------------------------------------------------------
		// Loop to find all of the property identifiers for the specified  
		// certificate. The loop continues until 
		// CertEnumCertificateContextProperties returns zero.

		while (dwPropId = CertEnumCertificateContextProperties(
			pCertContext, // The context whose properties are to be listed.
			dwPropId))    // Number of the last property found.  
						  // This must be zero to find the first 
						  // property identifier.
		{
			//-------------------------------------------------------------------
			// When the loop is executed, a property identifier has been found.
			// Print the property number.

			printf("Property # %d found->", dwPropId);

			//-------------------------------------------------------------------
			// Indicate the kind of property found.

			switch (dwPropId)
			{
			case CERT_FRIENDLY_NAME_PROP_ID:
			{
				printf("Display name: ");
				break;
			}
			case CERT_SIGNATURE_HASH_PROP_ID:
			{
				printf("Signature hash identifier ");
				break;
			}
			case CERT_KEY_PROV_HANDLE_PROP_ID:
			{
				printf("KEY PROVE HANDLE");
				break;
			}
			case CERT_KEY_PROV_INFO_PROP_ID:
			{
				printf("KEY PROV INFO PROP ID ");
				break;
			}
			case CERT_SHA1_HASH_PROP_ID:
			{
				printf("SHA1 HASH identifier");
				break;
			}
			case CERT_MD5_HASH_PROP_ID:
			{
				printf("md5 hash identifier ");
				break;
			}
			case CERT_KEY_CONTEXT_PROP_ID:
			{
				printf("KEY CONTEXT PROP identifier");
				break;
			}
			case CERT_KEY_SPEC_PROP_ID:
			{
				printf("KEY SPEC PROP identifier");
				break;
			}
			case CERT_ENHKEY_USAGE_PROP_ID:
			{
				printf("ENHKEY USAGE PROP identifier");
				break;
			}
			case CERT_NEXT_UPDATE_LOCATION_PROP_ID:
			{
				printf("NEXT UPDATE LOCATION PROP identifier");
				break;
			}
			case CERT_PVK_FILE_PROP_ID:
			{
				printf("PVK FILE PROP identifier ");
				break;
			}
			case CERT_DESCRIPTION_PROP_ID:
			{
				printf("DESCRIPTION PROP identifier ");
				break;
			}
			case CERT_ACCESS_STATE_PROP_ID:
			{
				printf("ACCESS STATE PROP identifier ");
				break;
			}
			case CERT_SMART_CARD_DATA_PROP_ID:
			{
				printf("SMART_CARD DATA PROP identifier ");
				break;
			}
			case CERT_EFS_PROP_ID:
			{
				printf("EFS PROP identifier ");
				break;
			}
			case CERT_FORTEZZA_DATA_PROP_ID:
			{
				printf("FORTEZZA DATA PROP identifier ");
				break;
			}
			case CERT_ARCHIVED_PROP_ID:
			{
				printf("ARCHIVED PROP identifier ");
				break;
			}
			case CERT_KEY_IDENTIFIER_PROP_ID:
			{
				printf("KEY IDENTIFIER PROP identifier ");
				break;
			}
			case CERT_AUTO_ENROLL_PROP_ID:
			{
				printf("AUTO ENROLL identifier. ");
				break;
			}
			} // End switch.

		 //-------------------------------------------------------------------
		 // Retrieve information on the property by first getting the 
		 // property size. 
		 // For more information, see CertGetCertificateContextProperty.
			if (CertGetCertificateContextProperty(
				pCertContext,
				dwPropId,
				NULL,
				&cbData))
			{
				//  Continue.
			}
			else
			{
				// If the first call to the function failed,
				// exit to an error routine.
				MyHandleError("Call #1 to GetCertContextProperty failed.");
			}
			//-------------------------------------------------------------------
			// The call succeeded. Use the size to allocate memory 
			// for the property.

			if (pvData = (void*)malloc(cbData))
			{
				// Memory is allocated. Continue.
			}
			else
			{
				// If memory allocation failed, exit to an error routine.
				MyHandleError("Memory allocation failed.");
			}
			//----------------------------------------------------------------
			// Allocation succeeded. Retrieve the property data.

			if (CertGetCertificateContextProperty(
				pCertContext,
				dwPropId,
				pvData,
				&cbData))
			{
				// The data has been retrieved. Continue.
			}
			else
			{
				// If an error occurred in the second call, 
				// exit to an error routine.
				MyHandleError("Call #2 failed.");
			}
			//---------------------------------------------------------------
			// Show the results.

			printf("The Property Content is %d \n", pvData);

			//----------------------------------------------------------------
			// Free the certificate context property memory.

			free(pvData);
			pvData = NULL;
		}  // End inner while.

	} // End outer while.

	//-------------------------------------------------------------------
	// Select a new certificate by using the user interface.
	if (!(pCertContext = CryptUIDlgSelectCertificateFromStore(
		hCertStore,
		NULL,
		NULL,
		NULL,
		CRYPTUI_SELECT_LOCATION_COLUMN,
		0,
		NULL)))
	{
		MyHandleError("Select UI failed.");
	}


	//-------------------------------------------------------------------
	// Clean up.
	//pswNameString = NULL;
	pszStoreName.Empty();
	CertCloseStore(hCertStore, 0);
	CertFreeCertificateContext(pCertContext);
	printf("The function completed successfully. \n");
	return 1;
} // End of main.


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
