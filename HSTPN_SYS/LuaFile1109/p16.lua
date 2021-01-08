function  p16(bcanEnable, step, n,tag1, tag2, tag3,flag_bjx,flag_yc,flag_dzj_bjx,plane_dzj_bjx,plane_gjj_bjx,plane_hzj_bjx)
   
   --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
   --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
   --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
   --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...
   --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；
   --（2）有向弧上的数字必须大于0；
   --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

   --TODO:Write your function here! 

   n=3
   
   tag1=1
   tag2=2
   tag3=3   
   
   flag_bjx=1
   flag_yc=1
   
   --电子战飞机只出动一次
   if plane_dzj_bjx>0
   then
	   if(flag_dzj_bjx==0)
	   then
		flag_dzj_bjx=1
	   else
		plane_dzj_bjx=0
	   end
	end
	
	--若没有轰炸机则不进行攻击
	if plane_hzj_bjx==0
	then
		plane_dzj_bjx=0
		plane_gjj_bjx=0
		plane_hzj_bjx=0	
    end
	
    bcanEnable=true
   
   return bcanEnable, step,n, tag1, tag2, tag3,flag_bjx,flag_yc,flag_dzj_bjx,plane_dzj_bjx,plane_gjj_bjx,plane_hzj_bjx
end
