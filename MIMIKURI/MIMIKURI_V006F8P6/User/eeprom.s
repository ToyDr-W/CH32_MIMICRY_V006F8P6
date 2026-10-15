	.section  .text
	.align  10		//1kﾊﾞｲﾄ境界
	.globl  eepdata
eepdata:
	.rept	1024*57		//ｾｸﾀｻｲｽﾞ*ｾｸﾀ数
	.byte	0xff
	.endr
