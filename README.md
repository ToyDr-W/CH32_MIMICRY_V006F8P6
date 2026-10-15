# 🚀 CH32V006用ミミクリーペット音声換装ファームウェア（MIMICRY_V006F8P6）


## 📌 はじめに (Introduction)

音声録音・オウム返しおもちゃの故障に対し、汎用32bit RISC-Vマイコン（**CH32V006F8P6**）へと基板ごと換装し、自前の音声処理で蘇らせるオープンソース・ファームウェアです。
名張市つつじが丘おもちゃ病院（大泉茂幸氏）の先行知見を次世代に継承するために高度な治療技術を紹介しています。

---

## ✨ CH32V006を採用する技術的メリット

IMA-ADPCMの導入によりデータ効率が向上し高音質化、低電圧で超省電力、おもちゃ修理のオープンハードウェアプロジェクトです。

- **サンプルレートの倍増**: 8ksps録音、16ksps再生。
- **解像度の向上**: 12bit ADC録音、16bit PCMでのADPCM処理、10bit相当のPWM。
- **ノイズレスな繰り返し再生**: 2回目以降の再生はSRAM保持のデコード値を利用しポップノイズを解消。
- **録音時間の確保**: 最大15秒の録音時間をキープ。
- **省電力性能**: スリープ時**実測11μA**。

---

## 📖 詳細な解説・使い方 (Documentation)

回路図や書き込み手順などの詳細は元記事をご確認ください。

👉 [**名張市つつじが丘おもちゃ病院 元記事を読む**](https://tutujith.blog.fc2.com/blog-entry-1075.html)

- **開発プロジェクト**: `MIMIKURI\MIMIKURI_V006F8P6`
- **ワークスペース/プロジェクトファイル**: `MIMIKURI_V006F8P6.wvproj`

---

## 🧸試作機で動作検証 (Prototype)

私（糸魚川で活動する「おもちゃクリニックゆりかご」代表ToyDr.わたなべ）を含めシニア世代のおもちゃドクターが、技術研修や臨床現場で使いやすいように、基板レイアウト例を作りました。
EasyEDA STDからワン・クリックで一気通貫、JLCPCBに格安発注することが出来ました。

<p align="center">
<img width="60%" alt="CH32_MIMICRY_V006F8P6" src="https://github.com/user-attachments/assets/6091e703-369f-4eb3-bf5a-bbed869c4933" />
  <br>
<img width="60%" alt="261015_MIMICRY_pcb (1)" src="https://github.com/user-attachments/assets/d0744aa2-2f7d-4f40-8d10-79ca378cae6d" />
  <br>
<img width="45%" alt="261015_MIMICRY_pcb (2)" src="https://github.com/user-attachments/assets/6e60093d-239c-4266-9fc4-eb0e37551685" />
  <br>
</p>

---

## ⚖ 免責事項 (Disclaimer)

利用は自己責任となります。詳細は同梱の `LICENSE` ファイル（TIMライセンス）をご確認ください。

---

# 🚀 English Summary (For Global Makers & Toy Doctors)

## 🚀 Project Overview
MimicryPet voice-changer firmware for CH32V006. High-quality audio via IMA-ADPCM, ultra-low power (11uA sleep), and open hardware toy repair.
An open-source firmware to replace broken toy voice-recorder PCBs with an affordable 32-bit RISC-V MCU (**CH32V006F8P6**), based on the work of Nabarishi Tsutsujigaoka Toy Hospital.

## ✨ Key Technical Benefits
- **Doubled Sample Rate**: 8 ksps recording and 16 ksps playback.
- **Enhanced Resolution**: 12-bit ADC and 10-bit PWM equivalent output.
- **Pop-Noise Elimination**: SRAM-based loop playback.
- **Recording Time**: Up to 15 seconds.
- **Ultra-Low Power**: 11 μA in Sleep mode.

## 📄 Documentation & Disclaimer
👉 [**Original Blog Post**](https://tutujith.blog.fc2.com/blog-entry-1075.html)  
Project Path: `MIMIKURI\MIMIKURI_V006F8P6` (`MIMIKURI_V006F8P6.wvproj`)  
Provided "as is" under the **TIM License**.
