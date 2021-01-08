PI=3.1415926
K = 1.380649*10^-23--玻尔兹曼常数
Pt = 1000000 -- 发射功率
G = 50 -- 天线增益
lamda = 0.03 -- 雷达波长
sigma = 50 --目标反射面积
tau = 1 --脉冲宽度&#x0D;
T = 290 -- 大气热力学温度&#x0D;
Br = 1000000 -- 带宽&#x0D;
L = 2.5 -- 损耗因子&#x0D;
F = 12 -- 接收机噪声系数
n = 4-- 信号脉冲数
h =4.75*math.sqrt(n)+n -- 雷达监测门限值
PJ = 500 --干扰机的发射功率
GJ= 7--干扰机对雷达方向之间的增益
GI= 50 --雷达在干扰机方向之间的增益
BJ=4000000--干扰机干扰信号带宽&#x0D;
L1 = 4 --干扰信号的传输损耗&#x0D;
RJ=300000 --雷达与干扰机之间的距离

function R(t)
	juli= 10000
	return juli 
end		

function SAN(t)
	san= (Pt*G^2*lamda^2*sigma)/((4*PI)^3*K*T*Br*L*F*R(t)^4)
	return san 
end	

function SANr(t)
	san= (Pt*G^2*RJ^2*sigma*L1*BJ)/(4*PI*PJ*R(t)^4*GI*GJ*L*Br)
	return san 	
end

function Rmax(t)
	rmax= ((Pt*n*tau*G^2*lamda^2*sigma)/((4*PI)^3*K*T*L*F*SANr(t)))^(1/4)
	return rmax 
end	
	
function Prd(t)
	
		pr= math.exp(-4.75/(math.sqrt(n)*SAN(t)))
 prd=1-(1-pr)
	
	return prd
end

function Prdr(t)
	
		pr= math.exp(-4.75/(math.sqrt(n)*SANr(t)))
 prd=1-(1-pr)
	
	return prd
end

function p4(bcanEnable, step, t)
 
 --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
 --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
 --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
 --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...
 --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；
 --（2）有向弧上的数字必须大于0；
 --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

 --TODO:Write your function here! 

 t=0
 bcanEnable=true
 return bcanEnable, step, t
end
