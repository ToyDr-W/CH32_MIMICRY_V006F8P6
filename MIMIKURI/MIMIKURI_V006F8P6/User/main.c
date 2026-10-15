//#define	SHDBS		//ﾃﾞﾊﾞｸﾞ信号を出力するときに宣言する
//#define	SHDBG		//ﾃﾞﾊﾞｸﾞ情報を出力するときに宣言する
//#define	SHBPS	115200	//ﾃﾞﾊﾞｸﾞ情報のﾎﾞｰﾚｰﾄを宣言する
				//　(ﾃﾞﾌｫﾙﾄ値は38400bps)
//#define	SHDBG_SOFT	//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装するときに宣言する	
//#define	NO_WRITE	//ﾌﾗｯｼｭに書込まないで実行するときに宣言する(ﾃﾞﾊﾞｸﾞ用)
#define	SLEEP_EN		//Sleep機能を有効にするときに宣言する


/*
ﾐﾐｸﾘ系の音声録音再生(ﾎﾞｲｽﾁｪﾝｼﾞｬｰ)を換装するためのﾌｧｰﾑｳｪｱ

【ﾀｰｹﾞｯﾄ】
・CH32V006F8P6(内部48MHzｸﾛｯｸ)

【動作の流れ】
・ﾓｰﾀｰ停止時間後に音声入力を始める｡
・入力された音声は先行ﾊﾞｯﾌｧに記録する
・音声が既定のﾚﾍﾞﾙを超えるとﾌﾗｯｼｭへの録音を開始する｡
・録音は､先ず先行ﾊﾞｯﾌｧのﾃﾞｰﾀを録音し､その後ﾘｱﾙﾀｲﾑﾃﾞｰﾀを録音する｡
・音声が途切れたとき､若しくはﾌﾗｯｼｭﾒﾓﾘが満杯になったら録音を終了し､再生を開始する｡
・再生中はﾓｰﾀｰを駆動する｡
・再生を終了するとﾓｰﾀｰを停止し､停止するまで一定時間待つ｡
・次の録音のﾒﾓﾘｱﾄﾞﾚｽを決めて､最初から繰返す｡

【音声ﾃﾞｰﾀ】
・ADC結果は12ﾋﾞｯﾄだが､ﾊﾞｲｱｽとﾀﾞｲﾅﾐｯｸﾚﾝｼﾞを確保するため11ﾋﾞｯﾄﾌﾙｽｹｰﾙで扱う｡
・正規化後に0～2047の範囲とし､1024をPCMの0ﾚﾍﾞﾙとする｡
・録音時は､ADC値を正規化した0～2047の値をﾘﾝｸﾞﾊﾞｯﾌｧに格納する｡
・再生時は､IMA-ADPCMﾃﾞｺｰﾄﾞ後の0～65535の値をﾘﾝｸﾞﾊﾞｯﾌｧに格納する｡

【ｻﾝﾌﾟﾙﾚｰﾄ】
・録音は8ksps､再生は16kspsで実行することでﾎﾞｲｽﾁｪﾝｼﾞする｡
・音源の帯域は4kHzとなる｡
・再生はｾﾞﾛｸﾛｽﾎﾟｲﾝﾄ間毎に2回ずつ繰返すことで､喋る速度は録音時と同じにする｡
・ｾﾞﾛｸﾛｽの判定は､正ﾚﾍﾞﾙが一定ｻﾝﾌﾟﾙ数以上継続後に負ﾚﾍﾞﾙになること｡

【先行ﾃﾞｰﾀの再生】
・音声を検知してから録音を開始すると音声の最初の部分が欠けて聴き取り辛くなる｡
・そのため､先行ﾊﾞｯﾌｧに音声を常時記録しておき､音声を検知すると先行ﾊﾞｯﾌｧのﾃﾞｰﾀから録音する｡
・先行ﾊﾞｯﾌｧのﾃﾞｰﾀの録音後に､ﾘｱﾙﾀｲﾑの音声を録音する｡
・音源の帯域を10Hz以上とすると､8ksps/10Hz=800ｻﾝﾌﾟﾙとなり､ﾊﾞｯﾌｧは1kｻﾝﾌﾟﾙ分を用意する｡

【ﾀｲﾐﾝｸﾞ】
・TIMｸﾛｯｸをHCLK(48MHz)､PWMｽﾃｯﾌﾟ数を1204とする｡
・PWM周波数は48MHz/1024=46.9kHz､PWM周期は21.3usとなる｡
・LPF用移動平均のｻﾝﾌﾟﾙ数は46.9kHz/2kHz=23となる｡2のべき乗に合わせて32とする｡
・PWM周期をﾍﾞｰｽとするｻﾝﾌﾟﾘﾝｸﾞ処理のﾌﾟﾘｽｹｰﾙ値は､録音時は46.9kHz/4kHz=12､
　再生時は46.9kHz/8kHz=6となる。

【ﾒﾓﾘの利用形態】
・CH32V006F8P6のﾌﾗｯｼｭﾒﾓﾘから57kﾊﾞｲﾄを音声ﾒﾓﾘに割当て､最長14.5秒間の音声を記録できる｡
・全容量を使わなかった場合は､最後に使用したｱﾄﾞﾚｽの次から記録を開始する｡

【音声有無の判定】
・音声ﾚﾍﾞﾙは､ADC結果の移動平均との差分で評価する｡
・音声ﾚﾍﾞﾙを特定の閾値と比較して､音声の有無を判定する｡
・1ｻﾝﾌﾟﾙの閾値超えで音声有りとし､一定時間の音声有りを検出しなかったら音声無しとする｡

【処理分担】
・ADCとPWMﾃﾞｭｰﾃｨ値更新は割込み処理で実行する｡
・音声ﾒﾓﾘのｱｸｾｽと全体的な制御はﾀｽｸ処理で実行する｡
・ﾀｽｸ処理と割込み処理間はﾘﾝｸﾞﾊﾞｯﾌｧを介してﾃﾞｰﾀ交換する｡

【ﾘﾝｸﾞﾊﾞｯﾌｧの扱い】
・readﾎﾟｲﾝﾀとwriteﾎﾟｲﾝﾀが等しいときはﾃﾞｰﾀ無し｡
・writeﾎﾟｲﾝﾀを進めてみてreadﾎﾟｲﾝﾀに等しくなるときは空き無し｡
・ﾊﾞｯﾌｧﾌﾙのときは､割込み処理ではﾃﾞｰﾀを捨てる､ﾀｽｸ処理では空くまで待つ｡

【繰返し再生ﾊﾞｯﾌｧ(兼先行ﾊﾞｯﾌｧ)の扱い】
・readﾎﾟｲﾝﾀとwriteﾎﾟｲﾝﾀが等しいときはﾃﾞｰﾀ無し｡
・writeﾎﾟｲﾝﾀを進めてみてreadﾎﾟｲﾝﾀに等しくなるときは空き無し｡
・空きがないときは最古のデータを捨てて書込む｡

【処理方式】
・動作ﾓｰﾄﾞを変数modeで管理していて､ﾀｽｸ処理と割込み処理で処理が競合しないようにしている｡
・ﾏｲｸｱﾝﾌﾟ回路への通電とADCの有効無効の制御は録音処理で行う｡

【ﾌﾗｯｼｭの消去/書込み処理時間待ちへの対応】
・ﾌﾗｯｼｭ側で処理が実行されている間はCPUからのﾌﾗｯｼｭｱｸｾｽが待たされる｡
・ﾍﾞｸﾀﾃｰﾌﾞﾙ/ISRとその中で実行される関数/ﾌﾗｯｼｭの消去/書込み処理関数をRAM上で実行する｡


//==============================================
//CH32Vのﾋﾟﾝ接
//==============================================
CH32V006F8P6のﾎﾟｰﾄの割当て
1:(PD4)(OPAout)ﾏｲｸｱﾝﾌﾟ出力(波形観測用)
2:(PD5)/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報
3:(PD6)ﾏｲｸ回路電源
4:(PD7)RST
5:(PA1)(OPA-in)GND間に利得調整4.7kΩを繋ぐ
6:(PA2)(OPA+in)ﾏｲｸ入力(直流分はVDD/2程度が適切)
7:VSS
8:(PD0)
9:VDD
10:(PC0)ﾃﾞﾊﾞｸﾞ信号
11:(PC1)ﾓｰﾀｰ制御
12:(PC2)
13:(PC3)/(T1CH3)[ｼﾝｸﾞﾙ･ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ]正相出力/(T1CH1N)[ｺﾝﾌﾟﾘ]逆相出力
14:(PC4)/(T1CH1)[ｺﾝﾌﾟﾘ]正相出力/(T1CH4)[ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ]逆相出力
15:(PC5)
16:(PC6)
17:(PC7)
18:(PD1)/(SWIO)
19:(PD2)
20:(PD3)
*/


//==============================================
//ﾍｯﾀﾞﾌｧｲﾙの呼込み
//==============================================
#include <ch32v00X.h>				//ｼｽﾃﾑのﾍｯﾀﾞﾌｧｲﾙ


//==============================================
//構成情報(ｱﾌﾟﾘｹｰｼｮﾝとﾀｰｹﾞｯﾄﾃﾞﾊﾞｲｽに関わるもの)
//==============================================
//ｼｽﾃﾑｸﾛｯｸ
#define	SYS_CLK	48000				//ｼｽﾃﾑｸﾛｯｸ[kHz]

//出力ﾀｲﾌﾟ
#define	BTL		0			//音声信号出力ﾀｲﾌﾟ
						//　0=ｼﾝｸﾞﾙ､1=ｺﾝﾌﾟﾘ､2=ﾌﾞﾘｯｼﾞ､3=ﾌﾞﾚｰｷ	

//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ定義
#define	DBS_GPIO	GPIOC			//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ
#define	DBS_BIT		0			//ﾃﾞﾊﾞｸﾞ信号のﾋﾞｯﾄ

//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ定義
#define	DBG_GPIO	GPIOD			//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ
#define	DBG_BIT		5			//ﾃﾞﾊﾞｸﾞ情報のﾋﾞｯﾄ

//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOC			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	3			//PWM正相のﾋﾞｯﾄ
#define	PWM_R_GPIO	GPIOC			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	4			//PWM逆相のﾋﾞｯﾄ
#define	PWM_C_GPIO	GPIOC			//PWM相補のﾎﾟｰﾄ
#define	PWM_C_BIT	4			//PWM相補のﾋﾞｯﾄ

//ﾏｲｸ電源のﾎﾟｰﾄ定義
#define	MIC_GPIO	GPIOD			//ﾏｲｸ電源のﾎﾟｰﾄ
#define	MIC_BIT		6			//ﾏｲｸ電源のﾋﾞｯﾄ

//ﾓｰﾀｰのﾎﾟｰﾄ定義
#define	MTR_GPIO	GPIOC			//MTRのﾎﾟｰﾄ
#define	MTR_BIT		1			//MTRのﾋﾞｯﾄ


//==============================================
//ｱﾌﾟﾘｹｰｼｮﾝの定数宣言
//==============================================
#define	PWM_STEP	1024			//PWMｽﾃｯﾌﾟ数
#define	PWM_PR		(PWM_STEP*1000/SYS_CLK)	//PWM周期=21.3[us]
#define	REC_PS		6	//=128us	//録音時ﾃﾞｰﾀ書出しﾌﾟﾘｽｹｰﾙ値[PWM周期]
#define	PLY_PS		3	//=64us		//再生時ﾃﾞｰﾀ読込みﾌﾟﾘｽｹｰﾙ値[PWM周期]
#define	AVE_BITS_LPF	4			//移動平均ｼﾌﾄﾋﾞｯﾄ数(LPF用)
#define	AVE_BITS_BIAS	12			//移動平均ｼﾌﾄﾋﾞｯﾄ数(ﾊﾞｲｱｽ用)
#define	VOICE_ON	400			//音声有り判定の閾値
						//　(平均値を超える値)
#define	VOICE_OFF	240			//音声無し判定の閾値
						//　(平均値を超える値)
#define	BUF_SIZE	200			//ﾘﾝｸﾞﾊﾞｯﾌｧの大きさ[ﾊﾞｲﾄ]
#define	ZERO_TIME	200	//=12.8ms	//ｾﾞﾛｸﾛｽ区間の最少時間[64us]
#define	NO_VOICE	2000	//=256ms	//音声無し判定の時間[128us]
#define	MTR_TEISI	300			//ﾓｰﾀｰ停止時間[ms]
#define	SLEEP_TMR	1172000	//=300s		//Sleep時限[256us]
#define	R_BUF_SZ	2048	//=131ms	//繰返し再生ﾊﾞｯﾌｧの大きさ[16ﾋﾞｯﾄ]
#define	R_BUF_SZ_P	1024	//=66ms		//先行再生ﾊﾞｯﾌｧの大きさ[16ﾋﾞｯﾄ]


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#include "../../common.c"			//共通ﾓｼﾞｭｰﾙを呼込む


//==============================================
//ﾌﾗｯｼｭﾒﾓﾘ関連定数
//ｱｾﾝﾌﾞﾗｿｰｽと定数値を一致させること
//==============================================
#define	MM_SEC		1024			//ﾌﾗｯｼｭﾒﾓﾘのｾｸﾀｻｲｽﾞ[ﾊﾞｲﾄ]
#define	MM_PAGE		256			//ﾌﾗｯｼｭﾒﾓﾘのﾍﾟｰｼﾞｻｲｽﾞ[ﾊﾞｲﾄ]
#define	MM_SIZE		57			//ﾌﾗｯｼｭﾒﾓﾘの大きさ[ｾｸﾀ]
//#define	MM_SIZE		10		//ﾌﾗｯｼｭﾒﾓﾘの大きさ[ｾｸﾀ]ﾃﾞﾊﾞｸﾞ用
extern volatile unsigned char eepdata[MM_SEC*MM_SIZE];
						//EEPROM実体


//==============================================
//大域ﾃﾞｰﾀ
//==============================================
static volatile unsigned char mode;		//動作ﾓｰﾄﾞ(0=ｱｲﾄﾞﾙ､1=再生､2=録音)
static volatile unsigned char tim_ps;		//TMR割込みｿﾌﾄﾌﾟﾘｽｹｰﾗ
static volatile unsigned short buf[BUF_SIZE];	//PCMﾃﾞｰﾀﾊﾞｯﾌｧ
static volatile unsigned char buf_read;		//PCMﾃﾞｰﾀ読込みﾎﾟｲﾝﾀ
static volatile unsigned char buf_write;	//PCMﾃﾞｰﾀ書込みﾎﾟｲﾝﾀ
static volatile unsigned long lpf_acc;		//LPF積算値
static volatile unsigned short lpf_ave;		//LPF平均値
static volatile unsigned long bias_acc;		//BIAS積算値
static volatile unsigned short bias_ave;	//BIAS平均値
#if R_BUF_SZ>R_BUF_SZ_P			//R_BUF_SZとR_BUF_SZ_Pの大きい方を採る
#define	R_BUF_SIZE	R_BUF_SZ
#else
#define	R_BUF_SIZE	R_BUF_SZ_P
#endif
static volatile unsigned short r_buf[R_BUF_SIZE];
						//繰返し再生(兼先行)ﾊﾞｯﾌｧ
static volatile unsigned short r_buf_read;	//繰返し再生(兼先行)読込みﾎﾟｲﾝﾀ
static volatile unsigned short r_buf_write;	//繰返し再生(兼先行)書込みﾎﾟｲﾝﾀ


//==============================================
//ﾌﾗｯｼｭｱｸｾｽ関数
//==============================================
//ch32v00X.hの間違いを是正する
#define	_FLASH_STATR_BSY	((uint32_t)0x01)	//Busy
#define	_FLASH_CTLR_PER		((uint32_t)0x0002)	//Page Erase 1KByte
#define	_FLASH_CTLR_STRT	((uint32_t)0x0040)	//Start
#define	_FLASH_CTLR_PAGE_PG	((uint32_t)0x00010000)	//Page Programming 256Byte
#define	_FLASH_CTLR_BUF_RST	((uint32_t)0x00080000)	//Buffer Reset
#define	_FLASH_CTLR_BUF_LOAD	((uint32_t)0x00040000)	//Buffer Load


//消去処理
__attribute__((section(".ramfunc")))
static void EEPDATA_erase(
	unsigned long addr)			//消去するｱﾄﾞﾚｽ(1kﾊﾞｲﾄ境界)
{

//DBG_C('\n');
//DBG_C('e');
//DBG_L(addr);

#ifndef NO_WRITE			//ﾌﾗｯｼｭに書込むとき
	FLASH->KEYR=0x45670123;			//ﾌﾗｯｼｭ解錠
	FLASH->KEYR=0xcdef89ab;
	FLASH->CTLR|=_FLASH_CTLR_PER;		//ｾｸﾀ(1kﾊﾞｲﾄ)消去
	FLASH->ADDR=(addr+(unsigned long)eepdata)|0x08000000;
						//消去するｱﾄﾞﾚｽ
	FLASH->CTLR|=_FLASH_CTLR_STRT;		//消去開始
	while(FLASH->STATR&_FLASH_STATR_BSY){};	//完了を待つ
	FLASH->CTLR&=~_FLASH_CTLR_PER;		//ｾｸﾀ(1kﾊﾞｲﾄ)消去終了
#endif
}


//書込み開始
static void EEPDATA_write_start(void)
{

//DBG_C('s');

#ifndef NO_WRITE			//ﾌﾗｯｼｭに書込むとき
	FLASH->KEYR=0x45670123;			//ﾌﾗｯｼｭ解錠
	FLASH->KEYR=0xcdef89ab;
	FLASH->MODEKEYR=0x45670123;		//Fastﾓｰﾄﾞ解錠
	FLASH->MODEKEYR=0xcdef89ab;
	FLASH->CTLR|=_FLASH_CTLR_PAGE_PG;	//Fast書込み有効
	FLASH->CTLR|=_FLASH_CTLR_BUF_RST;	//ﾊﾞｯﾌｧｸﾘｱ
	while(FLASH->STATR&_FLASH_STATR_BSY){};	//完了を待つ
#endif
}


//4ﾊﾞｲﾄ単位書込み
static void EEPDATA_write_data(
	unsigned long addr,			//書込むｱﾄﾞﾚｽ(4ﾊﾞｲﾄ境界)
	unsigned long data)			//書込むﾃﾞｰﾀ(4ﾊﾞｲﾄ)
{

//DBG_C('w');
//DBG_L(addr);
//DBG_C('-');
//DBG_L(data);

#ifndef NO_WRITE			//ﾌﾗｯｼｭに書込むとき
	*(uint32_t*)((addr+(unsigned long)eepdata)|0x08000000)=data;
						//ﾃﾞｰﾀ書込み
	FLASH->CTLR|=_FLASH_CTLR_BUF_LOAD;	//ﾊﾞｯﾌｧへﾛｰﾄﾞ
	while(FLASH->STATR&_FLASH_STATR_BSY){};	//完了を待つ
#endif
}


//書込み終了
__attribute__((section(".ramfunc")))
void EEPDATA_write_end(
	unsigned long addr)			//書込み開始ｱﾄﾞﾚｽ(256ﾊﾞｲﾄ境界)
{

//DBG_C('t');
//DBG_L(addr);

#ifndef NO_WRITE			//ﾌﾗｯｼｭに書込むとき
	FLASH->ADDR=(addr+(unsigned long)eepdata)|0x08000000;
						//書込み開始ｱﾄﾞﾚｽ設定
//DBG_L(FLASH->ADDR);
	FLASH->CTLR|=_FLASH_CTLR_STRT;		//書込み開始
	while(FLASH->STATR&_FLASH_STATR_BSY){};	//完了を待つ
	FLASH->CTLR&=~_FLASH_CTLR_PAGE_PG;	//Fast書込み終了
#endif
}


//==============================================
//ﾊﾞｯﾌｧへ詰める
//ﾊﾞｯﾌｧﾌﾙの場合は
//　ﾀｽｸ処理(ﾌﾗｯｼｭ→ﾊﾞｯﾌｧ)では待合せる
//　割込み処理(ADC→ﾊﾞｯﾌｧ)では空振りさせる
//==============================================
__attribute__((section(".ramfunc")))
void put_buf(
	unsigned char exec,			//実行種別(0=ﾀｽｸ処理､1=割込み処理)
	unsigned short data)			//転送ﾃﾞｰﾀ
{
	unsigned char n;


	n=buf_write;				//書込みﾎﾟｲﾝﾀを取得する
	if(++n==BUF_SIZE) n=0;			//書込み位置を仮に進める
	for(;;)
	{
		if(n!=buf_read) break;		//空きがあれば抜ける
		if(exec) return;		//割込み処理(ADC→ﾊﾞｯﾌｧ)では空振りさせる
	}
	buf[buf_write]=data;			//ﾃﾞｰﾀをﾊﾞｯﾌｧに書込む
	buf_write=n;				//書込みﾎﾟｲﾝﾀを更新する
}


//==============================================
//ﾊﾞｯﾌｧから取出す
//ﾊﾞｯﾌｧが空の場合は
//　ﾀｽｸ処理(ﾊﾞｯﾌｧ→ﾌﾗｯｼｭ)では待合せる
//　割込み処理(ﾊﾞｯﾌｧ→PWM)では空振りさせる
//==============================================
static unsigned short get_buf(			//ﾃﾞｰﾀを返す
	unsigned char exec)			//実行種別(0=ﾀｽｸ処理､1=割込み処理)
{
	unsigned short data;

	for(;;)
	{
		if(buf_read!=buf_write) break;	//ﾃﾞｰﾀが有ったら抜ける
		{
			if(!exec) continue;	//ﾀｽｸ処理ではﾃﾞｰﾀを待つ
			return(buf[buf_read]);	//割込み処理では前回ﾃﾞｰﾀを返す
		}
	}
	data=buf[buf_read];			//ﾃﾞｰﾀを取出す
	if(buf_read==(BUF_SIZE-1)) buf_read=0;	//ﾊﾞｯﾌｧの後尾になったら
						//　先頭に戻す
	else buf_read++;			//ﾊﾞｯﾌｧを進める
	return(data);
}


//==============================================
//ﾀｲﾏｰ更新割込みﾊﾝﾄﾞﾗ
//再生処理
//・ｿﾌﾄﾌﾟﾘｽｹｰﾙ(PLY_PS)周期で､ﾘﾝｸﾞﾊﾞｯﾌｧからPCM値を取り出しPWMﾃﾞｭｰﾃｨｻｲｸﾙを更新する
//・ﾊﾞｯﾌｧｱﾝﾀﾞｰﾗﾝ時はPWMﾃﾞｭｰﾃｨｻｲｸﾙを前回の値を継続する
//録音処理
//・LPF移動平均とBIAS移動平均を計算する
//・BIAS移動平均との差分をPCM値とする
//・ｿﾌﾄﾌﾟﾘｽｹｰﾙ(REC_PS)周期で､PCM値をﾘﾝｸﾞﾊﾞｯﾌｧに格納する
//・ﾊﾞｯﾌｧｵｰﾊﾞｰﾗﾝ時はADC結果を捨てる
//==============================================
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
__attribute__((section(".ramfunc")))
void TIM1_UP_IRQHandler(void)
{

	unsigned short data;			//PCMﾃﾞｰﾀ

	//共通処理	
	TIM1->INTFR=0;				//全ｲﾍﾞﾝﾄをｸﾘｱする

	//再生処理	
	if(mode==1)				//再生ﾓｰﾄﾞのとき
	{
//DBG_C('\n');
		if(++tim_ps==PLY_PS)		//再生ﾀｲﾐﾝｸﾞになったら
		{
DBS_H;
			tim_ps=0;		//ﾌﾟﾘｽｹｰﾗを初期化する
			data=get_buf(1)/(32768/PWM_STEP);
						//ﾊﾞｯﾌｧからPCM値をPWM_STEP*2のｽｹｰﾙで取出す
//DBG_B(data);
			//ﾃﾞｭｰﾃｨｻｲｸﾙを設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
			TIM1->CH3CVR=data/2;	//正相を設定する
#endif
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
			TIM1->CH1CVR=data/2;	//正相･逆相を設定する
#endif
#if BTL>1				//ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ出力のとき
			if(data>PWM_STEP)	//正値のとき
			{
				TIM1->CH4CVR=0;	//逆相を止める
				TIM1->CH3CVR=data-PWM_STEP;
						//正相を設定する
			}
			else			//負値のとき
			{
				TIM1->CH3CVR=0;	//正相を止める
				TIM1->CH4CVR=PWM_STEP-data;
						//逆相を設定する
			}
#endif
DBS_L;
		}
	}

	//録音処理	
	else if(mode==2)			//録音ﾓｰﾄﾞのとき
	{
//DBG_C('\n');

		//ADC結果の取込み
		while(!(ADC1->STATR&ADC_EOC));	//変換完了を待つ
		data=ADC1->RDATAR;		//ADC結果を取込む
//DBG_S(data);
		ADC1->CTLR2|=ADC_ADON;		//次の変換開始

		//LPF計算
		lpf_acc=lpf_acc-lpf_ave+data;	//積算
//DBG_C('-');
//DBG_L(lpf_acc);
		lpf_ave=lpf_acc>>AVE_BITS_LPF;	//平均
//DBG_C('=');
//DBG_S(lpf_ave);

		//BIAS計算
		bias_acc=bias_acc-bias_ave+data;//積算
//DBG_C('-');
//DBG_L(bias_acc);
		bias_ave=bias_acc>>AVE_BITS_BIAS;
						//平均
//DBG_C('=');
//DBG_S(bias_ave);

		//PCMﾃﾞｰﾀの記録
//DBG_C('@');
//DBG_B(tim_ps);
		if(++tim_ps==REC_PS)		//録音ﾀｲﾐﾝｸﾞになったら
		{
DBS_H;
			tim_ps=0;		//ﾌﾟﾘｽｹｰﾗを初期化する
			data=lpf_ave;
//DBG_C('-');
//DBG_S(data);

			//ADC値の正規化
			if(data>bias_ave)
			{
				data=data-bias_ave;
				if(data>1023) data=1023;
				data=1024+data;
			}
			else
			{
				data=bias_ave-data;
				if(data>1023) data=1023;
				data=1024-data;
			}
//DBG_C('=');
//DBG_B(data);
			//ﾊﾞｯﾌｧへ書出し
			put_buf(1,data);	//評価値をﾊﾞｯﾌｧに格納する
DBS_L;
		}
	}
}


//==============================================
//IMA-ADPCM制御ﾃｰﾌﾞﾙ
//==============================================
//ｽﾃｯﾌﾟｻｲｽﾞﾃｰﾌﾞﾙ(89段階)
static const unsigned short STEP_TABLE[89]=
	{
	7,8,9,10,11,12,13,14,
	16,17,19,21,23,25,28,31,
	34,37,41,45,50,55,60,66,
	73,80,88,97,107,118,130,143,
	157,173,190,209,230,253,279,307,
	337,371,408,449,494,544,598,658,
	724,796,876,963,1060,1166,1282,1411,
	1552,1707,1878,2066,2272,2499,2749,3024,
	3327,3660,4026,4428,4871,5358,5894,6484,
	7132,7845,8630,9493,10442,11487,12635,13899,
	15289,16818,18500,20350,22385,24623,27086,29794,
	32767
	};

//ｲﾝﾃﾞｯｸｽ調整ﾃｰﾌﾞﾙ(ｴﾝｺｰﾄﾞ値の下位3bitで引く)
static const short INDEX_TABLE[8]=
	{
	-1,-1,-1,-1,2,4,6,8
};

static unsigned short predicted;		//直前の予測値
static short step_index;			//ｽﾃｯﾌﾟｲﾝﾃﾞｯｸｽ(0〜88)
static unsigned short step;


//==============================================
//IMA-ADPCM共通処理
//==============================================
static void ima_step(
	unsigned char nibble)			//ｴﾝｺｰﾄﾞ値(4ﾋﾞｯﾄADPCM値)
{
	unsigned short delta;

	//予測値を更新
	delta=step>>3;
	if(nibble&4) delta+=step;
	step>>=1;
	if(nibble&2) delta+=step;
	step>>=1;
	if(nibble&1) delta+=step;
	if(nibble&8)
	{
		if(predicted<delta) predicted=0;
		else predicted-=delta;
	}
	else
	{
		if((65535-predicted)<delta) predicted=65535;
		else predicted+=delta;
	}

	//step_indexを更新
	step_index+=INDEX_TABLE[nibble&7];
	if(step_index<0) step_index=0;
	else if(step_index>88) step_index=88;
}


//==============================================
//IMA-ADPCMﾃﾞｺｰﾄﾞ処理
//==============================================
static unsigned short ima_decode(		//ﾃﾞｺｰﾄﾞ値(16ﾋﾞｯﾄPCM値)を返す
						//　0ﾚﾍﾞﾙは32768
	unsigned char nibble)			//ｴﾝｺｰﾄﾞ値(4ﾋﾞｯﾄADPCM値)
{

	//ﾃﾞｺｰﾄﾞの初期化
	step=STEP_TABLE[step_index];

	//共通処理
	ima_step(nibble);

	return(predicted);
}
	

//==============================================
//IMA-ADPCMｴﾝｺｰﾄﾞ処理
//==============================================
static unsigned char ima_encode(		//ｴﾝｺｰﾄﾞ値(4ﾋﾞｯﾄADPCM値)を返す
	unsigned short sample)			//ｻﾝﾌﾟﾙ値(16ﾋﾞｯﾄPCM値)
						//　0ﾚﾍﾞﾙは32768
{
	long diff;
	unsigned char nibble;

	//ｴﾝｺｰﾄﾞの初期化
	step=STEP_TABLE[step_index];
	diff=sample-predicted;
	nibble=0;

	//bit3:符号ﾋﾞｯﾄ
	if(diff<0)
	{
		nibble=8;
		diff=-diff;
	}

	//bit2〜0:差分をｽﾃｯﾌﾟｻｲｽﾞで量子化
	if(diff>=step)
	{
		nibble|=4;
		diff-=step;
	}
	if(diff>=step>>1)
	{
		nibble|=2;
		diff-=step>>1;
	}
	if(diff>=step>>2)
	{
		nibble|=1;
	}

	//共通処理
	ima_step(nibble);

	return(nibble);
}


//==============================================
//main
//==============================================
int main(void)
{
	unsigned long sleep_tmr;	//SLEEPﾀｲﾏｰ
	unsigned long ad_st;		//録音の開始ｱﾄﾞﾚｽ(eepdata上のﾆﾌﾞﾙｵﾌｾｯﾄ)
	unsigned long ad_stop;		//終了ｱﾄﾞﾚｽ(eepdata上のﾆﾌﾞﾙｵﾌｾｯﾄ)
	unsigned long ad;		//ｶﾚﾝﾄｱﾄﾞﾚｽ(eepdata上のﾆﾌﾞﾙｵﾌｾｯﾄ)
	unsigned char nibble;		//ｴﾝｺｰﾄﾞ値
	unsigned short data;		//音声ﾃﾞｰﾀ
	unsigned long data8=0;		//音声ﾃﾞｰﾀ(8個で1ﾜｰﾄﾞ)
	unsigned short timer;		//計時ｶｳﾝﾀ
	unsigned char zero_hst;		//ｾﾞﾛｸﾛｽ検知履歴
	unsigned long zero_time;	//ｾﾞﾛｸﾛｽ区間ｶｳﾝﾀ
	unsigned short n;

	//ｸﾛｯｸ供給設定
	RCC->PB2PCENR|=RCC_PB2Periph_GPIOA|
			RCC_PB2Periph_GPIOC|
			RCC_PB2Periph_GPIOD|
			RCC_PB2Periph_TIM1|
			RCC_PB2Periph_USART1|
			RCC_PB2Periph_AFIO|
			RCC_PB2Periph_ADC1;
	RCC->PB1PCENR|=RCC_PB1Periph_PWR;

	//GPIO設定
	GPIOA->CFGLR=GPIOC->CFGLR=0x00000000;	//ﾎﾟｰﾄA､Cをｱﾅﾛｸﾞ入力にしておく
	GPIOD->CFGLR=0x00000080;		//PD1以外をｱﾅﾛｸﾞ入力にしておく

	//ﾃﾞﾊﾞｸﾞ信号設定
#ifdef SHDBS				//ﾃﾞﾊﾞｸﾞ信号を出力するとき
	dbs_init();				//ﾃﾞﾊﾞｸﾞ情報を初期化する
//待ち時間のﾃｽﾄ
//for(;;){wait_us(10);DBS_H;wait_us(10);DBS_L;}
#endif

	//ﾃﾞﾊﾞｸﾞ情報設定
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	dbg_init();				//ﾃﾞﾊﾞｸﾞ情報を初期化する
//ﾃﾞﾊﾞｸﾞ情報出力のﾃｽﾄ
//for(;;){DBG_C('U');}
#endif

DBG_C('\n');
DBG_C('!');

	//ﾏｲｸ電源ﾎﾟｰﾄ
	GPIO_H(MIC_GPIO,MIC_BIT);		//ﾏｲｸ電源をH
	GPIO_OUT(MIC_GPIO,MIC_BIT);		//ﾏｲｸ電源を出力

	//ﾓｰﾀｰ制御ﾎﾟｰﾄ
	GPIO_L(MTR_GPIO,MTR_BIT);		//ﾓｰﾀｰ制御をH
	GPIO_OUT(MTR_GPIO,MTR_BIT);		//ﾓｰﾀｰ制御を出力

	//OPA設定	//ﾎﾟｰﾄ割当ての自由度は無いので､この構成で固定する
			//ﾏｲｸｱﾝﾌﾟ回路の調整のため､OPA出力をPD4に出力しておく
	OPA->OPAKEY=0x45670123;			//OPAﾛｯｸ解除1
	OPA->OPAKEY=0xcdef89ab;			//OPAﾛｯｸ解除2
	OPA->CTLR1=OPA_CTLR1_FB_EN1|OPA_CTLR1_OPA_EN1;
						//負帰還抵抗(192k)有効､OPA有効
//DBG_L(OPA->CTLR1);
	GPIO_AN(GPIOA,1);			//PA1はOPN0
	GPIO_AN(GPIOA,2);			//PA2はOPP0
	GPIO_AF(GPIOD,4);			//PD4はOPO
	wait_ms(200);				//OPA安定時間

	//ADC設定
	ADC1->CTLR2=ADC_ADON|ADC_SWSTART;	//ADC有効､ｿﾌﾄﾄﾘｶﾞでruleﾁｬﾈﾙ変換
	while(!(ADC1->CTLR2&ADC_ADON));		//ADC有効を待つ
	ADC1->SAMPTR2=0x00000000;		//ｻﾝﾌﾟﾙﾀｲﾑは3ｻｲｸﾙ
	ADC1->RSQR3=9;				//ｼｰｹﾝｽ1をOPAに設定する

	//PWM設定
	TIM1->ATRLR=PWM_STEP-1;			//自動ﾘﾛｰﾄﾞ値(PWM周期)
	TIM1->SWEVGR=0x0001;			//更新ｲﾍﾞﾝﾄを発生させる
	TIM1->CHCTLR1=0x0068;			//CH1をPWMﾓｰﾄﾞ1､ﾌﾟﾘﾛｰﾄﾞ有効
	TIM1->CHCTLR2=0x6868;			//CH3とCH4をPWMﾓｰﾄﾞ1､ﾌﾟﾘﾛｰﾄﾞ有効
	TIM1->BDTR=TIM_MOE;			//出力有効
	TIM1->CTLR1=TIM_ARPE|TIM_CEN;		//自動ﾘﾛｰﾄﾞﾌﾟﾘﾛｰﾄﾞ有効､TMR有効
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
	TIM1->CH1CVR=PWM_STEP/2;		//初期出力を50%ﾃﾞｭｰﾃｨにする
#endif
	TIM1->INTFR=~TIM_IT_Update;		//更新ｲﾍﾞﾝﾄをｸﾘｱする
	while(!(TIM1->INTFR&TIM_IT_Update));	//更新ｲﾍﾞﾝﾄを待つ
	GPIO_AF(PWM_F_GPIO,PWM_F_BIT);		//正相を交代機能に設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
	TIM1->CCER=0x0100;			//CC3を非反転出力
#endif
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
	TIM1->CCER=0x0005;			//CC1を非反転出力､CC1Nを非反転出力
	TIM1->BDTR|=PWM_DB_CMP;			//ﾃﾞｯﾄﾞﾊﾞﾝﾄﾞを設定する
	GPIO_AF(PWM_C_GPIO,PWM_C_BIT);		//相補を交代機能に設定
#endif
#if BTL==2				//ﾌﾞﾘｯｼﾞ出力のとき
	TIM1->CCER=0x1100;			//CC3を非反転出力､CC4を非反転出力
	GPIO_AF(PWM_R_GPIO,PWM_R_BIT);		//逆相を交代機能に設定
#endif
#if BTL==3				//ﾌﾞﾚｰｷ出力のとき
	TIM1->CCER=0x3300;			//CC3を反転出力､CC4を反転出力
	GPIO_AF(PWM_R_GPIO,PWM_R_BIT);		//逆相を交代機能に設定
#endif
	TIM1->DMAINTENR=TIM_IT_Update;		//更新ｲﾍﾞﾝﾄ割込み許可
	NVIC_EnableIRQ(TIM1_UP_IRQn);		//TIM1更新ｲﾍﾞﾝﾄ割込み許可(Coreﾏｸﾛ)


//OPA-ADCﾃｽﾄ////////////////////////////////////
#if 0
	unsigned short value;
	for(;;)
	{
		wait_ms(10);
		ADC1->CTLR2|=ADC_ADON;		//変換開始
		while(!(ADC1->STATR&ADC_EOC));	//変換完了を待つ
		value=ADC1->RDATAR;		//変換結果を取出す
DBG_C('\n');
DBG_S(value);
	}
#endif
////////////////////////////////////////////////


	//ﾌﾗｯｼｭﾒﾓﾘのｱﾄﾞﾚｽ初期化
//DBG_L(flash_base);
	ad_st=0;				//録音開始ｱﾄﾞﾚｽ
						//　ﾘｾｯﾄ後はeepdataの先頭､
						//　2回目以降は前回終了時の
						//　次ﾍﾟｰｼﾞに位置付けることで
						//　ﾌﾗｯｼｭの消耗を均等にする

	//ADCﾃﾞｰﾀ初期設定
	ADC1->CTLR2|=ADC_ADON;			//最初の変換開始
	while(!(ADC1->STATR&ADC_EOC));		//変換完了を待つ
	lpf_ave=bias_ave=ADC1->RDATAR;		//ADC結果を平均値に取込む
DBG_S(lpf_ave);
	lpf_acc=lpf_ave<<AVE_BITS_LPF;		//LPF用積算値を初期化
DBG_C('-');
DBG_L(lpf_acc);
	bias_acc=bias_ave<<AVE_BITS_BIAS;	//BIAS用積算値を初期化
DBG_C('-');
DBG_L(bias_acc);
	ADC1->CTLR2|=ADC_ADON;			//次の変換を開始しておく

	//ﾒｲﾝﾙｰﾌﾟ
	for(;;)
	{
DBG_C('\n');
DBG_C('>');

		//ﾓｰﾀｰ停止を待つ
DBG_C('a');
		wait_ms(MTR_TEISI);		//ﾓｰﾀｰ停止時間

		//録音処理の設定
DBG_C('b');
		ad=ad_st;			//録音開始位置をｶﾚﾝﾄｱﾄﾞﾚｽに設定
DBG_L(ad);
		tim_ps=0;			//ﾀｲﾏｰ割込みｿﾌﾄﾌﾟﾘｽｹをｸﾘｱしておく
		buf_read=buf_write=0;		//ﾊﾞｯﾌｧをｸﾘｱしておく
		r_buf_read=r_buf_write=0;	//先行ﾊﾞｯﾌｧをｸﾘｱしておく
		mode=2;				//録音ﾓｰﾄﾞにする

		//音声を待つ
		sleep_tmr=0;
		for(;;)
		{
			//ﾘｱﾙﾀｲﾑﾃﾞｰﾀをﾁｪｯｸ
			data=get_buf(0);	//音声ﾃﾞｰﾀを取出す
//DBG_C('=');
//DBG_S(data);

			//先行ﾊﾞｯﾌｧに記録する
//DBG_C('c');
			r_buf[r_buf_write]=data;//書込み位置に格納する
			if(++r_buf_write==R_BUF_SZ_P) r_buf_write=0;
						//ﾊﾞｯﾌｧの後尾になったら先頭に戻す
			if(r_buf_read==r_buf_write)
			{
				if(++r_buf_read==R_BUF_SZ_P) r_buf_read=0;
						//ﾊﾞｯﾌｧの後尾になったら先頭に戻す
			}

			//音声検知
//DBG_C('d');
			if(data>(1024+VOICE_ON)) break;
			if(data<(1024-VOICE_ON)) break;
#ifdef SLEEP_EN				//Sleepするとき
			if(sleep_tmr++>SLEEP_TMR)
						//Sleep時限になったら
			{
DBG_C('e');
DBG_SYNC();
				GPIOA->CFGLR=GPIOC->CFGLR=GPIOD->CFGLR=0x00000000;
							//全ﾎﾟｰﾄをｱﾅﾛｸﾞ入力にしておく
				ADC1->CTLR2=0;		//ADC無効
				OPA->CTLR1=0;		//OPA無効
				GPIO_L(MIC_GPIO,MIC_BIT);
							//ﾏｲｸ電源をｵﾌ
				PWR->CTLR|=0x00000002;	//PDDSをｾｯﾄ
				NVIC->SCTLR|=0x04;	//SLEEPDEEPをｾｯﾄ
				__WFE();		//StandbyModeへ移行する
				//ここには到達しない	
			}
#endif
		}

		//録音の準備
DBG_C('f');
		predicted=32768;		//IMA-ADPCM保管値を初期化
		step_index=0;			//IMA-ADPCMｽﾃｯﾌﾟ値を初期化
		timer=0;			//音声無し計時値をｸﾘｱしておく

		//録音する
		for(;;)
		{
DBG_C('\n');
DBG_L(ad);
			if(!(ad&(MM_SEC*2-1)))	//ｾｸﾀ境界のときはｾｸﾀ消去する
			{
//DBG_C('g');
//DBG_L(ad);
				EEPDATA_erase(ad/2);
			}
			if(!(ad&(MM_PAGE*2-1)))	//ﾍﾟ-ｼﾞ境界のときは書込み開始する
			{
//DBG_C('h');
//DBG_L(ad);
				EEPDATA_write_start();
			}

			//先行ﾃﾞｰﾀを録音する
			if(r_buf_read!=r_buf_write)	//先行ﾃﾞｰﾀがあれば
			{
DBG_C('i');
				data=r_buf[r_buf_read];	//先行ﾃﾞｰﾀを取り出す
				if(++r_buf_read==R_BUF_SZ_P) r_buf_read=0;
							//読出しﾎﾟｲﾝﾀを進めて
							//　後尾になったら先頭に戻す
			}

			//ﾘｱﾙﾀｲﾑﾃﾞｰﾀを録音する
			else
			{
DBG_C('j');
				data=get_buf(0);	//音声ﾃﾞｰﾀを取出す
			}
DBG_C(',');
DBG_S(data);
DBG_C(',');
DBG_S(data*65536/2048);
			nibble=ima_encode(data*65536/2048);
DBG_C(',');
DBG_B(nibble);
			data8=(data8>>4)+((unsigned long)nibble<<28);
						//8個ﾊﾞｯﾌｧに入れる
//DBG_C('=');
//DBG_L(data8);
			if((ad&7)==7)		//8個溜まったら書込む
			{
//DBG_C('k');
				EEPDATA_write_data(ad/2&~3,data8);
			}
			if((ad&(MM_PAGE*2-1))==(MM_PAGE*2-1))
						//ﾍﾟ-ｼﾞの最後のときは
						//　書込み終了する
			{
//DBG_C('l');
//DBG_L(ad);
				EEPDATA_write_end(ad/2&~(MM_PAGE-1));
			}
			if(++ad==MM_SEC*MM_SIZE*2) ad=0;
						//ｱﾄﾞﾚｽを進めて
						//　eepdataの最後になったら
						//　先頭に戻す(ﾛｰﾙｱｯﾌﾟ)
//DBG_L(ad);
			if(ad==ad_st)
			{
//DBG_C('m');
				break;		//ﾒﾓﾘが満杯になったら抜ける
			}
			if(++timer>NO_VOICE) break;
						//音声を一定時間検出しなかったら
						//　抜ける
			else
			{
DBG_C('n');
				if((data>(1024+VOICE_OFF))||(data<(1024-VOICE_OFF)))
						//音声を検出したら
				{
					timer=0;//計時値をｸﾘｱする
				}
			}
		}
DBG_C('o');
		if((ad&(MM_PAGE*2-1))!=(MM_PAGE*2-1))
						//書込み終了が未のときは
						//　書込み終了する
		{
DBG_C('p');
DBG_L(ad);
			EEPDATA_write_end(ad/2&~(MM_PAGE-1));
		}
		ad_stop=ad&~7;			//終了ｱﾄﾞﾚｽを控えておく
DBG_C('#');
DBG_L(ad_stop);
DBG_L(ad_st);
DBG_C('\n');
DBG_C('<');

		//再生処理の設定
		mode=0;				//ｱｲﾄﾞﾙにする
		zero_hst=0;			//ｾﾞﾛｸﾛｽ検出を初期化しておく
		ad=ad_st;			//再生開始ｱﾄﾞﾚｽは録音開始ｱﾄﾞﾚｽ
		predicted=32768;		//IMA-ADPCM保管値を初期化
		step_index=0;			//IMA-ADPCMｽﾃｯﾌﾟ値を初期化
		GPIO_H(MTR_GPIO,MTR_BIT);	//ﾓｰﾀｰをｵﾝ
		tim_ps=0;			//ﾀｲﾏｰ割込みｿﾌﾄﾌﾟﾘｽｹをｸﾘｱしておく
		buf_read=buf_write=0;		//ﾊﾞｯﾌｧをｸﾘｱしておく
		mode=1;				//再生ﾓｰﾄﾞにする
DBG_C('q');
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
		data=0;
		while(data<32768)		//DCが50%になるまで
		{
			data+=16;		//ﾗﾝﾌﾟ上昇させる
			put_buf(0,data);
		}
#endif

DBG_C('r');
		//区間再生の実行
		for(;;)
		{

DBG_C('\n');
DBG_C('s');
			//区間再生の準備
			zero_time=0;		//ｾﾞﾛｸﾛｽ区間ｶｳﾝﾀを初期化
			r_buf_write=0;		//繰返しﾊﾞｯﾌｧの有効ﾃﾞｰﾀ数を初期化

			//区間再生の1回目(ﾌﾗｯｼｭから）
			for(;;)
			{
DBG_C('\n');
DBG_L(ad);
				//ﾌﾗｯｼｭからﾃﾞｰﾀを取得
				nibble=eepdata[ad/2];	//eepdataからﾃﾞｰﾀを取出す
				if(ad&1) nibble>>=4;	//後行ｱﾄﾞﾚｽのときは上位4ﾋﾞｯﾄ
				else nibble&=15;	//先行ｱﾄﾞﾚｽのときは下位4ﾋﾞｯﾄ
DBG_C(',');
DBG_B(nibble);
				if(++ad==MM_SEC*MM_SIZE*2) ad=0;
							//ｱﾄﾞﾚｽを進めて
							//　最後になったら
							//　先頭に戻す(ﾛｰﾙｱｯﾌﾟ)
				data=ima_decode(nibble);
							//IMA-ADPCMﾃﾞｺｰﾄﾞする
DBG_C(',');
DBG_S(data);
				put_buf(0,data);	//PCMﾃﾞｰﾀをﾊﾞｯﾌｧに詰める

				//繰返し再生ﾊﾞｯﾌｧに詰める
				if(r_buf_write<R_BUF_SZ)//安全のためｵｰﾊﾞﾌﾛｰをﾁｪｯｸする
				{
					r_buf[r_buf_write++]=data;
							//繰返し再生用ﾊﾞｯﾌｧに記録する
				}
DBG_C('t');
				zero_hst<<=1;		//ｾﾞﾛｸﾛｽ検知履歴を進める
				if(data<32768) zero_hst|=1;
							//今回値を履歴に追加する
				if(zero_time<ZERO_TIME) zero_time++;
				else			//ｾﾞﾛｸﾛｽ最少区間を過ぎたら
				{
					if(zero_hst==1) break;
							//ｾﾞﾛｸﾛｽを検知したら抜ける
				}
				if(ad==ad_stop) break;	//終了ｱﾄﾞﾚｽに達したら抜ける
			}

			//区間再生の2回目(繰返し再生ﾊﾞｯﾌｧから)
DBG_C('\n');
DBG_C('u');
			for(n=0;n<r_buf_write;n++)
			{
				put_buf(0,r_buf[n]);	//繰返し再生用ﾊﾞｯﾌｧから
							//　ﾘﾝｸﾞﾊﾞｯﾌｧへ詰める
			}
DBG_C('\n');
DBG_C('v');
			if(ad==ad_stop) break;		//終了ｱﾄﾞﾚｽに達したら抜ける
		}

		//再生の終了
		GPIO_L(MTR_GPIO,MTR_BIT);	//ﾓｰﾀｰをｵﾌ
DBG_C('w');
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
		data&=~15;			//DCを16境界にする
		put_buf(0,data);
		while(data)			//DCが50%になるまで
		{
			put_buf(0,data);
			data-=16;		//ﾗﾝﾌﾟ下降させる
		}
#else					//ｼﾝｸﾞﾙ出力以外のとき
		if(data<32768) data+=15;	//DCを16境界にする
		data&=~15;
		for(;;)				//DCを0%までﾗﾝﾌﾟ下降する
		{
			put_buf(0,data);
			if(data>32768) data-=16;
			else if(data<32768) data+=16;
			else break;
		}
#endif
		while(buf_read!=buf_write);	//ﾊﾞｯﾌｧﾌﾗｯｼｭを待つ
		mode=0;				//ｱｲﾄﾞﾙにする

		//次の書込み開始ｱﾄﾞﾚｽを更新
DBG_C('x');
DBG_L(ad_stop);
		ad_st=ad_stop+(MM_PAGE*2-1)&~(MM_PAGE*2-1);
						//ﾍﾟｰｼﾞ境界に切上げる
DBG_C('-');
DBG_L(ad_st);
		if(ad_st>=MM_SEC*MM_SIZE*2) ad_st-=MM_SEC*MM_SIZE*2;
						//ﾛｰﾙｵｰﾊﾞｰさせる
DBG_C('=');
DBG_L(ad_st);
	}
DBG_C('y');
}
