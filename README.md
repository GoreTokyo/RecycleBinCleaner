# RecycleBinCleaner

## 概要
RecycleBinCleaner は、Windows のゴミ箱の中身を確認し、ログファイルに記録した後、ゴミ箱を空にする C++ プログラムです。

## 特徴
- ゴミ箱内のファイル数とサイズを取得
- ゴミ箱内のファイル名をログファイルに記録
- ゴミ箱が空でない場合、自動的に削除
- コンソールに処理結果を表示

## 動作環境
- Windows 10 / 11
- Visual Studio (C++ コンパイラ)
- Windows API に対応した環境

## ビルド方法
1. Visual Studio でプロジェクトを開く
2. `RecycleBinCleaner.cpp` をビルド
3. 実行ファイルを取得

## 使い方
1. `RecycleBinCleaner.exe` を実行
2. ゴミ箱の内容がコンソールに表示され、`recycle_bin_log.txt` に記録される
3. ゴミ箱が空でなければ、自動的に削除
4. 5 秒後にプログラム終了

## 注意事項
- 管理者権限での実行を推奨
- `recycle_bin_log.txt` に削除されたファイル名が記録される

## ライセンス
このプロジェクトは MIT ライセンスの下で提供されます。

