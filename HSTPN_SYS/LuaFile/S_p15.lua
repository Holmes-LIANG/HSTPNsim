function S_p15(bcanEnable, random_num,flag_dlbl)
 
 --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
 --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
 --Note:第二个参数为random_num，用户可自行设定的概率阈值
 --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

 --TODO:Write your function here! 

 if flag_dlbl==1 
 then
		random_num=1
		flag_dlbl=0
 else
		random_num=0
 end
 
 bcanEnable=true
 
 return bcanEnable, random_num,flag_dlbl
end
