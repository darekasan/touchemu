## touchemu

タッチスクリーンのタッチ操作をエミュレーションする何か

### なにこれ

Androidスマホに繋いでタッチ操作を自動化するためのツール。

シリアルでコマンドを受け取ってUSB HIDのタッチスクリーンデバイスとして振る舞うArduinoのスケッチと、PCからそれを操作して自動タップなどを行うWebインターフェイスで構成されます。

- [使っている様子1](https://twitter.com/darekasan_net/status/1481351055682469888)
- [使っている様子2](https://twitter.com/darekasan_net/status/1481357240456343552)

### Requirements

- Arduino Leonardo (Pro microも可)
- 任意のUSB-UART変換
- [ilufang/HID](https://github.com/ilufang/HID)
    - [Arduino HID Project](https://github.com/NicoHood/HID)のフォークで、タッチスクリーンになれる
- [Uberi/Arduino-CommandParser](https://github.com/Uberi/Arduino-CommandParser)
- USBホスト機能を備えたAndroidスマホ
- デスクトップ版Google Chrome
    - Serial APIやScreen Capture APIを使うため

### 使い方

1. Arduino Leonardoにスケッチをアップロード
2. ArduinoのUART TX/RXを任意のUSB-UART変換に接続
3. Arduinoをスマホに、USB-UART変換をPCに接続
4. PCからコマンドを発行

外部ディスプレイ接続時は操作不能なことがあるが原因不明

### gui

クリックでコマンドを発行したり、かんたんなマクロを実行したりできます。Screen Captureでスマホ画面ミラーリングのウィンドウやカメラ映像、スクショなどをはめ込んで操作しやすくする機能つき。

ローカルのWebサーバーでguiフォルダ内にあるindex.htmlを公開してChromeで開くと使えます。Pythonが入ってる環境なら`python -m SimpleHTTPServer 8080`でいいかも。

なぐり書きで例外処理などなくとりあえず動く程度。

##### 操作説明

1行目:

- Connect: シリアルポートを開く
- Capture: ウィンドウキャプチャで画面をはめこむ
- Start/Stop: マクロを実行/停止する

2行目:

- Swap: wとhの値を入れ替える 縦長横長の切り替え用
- w/h: スマホ画面の解像度を入力 縦横比さえあってりゃなんでもいい
- scale/x/y: キャプチャ画面の拡大率と位置の調整 見た目だけで挙動に影響なし
- Swap input axis: 座標のx/yを入れ替える
- Invert x/Invert y: 軸を反転

枠:

- 画面左の枠
    - クリックでタップコマンドを発行
    - Ctrl+クリックでマクロリストにコマンドを追記
- 画面右のテキストボックス
    - テキストボックスの上のボタンはコマンドスニペット

##### マクロについて

touchとreleaseはArduinoにそのまま送信しているだけなので、TeraTermなどで打ち込んでも使える。

- wait [ミリ秒]
    - 指定した時間待つ
- ;[ラベル名]
    - 行をラベルとする
    - ラベルを書いただけでは何も起きないのでコメント代わりに使える
- goto [ラベル名]
    - ラベルにジャンプし、ラベルの行から実行する
    - 条件分岐や変数が実装されていないのでもっぱら無限ループ用
- touch [id] [座標x] [座標y]
    - 画面をタッチしてることにする
    - releaseするまでずっとタッチしている状態になる
    - releaseをせずに別の座標をtouchするとフリックやスライドの動作を再現できる
    - idはマルチタッチ時に指を判別するためのもの マルチタッチしないなら常に0でOK
    - 座標の範囲は0-10000 画面の端から端まで
    - ディスプレイのドット数などとは無関係
- release [id]
    - 画面から指を離したことにする
    - touchで指定したidを指定する

### そのうちやりたい

- 条件分岐したい
    - コードを再利用する仕組みとしてほしい ある範囲を指定回数繰り返すとか
    - 画像認識やOCRでより高度な自動化ができたら色々悪いことできると思う
- キーボード操作やマウス操作も送信したい
    - ノートパソコンをKVMコンソールとして使うのに良さそう
- 入力デバイスの変換に使いたい
    - ゲームパッドの入力をタッチ操作に変換するとかそういう