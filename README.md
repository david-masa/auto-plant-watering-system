
# Raspberry Pi picoを使用した自動植物給水システム
<img width="600" height="800" alt="overall" src="https://github.com/user-attachments/assets/dcc031c7-32a7-407d-b2d1-49b8b6ba00f1" /><br>
## 概要

Raspberry Pi Picoと土壌湿度センサーを使用した自動植物給水システムです。

土壌湿度センサーからアナログ値を定期的に取得し、設定した閾値を下回った場合に給水ポンプを動作させます。<br>
ポンプは一定時間動作した後に停止し、再度センサー値を確認します。

センサー値の取得、乾燥判定、ポンプ制御、OLED表示を、組み込みソフトウェアとして実装しています。

## 動作

1. 土壌湿度センサーからアナログ値を取得する<br>
2. 取得した値と設定した閾値を比較する<br>
3. 土壌が乾燥している場合、リレーをONにする<br>
4. リレーを介して給水ポンプを一定時間動作させる<br>
5. ポンプを停止し、一定時間後に再度測定する<br>

## 🛠 パーツリスト

| 部品名 | 用途・詳細 |
| :--- | :--- |
| **Raspberry Pi Pico H** | メインマイコンボード |
| **土壌水分センサー** | アナログ出力タイプ (AO使用 / DO未使用) |
| **OLEDディスプレイ (SSD1306)** | I2C接続 (128x64) |
| **水中ポンプ** | 小型DC給水ポンプ |
| **リレーモジュール** | ポンプ電源ON/OFF制御用 |
| **NPNトランジスタ (S8050)** | リレー駆動用スイッチャー |
| **18650バッテリー** | Pico本体用電源 |
| **TP4056充電モジュール** | 18650充電・安全制御用 |
| **単4ニッケル水素電池 × 4本** | ポンプ専用電源 |
| **その他** | ブレッドボード、配線、1kΩ抵抗、熱収縮チューブ |

## 配線
| ピン番号 | 接続先 |
|---:|---|
| 31 (GP26 / ADC0) | 土壌水分センサー AO |
| 20 (GP15) | 1kΩ → S8050 ベース |
| 36 (3.3V OUT) | 土壌水分センサー VCC |
| 38 (GND) | 土壌水分センサー GND |
| 38 (GND) | S8050 エミッタ |
| VBUS | 18650充電モジュールの出力 |
| GND | 18650充電モジュールのGND |
| S8050 コレクタ | リレーモジュール IN |
| リレーモジュール COM | 単4ニッケル水素電池×4 の＋ |
| リレーモジュール NO | 水中ポンプ＋ |
| 水中ポンプ－ | 単4ニッケル水素電池×4 の－ | 
| 6 (GP4 / SDA) | OLEDディスプレイ SSD1306 SDA |
| 7 (GP5 / SCL) | OLEDディスプレイ SSD1306 SCL |
| 36 (3.3V OUT) | OLEDディスプレイ SSD1306 VCC |
| 38 (GND) | OLEDディスプレイ SSD1306 GND |
                 
## 配線図
<img width="742" height="612" alt="自動水やり装置 drawio" src="https://github.com/user-attachments/assets/74709897-3893-491b-b159-ec1e91acb0b5" />
<img width="800" height="600" alt="circuit" src="https://github.com/user-attachments/assets/cfb39c06-6532-4e87-a9ab-f8cff7c5984e" /><br>

## 熱収縮チューブを用いた防水処理
<img width="300" height="400" alt="PXL_20260903_112728157 MP" src="https://github.com/user-attachments/assets/1a9a1732-eb3f-4e08-b6cb-31d0fc6ef0e1" />
<img width="300" height="400" alt="PXL_20260903_112932439 MP" src="https://github.com/user-attachments/assets/67d52249-bdd0-4cd5-a3d7-3938b5951275" />

## 使用ライブラリ
本プロジェクトでは、以下のオープンソースライブラリを使用しています。

pico-ssd1306<br>
Author: daschr<br>
Repository: https://github.com/daschr/pico-ssd1306<br>
License: MIT License<br>
SSD1306 OLEDディスプレイの制御に使用しています。<br>
