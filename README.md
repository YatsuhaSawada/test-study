# test-study

## プロダクトコード
- ファイルテンプレート
- コメントテンプレート
    - ファイルテンプレート
    - クラスヘッダ
    - 関数ヘッダ

## テスト

メニューバーの`ファイル->ユーザー設定->ユーザースニペット`から以下のスニペットの登録をしてください。登録はcppファイルの設定に追加してください。
```json
	"GoogleTestTemplate": {
		"prefix": "gtesttemplate",
		"body": [			
			"\/\/Test テスト説明",
			"TEST($1, $2) {",
			"\t\/\/Arrange 事前条件の設定",
			"",
			"\t\/\/Action テスト実行",
			"",
			"\t\/\/Assert 事後条件と事前条件の確認",
			"}"
		],
		"description": "TEST(test_case_name, test_name in test_case_name)"
	}
```

以上を実行すると、`gtest～`と入力すると候補が表示されます。
```json
//Test テスト説明
TEST(, ) {
    //Arrange 事前条件の設定

    //Action テスト実行

    //Assert 事後条件と事前条件の確認
}
```