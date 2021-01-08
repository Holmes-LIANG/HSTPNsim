function  p2(bcanEnable, step,n, tag1, tag2,plane_dzj,plane_gjj,plane_hzj,health_bjx,health_dlbl,plane_dzj_bjx,plane_dzj_dlbl,plane_gjj_bjx,plane_gjj_dlbl,plane_hzj_bjx,plane_hzj_dlbl)
   
   --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
   --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
   --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
   --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...
   --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；
   --（2）有向弧上的数字必须大于0；
   --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

   --TODO:Write your function here! 
 
    n=2
 
 	tag1=1
    tag2=2  
	
	--电子战飞机资源分配
	if plane_dzj>=12
	then
		plane_dzj_bjx=6
		plane_dzj_dlbl=6
	elseif plane_dzj<12 and plane_dzj>=6
	then
      if (health_bjx>health_dlbl) 
	  then
		plane_dzj_bjx=6
		plane_dzj_dlbl=0
	  else
	    plane_dzj_bjx=0
	    plane_dzj_dlbl=6
	  end
	else
		plane_dzj_bjx=0
		plane_dzj_dlbl=0
	end
	
	--攻击机资源分配
	if plane_gjj>=16
	then
		plane_gjj_bjx=8
		plane_gjj_dlbl=8
	elseif plane_gjj<16 and plane_gjj>=8
	then
      if (health_bjx>health_dlbl) 
	  then
		plane_gjj_bjx=8
		plane_gjj_dlbl=0
	  else
	    plane_gjj_bjx=0
	    plane_gjj_dlbl=8
	  end
	else
		plane_gjj_bjx=0
		plane_gjj_dlbl=0
	end
	
	--轰炸机资源分配
	if plane_hzj>=24
	then
		plane_hzj_bjx=12
		plane_hzj_dlbl=12
	elseif plane_hzj<24 and plane_hzj>=12
	then
      if (health_bjx>health_dlbl) 
	  then
		plane_hzj_bjx=12
		plane_hzj_dlbl=0
	  else
	    plane_hzj_bjx=0
	    plane_hzj_dlbl=12
	  end
	else
		plane_hzj_bjx=0
		plane_hzj_dlbl=0
	end
	
	bcanEnable=true
   
   return bcanEnable, step,n,tag1, tag2,plane_dzj,plane_gjj,plane_hzj,health_bjx,health_dlbl,plane_dzj_bjx,plane_dzj_dlbl,plane_gjj_bjx,plane_gjj_dlbl,plane_hzj_bjx,plane_hzj_dlbl
end
