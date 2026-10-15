#include <ch32v00x.h>

//==============================================
//ﾎﾟｰﾄｱｸｾｽのﾏｸﾛ定義
//==============================================
#define	GPIO_OUT(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)|3<<bit*4
#define	GPIO_OUTOD(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)|7<<bit*4
#define	GPIO_H(gpio,bit)	gpio->BSHR=1<<bit
#define	GPIO_L(gpio,bit)	gpio->BCR=1<<bit
#define	GPIO_IN(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)|4<<bit*4
#define	GPIO_INPU(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)|8<<bit*4,\
				gpio->BSHR=1<<bit
#define	GPIO_INPD(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)|8<<bit*4,\
				gpio->BCR=1<<bit
#define	GPIO_R(gpio,bit)	(gpio->INDR&1<<bit)
#define	GPIO_AN(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)
#define	GPIO_AF(gpio,bit)	gpio->CFGLR=gpio->CFGLR&~(15<<bit*4)|11<<bit*4
#define	GPIO_AFOD(gpio,bit)	gpio->CFGLR|=15<<bit*4


//==============================================
//ｼｽﾃﾑｸﾛｯｸ設定
//==============================================
void SystemInit(void)
{

#ifndef HSI_TRIM			//HSI校正値が指定されていないとき
#define HSI_TRIM 16				//HSI校正値を中央値にする
#endif
#ifdef DEV_V003				//CH32V003ﾃﾞﾊﾞｲｽのとき
	FLASH->ACTLR=(uint32_t)FLASH_ACTLR_LATENCY_1;
						//FLASHﾚｲﾃﾝｼは1WAIT
#else					//CH32V003ﾃﾞﾊﾞｲｽ以外のとき
	FLASH->ACTLR=(uint32_t)FLASH_ACTLR_LATENCY_2;
						//FLASHﾚｲﾃﾝｼは2WAIT
#endif
	RCC->CTLR=RCC_HSION|HSI_TRIM<<3;	//HSIｵﾝ､HSI校正
	RCC->CFGR0=RCC_PLLSRC_HSI_Mul2;		//PLLｿｰｽはHSI､2xPLL､HBﾌﾟﾘｽｹ無し
	RCC->CTLR|=RCC_PLLON;			//PLLｵﾝ	
	while(!(RCC->CTLR&RCC_PLLRDY)){};	//PLLﾚﾃﾞｨを待つ
	RCC->CFGR0|=RCC_SW_PLL;			//ｼｽﾃﾑｸﾛｯｸをPLLに切替える
	while((RCC->CFGR0&RCC_SWS)!=(uint32_t)0x08){};
						//切替わるのを待つ
}


#ifdef	SHDBS
//==============================================
//ﾃﾞﾊﾞｸﾞ信号の初期設定
//==============================================
static void dbs_init(void)
{

	GPIO_L(DBS_GPIO,DBS_BIT);		//ﾃﾞﾊﾞｸﾞ信号をL
	GPIO_OUT(DBS_GPIO,DBS_BIT);		//ﾃﾞﾊﾞｸﾞ信号を出力
}


//==============================================
//ﾃﾞﾊﾞｸﾞ信号出力のﾏｸﾛ定義
//==============================================
#define	DBS_H		GPIO_H(DBS_GPIO,DBS_BIT);
#define	DBS_L		GPIO_L(DBS_GPIO,DBS_BIT);
#else
#define DBS_H
#define DBS_L
#endif



//==============================================
//1usの待ち
//==============================================
static void wait_1us(void)
{

	SysTick->SR&=~1;			//ﾌﾗｸﾞをｸﾘｱしておく
	SysTick->CMP=1;				//比較値を設定
	SysTick->CNT=0;				//ｶｳﾝﾀをｸﾘｱ
	SysTick->CTLR|=1;			//ｶｳﾝﾄ開始
	while(!(SysTick->SR&1));		//ﾌﾗｸﾞｾｯﾄを待つ
	SysTick->CTLR&=~1;			//ｶｳﾝﾄ停止
}


//==============================================
//us単位の待ち
//==============================================
static void wait_us(
	unsigned short us)			//待ち時間[us]
{

	SysTick->SR&=~1;			//ﾌﾗｸﾞをｸﾘｱしておく
	SysTick->CMP=(unsigned long)us*SYS_CLK/8000-4;
						//比較値を設定
	SysTick->CNT=0;				//ｶｳﾝﾀをｸﾘｱ
	SysTick->CTLR|=1;			//ｶｳﾝﾄ開始
	while(!(SysTick->SR&1));		//ﾌﾗｸﾞｾｯﾄを待つ
	SysTick->CTLR&=~1;			//ｶｳﾝﾄ停止
}


//==============================================
//ms単位の待ち
//==============================================
static void wait_ms(
	unsigned short ms)			//待ち時間[ms]
{

	while(ms--)
	{
		wait_us(1000);
	}
}


#ifdef SHDBG
#ifndef SHBPS				//ﾎﾞｰﾚｰﾄが宣言されていないとき
#define	SHBPS 38400				//38400bpsとする
#endif
#ifdef SHDBG_SOFT			//UARTをｿﾌﾄ実装するとき
//==============================================
//ﾃﾞﾊﾞｸﾞ情報の初期設定
//==============================================
static void dbg_init(void)
{

	GPIO_H(DBG_GPIO,DBG_BIT);		//ﾃﾞﾊﾞｸﾞ情報をH
	GPIO_OUT(DBG_GPIO,DBG_BIT);		//ﾃﾞﾊﾞｸﾞ情報を出力
}


//==============================================
//送信完了の同期
//==============================================
#define	outpt_sync()


//==============================================
//1文字を送信(ｿﾌﾄUART)
//==============================================
void outpt_c(					//戻り値無し
	unsigned char c)			//送信する文字
{
	unsigned char n=8;

	GPIO_L(DBG_GPIO,DBG_BIT);		//ｽﾀｰﾄﾋﾞｯﾄ
	wait_us(1000000/SHBPS);			//1ﾋﾞｯﾄ巾
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		if(c&1) GPIO_H(DBG_GPIO,DBG_BIT);
						//ﾃﾞｰﾀﾋﾞｯﾄ
		else GPIO_L(DBG_GPIO,DBG_BIT);
		wait_us((10000000/SHBPS+5)/10);	//1ﾋﾞｯﾄ巾
		c>>=1;				//次のﾃﾞｰﾀﾋﾞｯﾄへ進む
	}
	GPIO_H(DBG_GPIO,DBG_BIT);		//ｽﾄｯﾌﾟﾋﾞｯﾄ
	wait_us(1000000/SHBPS*3);		//3ﾋﾞｯﾄ巾
}


#else					//内蔵UARTﾓｼﾞｭｰﾙを使うとき
//==============================================
//UARTの初期設定
//==============================================
static void dbg_init(void)
{

	USART1->CTLR2=0x00002000;		//ｽﾄｯﾌﾟ2
	USART1->BRR=(SYS_CLK*10000/SHBPS+5)/10-1;
						//ﾎﾞｰﾚｰﾄ設定
	USART1->CTLR1|=0x0000200c;		//USART有効､送信有効､受信有効
#ifdef DBG_RM				//ﾃﾞﾊﾞｸﾞ情報出力をﾘﾏｯﾌﾟするとき
	AFIO->PCFR1|=DBG_RM;			//ﾘﾏｯﾌﾟする
#endif
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	GPIO_AF(DBG_GPIO,DBG_BIT);		//ﾃﾞﾊﾞｸﾞ情報出力を交代機能に設定
#endif
#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
	GPIO_AF(DBG_IN_GPIO,DBG_IN_BIT);	//ﾃﾞﾊﾞｸﾞ情報入力を交代機能に設定
#endif
}


//==============================================
//送信完了の同期
//==============================================
static void outpt_sync(void)
{

	while(!(USART1->STATR&USART_STATR_TC));	//送信完了を待つ
}


//==============================================
//1文字を送信
//==============================================
static void outpt_c(
	unsigned char c)			//送信する文字
{

		while(!(USART1->STATR&USART_STATR_TXE));
						//TDR空きを待つ
		USART1->DATAR=(unsigned long)c;	//ﾃﾞｰﾀを送信する
}
#endif


//==============================================
//文字列(文字列の最後は\0)を送信
//==============================================
static void outpt_a(
	unsigned char *a)			//送信する文字列
{

	while(*a) outpt_c(*a++);		//文字列を送信する
}


//==============================================
//charを16進2文字にして送信する
//==============================================
static void outpt_b(
	unsigned char b)			//送信するﾊﾞｲﾄﾃﾞｰﾀ
{
	unsigned char c;

	c=b>>4;					//上位ﾆﾌﾞﾙを取出す
	if(c>9) c+=55;				//16進文字に変換する
	else c+=48;
	outpt_c(c);				//1文字を送信する
	c=(unsigned char)(b&0x0f);		//下位ﾆﾌﾞﾙを取出す
	if(c>9) c+=55;				//16進文字に変換する
	else c+=48;
	outpt_c(c);				//1文字を送信する
}


//==============================================
//shortを16進4文字にして送信する
//==============================================
static void outpt_s(
	unsigned short s)			//送信するshortﾃﾞｰﾀ
{

	outpt_b(s>>8);				//上位ﾊﾞｲﾄを16進で送信する
	outpt_b(s);				//下位ﾊﾞｲﾄを16進で送信する
}


//==============================================
//longを16進8文字にして送信する
//==============================================
static void outpt_l(
	unsigned long l)			//送信するlongﾃﾞｰﾀ
{

	outpt_b(l>>24);				//最上位ﾊﾞｲﾄを16進で送信する
	outpt_b(l>>16);
	outpt_b(l>>8);
	outpt_b(l);				//最下位ﾊﾞｲﾄを16進で送信する
}


//==============================================
//複数ﾊﾞｲﾄを16進2文字にして送信する
//==============================================
static void outpt_d(
	unsigned char *d,			//送信するﾊﾞｲﾄﾃﾞｰﾀのｱﾄﾞﾚｽ
	unsigned char l)			//ﾊﾞｲﾄ数
{

	outpt_c('[');
	while(--l)
	{
		outpt_b(*d++);
		outpt_c('.');
	}
	outpt_b(*d++);
	outpt_c(']');
}


//==============================================
//ﾃﾞﾊﾞｸﾞ情報の16ﾋﾞｯﾄを10進5文字に変換して送信する
//==============================================
static void outpt_i(
	unsigned short i)			//送信する16ﾋﾞｯﾄﾃﾞｰﾀ
{
	unsigned char b[10];

	b[0]=i/10000;				//10000の桁を求める
	i-=b[0]*10000;
	b[1]=i/1000;				//1000の桁を求める
	i-=b[1]*1000;
	b[2]=i/100;				//100の桁を求める
	i-=b[2]*100;
	b[3]=i/10;				//10の桁を求める
	i-=b[3]*10;
	b[4]=i;					//1桁を求める
	outpt_c(b[0]+'0');			//10進文字に変換して送信する
	outpt_c(b[1]+'0');
	outpt_c(b[2]+'0');
	outpt_c(b[3]+'0');
	outpt_c(b[4]+'0');
}


//==============================================
//ﾃﾞﾊﾞｸﾞ情報出力のﾏｸﾛ定義
//==============================================
#define DBG_C(c) outpt_c((unsigned char)c)
#define DBG_A(a) outpt_a((unsigned char *)a)
#define DBG_B(b) outpt_b((unsigned char)b)
#define DBG_S(s) outpt_s((unsigned short)s)
#define DBG_L(l) outpt_l((unsigned int)l)
#define DBG_D(d,l) outpt_d((unsigned char *)d,l)
#define DBG_I(s) outpt_i((unsigned short)s)
#define DBG_SYNC() outpt_sync()
#else
#define DBG_C(c)
#define DBG_A(a)
#define DBG_B(b)
#define DBG_S(s)
#define DBG_L(l)
#define DBG_D(d,l)
#define DBG_I(s)
#define DBG_SYNC()
#endif
