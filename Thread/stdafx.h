// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <thread>
#include <iostream>
#include <windows.h>
#include <list>
#include <mutex>
#include <condition_variable>
#include <functional>

template <class T>
inline void safe_delete(T*& p) {
	delete p;
	p = nullptr;
}

template <typename T>
inline void safe_delete_array(T*& p) {
	delete[] p;
	p = NULL;
}

#define FOR_BY_ITR(i,c) for(decltype((c).begin()) i=(c).begin(); i!=(c).end(); ++i)

// TODO: プログラムに必要な追加ヘッダーをここで参照してください
