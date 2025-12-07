# メモリクリーンアップ実装ドキュメント

## 概要

Valgrindで検出される "Still Reachable" メモリを解放し、プログラム終了時のメモリリークをゼロにするための実装です。

## 実装内容

### 1. 新規ファイル: `srcs/cleanup.c`

以下の3つの関数を実装しました：

#### `free_map(t_map *map)`
- 環境変数マップ (`t_map`) とその全てのアイテム (`t_item`) を解放
- 各アイテムの `name` と `value` も解放
- リンクリストを辿って全てのノードを解放

#### `cleanup_shell(t_shell *shell)`
- シェル全体のクリーンアップを実行
- `envmap` を解放 (`free_map` を呼び出し)
- `rl_clear_history()` を呼び出してReadlineの履歴を消去

#### `exit_shell(t_shell *shell, int status)`
- `cleanup_shell` を呼び出してメモリを解放
- 指定されたステータスコードで `exit()` を実行

### 2. 変更ファイル

#### `includes/minishell.h`
- 新しいクリーンアップ関数のプロトタイプ宣言を追加
- `CLEANUP` セクションを新設

#### `srcs/main.c`
- `main()` 関数の終了時（Ctrl+D でのEOF時）に `cleanup_shell(&shell)` を呼び出し
- これにより、通常終了時にも全てのメモリが解放される

#### `srcs/builtin/builtin.c`
- `exec_builtin()` 関数内で `exit` コマンド実行時に `exit_shell()` を呼び出し
- これにより、`exit` コマンド実行時にも全てのメモリが解放される

#### `Makefile`
- `srcs/cleanup.c` をコンパイル対象に追加

## 解放されるメモリ

### 1. 環境変数マップ (`envmap`)
- `initenv()` で作成された環境変数のハッシュマップ
- 全ての環境変数の名前と値
- マップ構造体自体

### 2. Readlineの履歴
- `rl_clear_history()` により、Readlineライブラリが内部で保持している履歴データを解放

### 3. 既存の解放処理（変更なし）
- トークンリスト (`t_token`): `free_tok()` により各コマンド実行後に解放
- ASTノード (`t_node`): `free_node()` により各コマンド実行後に解放
- argv配列: `free_argv()` により使用後に解放

## 終了パス

プログラムは以下の2つの方法で終了します：

### 1. `exit` コマンド
```
minishell$ exit 0
```
- `builtin_exit()` が `BUILTIN_EXIT_REQUEST` を返す
- `exec_builtin()` が `exit_shell()` を呼び出す
- `cleanup_shell()` で全メモリを解放
- `exit()` で終了

### 2. Ctrl+D (EOF)
```
minishell$ ^D
```
- `readline()` が `NULL` を返す
- `main()` のループを抜ける
- `cleanup_shell(&shell)` を呼び出す
- `exit(shell.last_status)` で終了

## テスト方法

### macOSの場合（leaksコマンド）
```bash
# テストスクリプトを実行
./test_memory.sh
```

### Linuxの場合（Valgrind）
```bash
# exitコマンドでのテスト
echo -e "export TEST=hello\nexit 0" | valgrind --leak-check=full --show-leak-kinds=all ./minishell

# Ctrl+Dでのテスト
echo -e "export TEST=hello" | valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

期待される結果：
- "definitely lost" : 0 bytes
- "indirectly lost" : 0 bytes
- "possibly lost" : 0 bytes
- "still reachable" : 0 bytes (または最小限)

## 注意事項

1. **Readlineライブラリの内部メモリ**
   - `rl_clear_history()` を呼び出していますが、Readlineライブラリ自体が保持する一部のメモリは解放されない可能性があります
   - これはライブラリの実装に依存します

2. **環境変数の初期化**
   - `initenv()` で作成された `envmap` は、プログラム終了時まで保持されます
   - 終了時に確実に解放されます

3. **コマンド実行中のメモリ**
   - 各コマンド実行後、トークンとASTは即座に解放されます
   - 終了時に残っているのは、シェル自体が保持するデータのみです

## Norminette準拠

全ての新規・変更ファイルはNorminette準拠です：
```bash
norminette srcs/cleanup.c includes/minishell.h srcs/main.c srcs/builtin/builtin.c
```

## まとめ

この実装により、以下が達成されました：

✅ 環境変数マップの完全な解放  
✅ Readline履歴の解放  
✅ `exit` コマンドでの終了時のクリーンアップ  
✅ Ctrl+D (EOF) での終了時のクリーンアップ  
✅ Norminette準拠  
✅ 既存機能への影響なし  

Valgrindでの "Still Reachable" メモリは大幅に削減され、理想的にはゼロになります。

