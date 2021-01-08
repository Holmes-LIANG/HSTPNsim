function p79(bcanEnable, step, t,missile_bjx,launcher_bjx,plane_gjj_bjx,plane_hzj_bjx,prob_bjx,tactic_bjx,loss_bjx_gjj,loss_bjx_hzj,health_bjx, health_radar_bjx)
 
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
 loss_bjx_gjj=0
 loss_bjx_hzj=0
 
 --------班加西交战过程----------------
 if (plane_gjj_bjx+plane_hzj_bjx)>0
 then	
 --采用一拦一策略&#x0A;	 
	 if tactic_bjx==1
	 then
		if missile_bjx>=launcher_bjx*3 --导弹发射为三联发射
		then
			if launcher_bjx*3>=(plane_gjj_bjx+plane_hzj_bjx)
			then
				loss_bjx_gjj=plane_gjj_bjx*prob_bjx
				health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
				prob_bjx=health_radar_bjx/100*prob_bjx
				loss_bjx_hzj=plane_hzj_bjx*prob_bjx
				--计算剩余导弹数量
				missile_bjx=missile_bjx-plane_gjj_bjx-plane_hzj_bjx
			else
				if launcher_bjx*3<=plane_hzj_bjx
				then	
					loss_bjx_gjj=0
					loss_bjx_hzj=launcher_bjx*3*prob_bjx
					health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
					--计算剩余导弹数量
					missile_bjx=missile_bjx-launcher_bjx*3
				else
					--防止无攻击机时计算错误
					if plane_gjj_bjx>0
					then
						loss_bjx_gjj=(launcher_bjx*3-plane_hzj_bjx)*prob_bjx
						health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
						prob_bjx=health_radar_bjx/100*prob_bjx
					else
						loss_bjx_gjj=0
					end
					loss_bjx_hzj=plane_hzj_bjx*prob_bjx
					--计算剩余导弹数量
					missile_bjx=missile_bjx-launcher_bjx*3
				end
			end
		else
			if missile_bjx>=(plane_gjj_bjx+plane_hzj_bjx)
			then
				loss_bjx_gjj=plane_gjj_bjx*prob_bjx
				health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
				prob_bjx=health_radar_bjx/100*prob_bjx
				loss_bjx_hzj=plane_hzj_bjx*prob_bjx
				--计算剩余导弹数量
				missile_bjx=missile_bjx-plane_gjj_bjx-plane_hzj_bjx
			else
				if missile_bjx<=plane_hzj_bjx
				then	
					loss_bjx_gjj=0
					loss_bjx_hzj=missile_bjx*prob_bjx
					health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
					--计算剩余导弹数量
					missile_bjx=0
				else	
				--防止无攻击机时计算错误
				 if plane_gjj_bjx>0
					then
						loss_bjx_gjj=(missile_bjx-plane_hzj_bjx)*prob_bjx
						health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
						prob_bjx=health_radar_bjx/100*prob_bjx
					else
						loss_bjx_gjj=0
					end
					loss_bjx_hzj=plane_hzj_bjx*prob_bjx
					--计算剩余导弹数量
					missile_bjx=0
				end
			end			
		end
	 --采用二拦一策略&#x0A;	 
	 elseif tactic_bjx==2
	 then
		loss_bjx_gjj=plane_gjj_bjx*prob_bjx
		health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
		prob_bjx=health_radar_bjx/100*prob_bjx
		loss_bjx_hzj=plane_hzj_bjx*prob_bjx
		--计算剩余导弹数量
		missile_bjx=missile_bjx-(plane_hzj_bjx+plane_gjj_bjx)*2
	 end
 end
 
 --对战机数量向下取整
 loss_bjx_gjj=loss_bjx_gjj-loss_bjx_gjj%1
 loss_bjx_hzj=loss_bjx_hzj-loss_bjx_hzj%1
 
 --计算班加西生命值
 --一架轰炸机对基地造成8点伤害
 health_bjx=math.max(health_bjx-(plane_hzj_bjx-loss_bjx_hzj)*8,0)
 --health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
 
 bcanEnable=true
 
 return bcanEnable, step, t,missile_bjx,launcher_bjx,plane_gjj_bjx,plane_hzj_bjx,prob_bjx,tactic_bjx,loss_bjx_gjj,loss_bjx_hzj,health_bjx, health_radar_bjx
end
