function  p81(bcanEnable, step, n, tag1,health_yc,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj,plane_dzj,missile_bjx,missile_dlbl,missile_yc,health_radar_bjx,health_radar_dlbl,health_radar_yc)
   
   --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
   --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
   --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
   --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...
   --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；
   --（2）有向弧上的数字必须大于0；
   --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

   --TODO:Write your function here! 
 
  --[[ 初始值
   plane_dzj=18
   plane_gjj=12
   palane_hzj=30
   
   missile_bjx=10
   missile_dlbl=10
   missile_ycdd=10
   
   health_bjx=100
   halth_dlbl=100
  ]] 
   n=1
   
   plane_gjj=plane_gjj-(loss_bjx_gjj+loss_dlbl_gjj)
   plane_hzj=plane_hzj-(loss_bjx_hzj+loss_dlbl_hzj)  
   
   if (health_bjx<=0 and health_dlbl<=0) 
   then
	tag1=1 --美军胜利
   elseif plane_hzj<=12
   then
	tag1=3 --利军胜利
   else
	tag1=2 --继续战斗
   end
   
   ---flag_victory
   
   --tag1=2
   
   bcanEnable=true
   
   return bcanEnable, step, n, tag1,health_yc,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj,plane_dzj,missile_bjx,missile_dlbl,missile_yc,health_radar_bjx,health_radar_dlbl,health_radar_yc
end
