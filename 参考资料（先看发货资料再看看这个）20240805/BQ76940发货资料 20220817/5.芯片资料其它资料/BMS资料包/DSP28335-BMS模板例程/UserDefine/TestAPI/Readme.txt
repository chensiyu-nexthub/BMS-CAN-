Step：
	1、把Flash28335_API_V210.lib这个库文件加载到工程中，
	在File Search Path中加入	"${PROJECT_ROOT}/TestAPI/Flash28335_API_V210.lib"

	2、包含头文件API.h
	3、修改CMD如下：
		
		   Flash28_API:
		   {
		        -lFlash28335_API_V210.lib(.econst)
		        -lFlash28335_API_V210.lib(.text)
		   }                   LOAD = FLASHD,
		                       RUN = RAML0,
		                       LOAD_START(_Flash28_API_LoadStart),
		                       LOAD_END(_Flash28_API_LoadEnd),
		                       RUN_START(_Flash28_API_RunStart),
		                       PAGE = 0
		                       
	4、初始化API_Init();
	5、每个函数如果错误的话，则会返回0，否则返回1;
	6、如果一个地址里边已经有数据，则是不能重新写数据的，擦除的话要整个SECTOR一起擦除，默认使用的是FLASHH这个段;
	7、根据实际数组长度修改BufLength，也可以自己定义数组；
	8、消除警告#16002-D在消除警告的命令框输入 --diag_suppress=16002
	
 