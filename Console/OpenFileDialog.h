#pragma once
#include <windows.h>
#include <ShObjIdl_core.h>
#include <string>

inline HRESULT OpenFileDialog(HWND hwnd, std::wstring& filePath)
{
  // Initialize the COM library
  HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (FAILED(hr))
  {
    return hr;
  }

  // Create the File Open Dialog object
  IFileOpenDialog* pFileOpen = NULL;
  hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

  if (SUCCEEDED(hr))
  {
    // Show the Open dialog box
    hr = pFileOpen->Show(hwnd);

    // Get the file name from the dialog box
    if (SUCCEEDED(hr))
    {
      IShellItem* pItem;
      hr = pFileOpen->GetResult(&pItem);
      if (SUCCEEDED(hr))
      {
        PWSTR pszFilePath;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        if (SUCCEEDED(hr))
        {
          filePath = pszFilePath;
          CoTaskMemFree(pszFilePath);
        }
        pItem->Release();
      }
    }
    pFileOpen->Release();
  }

  // Uninitialize the COM library
  CoUninitialize();
  return hr;
}
