# 🚀 CH32V006用ミミクリーペット音声換装ファームウェア（MIMICRY_V006F8P6）


## 📌 はじめに (Introduction)

音声録音・オウム返しおもちゃの故障に対し、汎用32bit RISC-Vマイコン（**CH32V006F8P6**）へと基板ごと換装するために開発された、独自のボイスチェンジ機能が特徴的なファームウェアです。

名張市つつじが丘おもちゃ病院（大泉茂幸氏）の先行知見を次世代のドクターに継承するために、私（糸魚川で活動する「おもちゃクリニックゆりかご」代表ToyDr.わたなべ）の責任で先輩ドクターの治療技術を公開しています。

---

## ✨ CH32V006を採用する技術的メリット

私が感心するのはファームウェアが日々改善され広く公開されていることで、音声レベルのゼロクロスポイント区間での繰り返し再生は開発者の工夫点の一つ、従前より雑音レベルが大幅に減少されました。

さらに最近では、CH32V006F8P6 に IMA-ADPCM方式の採用により、音声データの圧縮と音質維持の両立、録音時間は約15秒を確保、マイクアンプやADPCMデコーダ機能も内蔵ファームで実装されました。

この高度な治療技術を、おもちゃドクターのみならず、より多くのソフト技術者の皆さま方にも興味を持っていただければ幸いです。

---

## 📂 リポジトリの構成

本リポジトリには以下のデータが含まれています。

| フォルダ・ファイル名 | 内容 |
| :--- | :--- |
| `Circuit_Schematic/` | 電子回路図データ |
| `PCB_Layout/` | 基板レイアウト（プリント基板の例） |
| `MIMIKURI/` | マイコン用のソースコード / ファームウェア |
| 他のファイル | プロジェクト関連ブログ記事・写真・資料 |

---

## 📖 詳細な解説・使い方 (Documentation)

回路図や書き込み手順などの詳細は元記事をご確認ください。

👉 [**名張市つつじが丘おもちゃ病院 元記事を読む**](https://tutujith.blog.fc2.com/blog-entry-1075.html)

- **直接ダウンロード**: [GitHub登録時、安定版 Source code (zip 3.5MB) ](https://github.com/ToyDr-W/CH32_MIMICRY_V006F8P6/archive/refs/tags/v1.0.0.zip)
- **開発プロジェクト**: `MIMIKURI\MIMIKURI_V006F8P6`
- **ワークスペース/プロジェクトファイル**: `MIMIKURI_V006F8P6.wvproj`

---

## 🧸試作機で動作検証 (Prototype)

私を含めシニア世代のおもちゃドクターが、技術研修や臨床現場で使いやすいように、基板レイアウト例を作りました。
EasyEDA STDからワン・クリックで一気通貫、JLCPCBに格安発注することが出来ました。

<p align="center">
<img width="60%" alt="CH32_MIMICRY_V006F8P6" src="https://github.com/user-attachments/assets/4e197fb6-f337-4a52-83d9-82b19a14266c" />
  <br>
<img width="60%" alt="261015_MIMICRY_boad (1)" src="https://github.com/user-attachments/assets/e966789d-9ce6-4ba5-a9cd-f5974ce3680f" />
  <br>
<img width="45%" alt="261015_MIMICRY_boad (2)" src="https://github.com/user-attachments/assets/4d22de9f-427f-4e07-a99f-dbafa3f58129" />
  <br>
</p>

---

## ⚖ 免責事項 (Disclaimer)

利用は自己責任となります。詳細は同梱の `LICENSE` ファイル（TIMライセンス）をご確認ください。

---

# 🚀 English Summary (For Global Makers & Toy Doctors)

## 🚀 Project Overview
This firmware features a unique voice change function and was developed to replace the entire board with a general-purpose 32-bit RISC-V microcontroller (CH32V006F8P6) in the event of a malfunction in a voice recording/parroting toy.

In order to pass on the advanced knowledge of Nabari City Tsutsujigaoka Toy Hospital (Mr. Shigeyuki Oizumi) to the next generation of doctors, I (ToyDr. Watanabe, representative of "Toy Clinic Yurikago" active in Itoigawa) am responsible for publishing the treatment techniques of senior doctors.


## ✨ Key Technical Benefits
What impresses me is that the firmware is being improved day by day and is being widely released.The repeated playback of the zero-crossing point section of the audio level is one of the developers' ideas, and the noise level has been significantly reduced compared to before.

More recently, the CH32V006F8P6 has adopted the IMA-ADPCM method to compress audio data and maintain sound quality, ensuring a recording time of approximately 15 seconds, and also has microphone amplifier and ADPCM decoder functions implemented in the built-in firmware.

We hope that not only toy doctors but also more software engineers will be interested in this advanced treatment technology.


## 📄 Documentation & Disclaimer
👉 [**Original Blog Post**](https://tutujith.blog.fc2.com/blog-entry-1075.html)  

- **Direct download**: [Stable version when registered on GitHub Source code (zip 3.5MB) ](https://github.com/ToyDr-W/CH32_MIMICRY_V006F8P6/archive/refs/tags/v1.0.0.zip)

Project Path: `MIMIKURI\MIMIKURI_V006F8P6` (`MIMIKURI_V006F8P6.wvproj`)  
Provided "as is" under the **TIM License**.
