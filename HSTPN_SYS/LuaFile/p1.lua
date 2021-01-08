function p1(bcanEnable, step, t, x)
 
 --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
 --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
 --Note:第二个和第三个参数分别为仿真步长和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
 --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

 --TODO:Write your function here! 

 
 bcanEnable=true
 return bcanEnable, step, t, x
end
