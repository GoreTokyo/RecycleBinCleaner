#include <windows.h>   
#include <shlobj.h>
#include <shobjidl.h>
#include <shlwapi.h>   // StrRetToBufA を使用するためのヘッダーファイル

#include <iostream>
#include <fstream>

#pragma comment(lib, "Shlwapi.lib")  // Shlwapi.lib をリンクして、StrRetToBufA を使えるようにする

// ゴミ箱の内容をログファイルに記録し、コンソールに表示する関数
void LogDeletedFiles() {
    // ログファイルを開く（ファイルが存在しない場合、新しく作成）
    std::ofstream logFile("recycle_bin_log.txt", std::ios::app);
    if (!logFile) {  // ファイルが開けない場合
        std::cerr << "ログファイルを開けませんでした。" << std::endl;
        return;  // 何もせず終了
    }

    // ゴミ箱の情報を取得するための構造体
    SHQUERYRBINFO rbInfo;
    rbInfo.cbSize = sizeof(SHQUERYRBINFO);  // 構造体のサイズを設定
    // ゴミ箱の情報を取得
    HRESULT result = SHQueryRecycleBin(NULL, &rbInfo);
    if (result == S_OK) {
        // 情報取得に成功した場合、ゴミ箱内のファイル数とサイズをログとコンソールに書き込む
        logFile << "ゴミ箱内のファイル数: " << rbInfo.i64NumItems << std::endl;
        logFile << "合計サイズ: " << rbInfo.i64Size << " バイト" << std::endl;
        std::cout << "ゴミ箱内のファイル数: " << rbInfo.i64NumItems << std::endl;
        std::cout << "合計サイズ: " << rbInfo.i64Size << " バイト" << std::endl;

        // ゴミ箱内にファイルがある場合、ファイル名を表示
        if (rbInfo.i64NumItems > 0) {
            // ゴミ箱内のファイルを列挙
            IShellFolder* pDesktopFolder;
            if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder))) {
                LPITEMIDLIST pRecycleBin;
                // ゴミ箱の場所（シェルフォルダ）を取得
                if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_BITBUCKET, &pRecycleBin))) {
                    IShellFolder* pRecycleFolder;
                    // ゴミ箱のフォルダをオブジェクトとして取得
                    if (SUCCEEDED(pDesktopFolder->BindToObject(pRecycleBin, NULL, IID_IShellFolder, (void**)&pRecycleFolder))) {
                        IEnumIDList* pEnum;
                        // ゴミ箱内のアイテムを列挙
                        if (SUCCEEDED(pRecycleFolder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pEnum))) {
                            LPITEMIDLIST pidl;
                            // ゴミ箱内の各アイテムに対して処理を行う
                            while (pEnum->Next(1, &pidl, NULL) == S_OK) {
                                STRRET str;
                                // ファイル名を取得
                                if (SUCCEEDED(pRecycleFolder->GetDisplayNameOf(pidl, SHGDN_NORMAL, &str))) {
                                    char szName[MAX_PATH];
                                    // 取得したファイル名をバッファに格納
                                    if (SUCCEEDED(StrRetToBufA(&str, pidl, szName, MAX_PATH))) {
                                        // ファイル名をログに書き込む
                                        logFile << szName << std::endl;
                                        // コンソールにも表示
                                        std::cout << szName << std::endl;
                                    }
                                }
                                // メモリの解放
                                CoTaskMemFree(pidl);
                            }
                            pEnum->Release();  // 列挙オブジェクトを解放
                        }
                        pRecycleFolder->Release();  // ゴミ箱フォルダオブジェクトを解放
                    }
                    CoTaskMemFree(pRecycleBin);  // ゴミ箱の場所情報を解放
                }
                pDesktopFolder->Release();  // デスクトップフォルダオブジェクトを解放
            }
        }
    }
    else {
        logFile << "ゴミ箱の情報取得に失敗しました。HRESULT: " << result << std::endl;
        std::cout << "ゴミ箱の情報取得に失敗しました。HRESULT: " << result << std::endl;
    }

    // ログファイルを閉じる
    logFile.close();
}

int main() {
    HRESULT hr = CoInitialize(NULL);  // COMライブラリを初期化
    if (FAILED(hr)) {  // 初期化に失敗した場合
        std::cerr << "COMライブラリの初期化に失敗しました。" << std::endl;
        return 1;  // エラーコード1で終了
    }

    LogDeletedFiles();  // ゴミ箱の情報をログに記録し、コンソールにも表示

    // ゴミ箱の情報を再確認
    SHQUERYRBINFO rbInfo;
    rbInfo.cbSize = sizeof(SHQUERYRBINFO);
    HRESULT result = SHQueryRecycleBin(NULL, &rbInfo);
    if (SUCCEEDED(result) && rbInfo.i64NumItems > 0) {
        // ゴミ箱にアイテムがある場合、ゴミ箱を空にする
        result = SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
        if (SUCCEEDED(result)) {  // 成功した場合
            std::cout << "ゴミ箱を空にしました。" << std::endl;
        }
        else {  // 失敗した場合
            std::cout << "ゴミ箱の削除に失敗しました。HRESULT: " << result << std::endl;
        }
    }
    else {
        std::cout << "ゴミ箱は空です。" << std::endl;
    }

    // ここで明示的に終了
    std::cout << "プログラムが終了します。" << std::endl;

    // 5秒後にプログラムを終了する
    Sleep(5000);  // 5秒間待機

    CoUninitialize();  // COMライブラリを解放
    return 0;  // 正常終了
}
