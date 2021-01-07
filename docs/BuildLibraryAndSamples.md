# SimpleObjectDrawingLibraryとサンプルプログラムのビルド手順

ここでは、SimpleObjectDrawingLibrary(以下、SODL)の使い方をサンプルプログラムのビルドを通して説明します。
(Windows環境を想定して説明します。)

![image_HowLibraryWorksOverview](fig/HowLibraryWorksOverview.PNG)


以下に手順を示します。

1. [ダウンロード](ダウンロード)
1. [依存ライブラリのインストール](依存ライブラリのインストール)
1. [cmakeによるVisualStudioプロジェクトの作成](cmakeによるVisualStudioプロジェクトの作成)
1. [ライブラリとサンプルプログラムのビルド](ライブラリとサンプルプログラムのビルド)

## ダウンロード
次のGitHubのリポジトリトップにアクセスしてgit clone や zip でダウンロードしてください。
https://github.com/HiroakiIMAI/SimpleObjectDrawingLibrary
![StartGuide_01_Download](fig/StartGuide_01_Download.PNG)

## 依存ライブラリのインストール
下記4つのライブラリをインストールしてください。
インストール作業の詳細は[こちらのページ](InstallDependency.md)を参照して下さい。
一般的なインストール方法を説明しているだけなので、このような作業に慣れている方は
読み飛ばして頂いて構いません。


### fleeglut
http://freeglut.sourceforge.net/index.php#download

### glew
http://glew.sourceforge.net/

### Eigen
http://eigen.tuxfamily.org/index.php?title=Main_Page

### Assimp
http://assimp.org/index.php/downloads



## cmakeによるVisualStudioプロジェクトの作成
以下では、 D:\tmp の下に SODL のソースコードを置いた場合の例を示します。
cmakeを起動して下記のようにパスを指定します
![](fig)


## ライブラリとサンプルプログラムのビルド
