   
   .area memory(abs)
   .org 0x10c0
   _test_50:: .blkw 1
     
	 clr  r24
	 sts  _test_50,R24