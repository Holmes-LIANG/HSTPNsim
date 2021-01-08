function p80(bcanEnable, step, t,missile_dlbl,launcher_dlbl,plane_gjj_dlbl,plane_hzj_dlbl,prob_dlbl,tactic_dlbl,loss_dlbl_gjj,loss_dlbl_hzj,health_dlbl, health_radar_dlbl)
 
 --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
 --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
 --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
 --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...
 --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；
 --（2）有向弧上的数字必须大于0；
 --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

 --TODO:Write your function here! 

 t=0
 
 --每次先把损失数置0
 loss_dlbl_gjj=0
 loss_dlbl_hzj=0
 
--------的黎波里交战过程----------------
 if (plane_gjj_dlbl+plane_hzj_dlbl)>0
 then	
 --采用一拦一策略&#x0A;	 
 if tactic_dlbl==1
	 then
		if missile_dlbl>=launcher_dlbl*3 --导弹发射为三联发射
		then
			if launcher_dlbl*3>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_dlbl
				health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
				prob_dlbl=health_radar_dlbl/100*prob_dlbl
				loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
				--计算剩余导弹数量
				missile_dlbl=missile_dlbl-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if launcher_dlbl*3<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=launcher_dlbl*3*prob_dlbl
					health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
					--计算剩余导弹数量
					missile_dlbl=missile_dlbl-launcher_dlbl*3
				else
					if plane_gjj_dlbl>0
					then
						loss_dlbl_gjj=(launcher_dlbl*3-plane_hzj_dlbl)*prob_dlbl
						health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
						prob_dlbl=health_radar_dlbl/100*prob_dlbl
					else
						loss_dlbl_gjj=0
					end
					loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
					--计算剩余导弹数量
					missile_dlbl=missile_dlbl-launcher_dlbl*3
				end
			end
		else
			if missile_dlbl>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_dlbl
				health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
				prob_dlbl=health_radar_dlbl/100*prob_dlbl
				loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
				--计算剩余导弹数量
				missile_dlbl=missile_dlbl-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if missile_dlbl<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=missile_dlbl*prob_dlbl
					health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
					--计算剩余导弹数量
					missile_dlbl=0
				else
					if plane_gjj_dlbl>0
					then
						loss_dlbl_gjj=(missile_dlbl-plane_hzj_dlbl)*prob_dlbl
						health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
						prob_dlbl=health_radar_dlbl/100*prob_dlbl
					else
						loss_dlbl_gjj=0
					end
					loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
					--计算剩余导弹数量
					missile_dlbl=0
				end
			end			
		end
 --采用二拦一策略&#x0A;	 
 elseif tactic_dlbl==2
	 then
		loss_dlbl_gjj=plane_gjj_dlbl*prob_dlbl
		health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
		prob_dlbl=health_radar_dlbl/100*prob_dlbl
		loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
		--计算剩余导弹数量
		missile_dlbl=missile_dlbl-(plane_hzj_dlbl+plane_gjj_dlbl)*2
	 end
 end
 
 --对战机数量向下取整
 loss_dlbl_gjj=loss_dlbl_gjj-loss_dlbl_gjj%1
 loss_dlbl_hzj=loss_dlbl_hzj-loss_dlbl_hzj%1
 
 --计算的黎波里生命值
 --一架轰炸机对基地造成8点伤害
 health_dlbl=math.max((health_dlbl-(plane_hzj_dlbl-loss_dlbl_hzj)*8),0) 
 --health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
 
 bcanEnable=true
 
 return bcanEnable, step, t,missile_dlbl,launcher_dlbl,plane_gjj_dlbl,plane_hzj_dlbl,prob_dlbl,tactic_dlbl,loss_dlbl_gjj,loss_dlbl_hzj,health_dlbl, health_radar_dlbl
end