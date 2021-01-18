
# ライブラリ機能とサンプルプログラムの説明

このページでは、下記サンプルの説明を通して　SimpleObjectDrawingLibrary (以下、SODL) の機能を説明します。

- [sample_01_LoadModel](sample_01_LoadModel.md)
- [sample_02_6AxRobot](sample_02_6AxRobot.md)
- [sample_03_MultiViewPort](sample_03_MultiViewPort.md)
- [sample_04_DrawGraph](sample_04_DrawGraph.md)

# 各サンプルの概要

## [sample_01_LoadModel](sample_01_LoadModel.md)
[sample_01_LoadModel 詳細説明はこちら](sample_01_LoadModel.md)

![FncAndSample_01_LoadModel_exeWindow](fig/FncAndSample_01_LoadModel_exeWindow.PNG)

このサンプルは、3Dモデルをロードして表示します。
本ライブラリにおける Hello World のようなものです。

- exeを実行した時点で、コンソールWindowとOpenGLのグラフィックウィンドウが立ち上がります。
- 3Dモデル "teapot.obj" をグラフィック画面に表示します。
- 描画している空間の原点 World_Origin を表す座標系シンボル(RGBがXYZに対応する直行する直線の形状)を表示します

本サンプルで説明するライブラリ機能
- モデル表示
- グラフィック画面のデフォルトUI操作


## [sample_02_6AxRobot](sample_02_6AxRobot.md)
[sample_02_6AxRobot 詳細説明はこちら](sample_02_6AxRobot.md)

![FncAndSample_02_6AxRobot_exeWindow](fig/FncAndSample_02_6AxRobot_exeWindow.PNG)

このサンプルは、6軸垂直多関節ロボットを模したモデルを表示し、
その各関節のモータ角度をキー入力で制御します。
キーボードの"1"\~"6"キーを押すとロボットの根本から先端の各関節角度が+方向に
"q"\~"y"キーを押すと、-方向に回転します。

本サンプルで説明するライブラリ機能
- 座標系連鎖
- ユーザ定義コールバック(キーボード)

## [sample_03_MultiViewPort](sample_03_MultiViewPort.md)
[sample_03_MultiViewPort 詳細説明はこちら](sample_03_MultiViewPort.md)

![FncAndSample_03_MultiViewPort_exeWindow](fig/FncAndSample_03_MultiViewPort_exeWindow.PNG)

このサンプルでは、[sample_02_6AxRobot](#sample_02_6AxRobot)と同じロボットアームを
4方向からカメラで撮影したような画面を表示します。
画面左上は[sample_02_6AxRobot](#sample_02_6AxRobot)と同様にマウスでカメラアングルを操作できます。
右上、左下、右下は、それぞれ上面図、側面図、正面図になっています。

本サンプルで説明するライブラリ機能
- マルチビューポート、マルチカメラ

## [sample_04_DrawGraph](sample_04_DrawGraph.md)
[sample_04_DrawGraph 詳細説明はこちら](sample_04_DrawGraph.md)

![FncAndSample_04_DrawGraph_exeWindow](fig/FncAndSample_04_DrawGraph_exeWindow.PNG)

このサンプルでは、[sample_02_6AxRobot](#sample_02_6AxRobot)と同じロボットアームを画面左側に表示し、
画面右側には2次元グラフをプロットしています。
上のグラフはロボットの一番根本側の関節の回転角度にランダムノイズを載せた値を時系列にプロットします。
左下のグラフは、画面上をマウスでドラッグした軌跡をプロットします。
画面右下のグラフは、ワールド座標系原点から見た、ロボットハンドのXY平面上の軌跡をプロットします。

本サンプルで説明するライブラリ機能
- グラフプロット
- 複数の描画空間


[トップページへ戻る](README.md)
