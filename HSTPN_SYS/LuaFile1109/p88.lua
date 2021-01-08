function p88(bcanEnable, step, t,missile_yc,launcher_yc,plane_dzj_bjx,plane_dzj_dlbl,prob_yc,tactic_yc,loss_dlbl_dzj,loss_bjx_dzj,interference_bjx,interference_dlbl,prob_bjx,prob_dlbl,plane_dzj,plane_gjj_bjx,plane_hzj_bjx,plane_gjj_dlbl,plane_hzj_dlbl,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj)
 
 --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误
 --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件
 --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作
 --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...
 --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；
 --（2）有向弧上的数字必须大于0；
 --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明

 --TODO:Write your function here! 

 ---------远程导弹交战过程-----------
 --[[
 if ((plane_dzj_dlbl>0 and plane_dzj_dlbl>0) and (health_dlbl<=health_dlbl)) or (plane_dzj_dlbl>0 and plane_dzj_dlbl=0)
 then
	if health_dlbl>=health_dlbl
]]		

--每次先把损失数置0
loss_dlbl_dzj=0
loss_bjx_dzj=0
--每次先把影响值置0
interference_bjx=0
interference_dlbl=0
--电子干扰飞机只进行一次干扰&#x0D;
 if (plane_dzj_bjx+plane_dzj_dlbl)>0
 then	
 --采用一拦一策略&#x0A;	 
	 if tactic_yc==1
		 then
			if missile_yc>=launcher_yc*3 --导弹发射为三联发射
			then
				if launcher_yc*3>=(plane_dzj_bjx+plane_dzj_dlbl)
				then
					loss_bjx_dzj=plane_dzj_bjx*prob_yc
					loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
					--计算剩余导弹数量
					missile_yc=missile_yc-plane_dzj_bjx-plane_dzj_dlbl
				else
				--优先保卫的黎波里&#x0D;
					if launcher_yc*3<=plane_dzj_dlbl
					then	
						loss_bjx_dzj=0
						loss_dlbl_dzj=launcher_yc*3*prob_yc
						--计算剩余导弹数量
						missile_yc=missile_yc-launcher_yc*3
					else
					--防止无攻击机时计算错误
						if plane_dzj_bjx>0
						then
							loss_bjx_dzj=(launcher_yc*3-plane_dzj_dlbl)*prob_yc
						else
							loss_bjx_dzj=0
						end
						loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
						--计算剩余导弹数量
						missile_yc=missile_yc-launcher_yc*3
					end
				end
			else
				if missile_yc>=(plane_dzj_bjx+plane_dzj_dlbl)
				then
					loss_bjx_dzj=plane_dzj_bjx*prob_yc
					loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
					--计算剩余导弹数量
					missile_yc=missile_yc-plane_dzj_bjx-plane_dzj_dlbl
				else
					if missile_yc<=plane_dzj_dlbl
					then	
						loss_bjx_dzj=0
						loss_dlbl_dzj=missile_yc*prob_yc
						--计算剩余导弹数量
						missile_yc=0
					else	
						--防止无攻击机时计算错误
					 if plane_dzj_bjx>0
						then
							loss_bjx_dzj=(missile_yc-plane_dzj_dlbl)*prob_yc
						else
							loss_bjx_dzj=0
						end
						loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
						--计算剩余导弹数量
						missile_yc=0
					end
				end			
			end
	 --采用二拦一策略&#x0A;	 
	 elseif tactic_yc==2
	 then
		loss_bjx_dzj=plane_dzj_bjx*prob_yc
		loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
		--计算剩余导弹数量
		missile_yc=missile_yc-(plane_dzj_bjx+plane_dzj_dlbl)*2
	 end
	 
	 --对损失的电子战飞机数量向下取整
	 loss_bjx_dzj=loss_bjx_dzj-loss_bjx_dzj%1
	 loss_dlbl_dzj=loss_dlbl_dzj-loss_dlbl_dzj%1
	 
	 --计算剩余的电子战飞机数量
	 plane_dzj=plane_dzj-(loss_bjx_dzj+loss_dlbl_dzj)
	 
	 --计算电子干扰对班加西和的黎波里的影响
	 --interference_bjx=math.min(0.4,(plane_dzj_bjx-loss_bjx_dzj)*0.1)
	 --interference_dlbl=math.min(0.4,(plane_dzj_dlbl-loss_dlbl_dzj)*0.1)

 end
 --计算电子干扰对班加西和的黎波里的影响
 interference_bjx=math.min(0.4,(plane_dzj_bjx-loss_bjx_dzj)*0.1)
 interference_dlbl=math.min(0.4,(plane_dzj_dlbl-loss_dlbl_dzj)*0.1)
 prob_bjx=prob_bjx*(1-interference_bjx)
 prob_dlbl=prob_dlbl*(1-interference_dlbl)
 
 
--20200108
--对班加西/的黎波里进行防御

loss_bjx_gjj=0
loss_bjx_hzj=0
loss_dlbl_gjj=0
loss_dlbl_hzj=0
if (plane_gjj_bjx+plane_hzj_bjx)>0
then

	if (plane_gjj_bjx+plane_hzj_bjx)*2<launcher_yc*3 and missile_yc>(plane_gjj_bjx+plane_hzj_bjx)*2
	then
		prob_yc=0.5
		tactic_yc=2
	else
		prob_yc=0.3
		tactic_yc=1
	end
	
 --------班加西交战过程----------------

 --采用一拦一策略&#x0A;	 
	 if tactic_yc==1
		 then
			if missile_yc>=launcher_yc*3 --导弹发射为三联发射
			then
				if launcher_yc*3>=(plane_gjj_bjx+plane_hzj_bjx)
				then
					loss_bjx_gjj=plane_gjj_bjx*prob_yc
					loss_bjx_hzj=plane_hzj_bjx*prob_yc
					--计算剩余导弹数量
					missile_yc=missile_yc-plane_gjj_bjx-plane_hzj_bjx
				else
					if launcher_yc*3<=plane_hzj_bjx
					then	
						loss_bjx_gjj=0
						loss_bjx_hzj=launcher_yc*3*prob_yc
						--计算剩余导弹数量
						missile_yc=missile_yc-launcher_yc*3
					else
						--防止无攻击机时计算错误
						if plane_gjj_bjx>0
						then
							loss_bjx_gjj=(launcher_yc*3-plane_hzj_bjx)*prob_yc
						else
							loss_bjx_gjj=0
						end
						loss_bjx_hzj=plane_hzj_bjx*prob_yc
						--计算剩余导弹数量
						missile_yc=missile_yc-launcher_yc*3
					end
				end
			else
				if missile_yc>=(plane_gjj_bjx+plane_hzj_bjx)
				then
					loss_bjx_gjj=plane_gjj_bjx*prob_yc
					loss_bjx_hzj=plane_hzj_bjx*prob_yc
					--计算剩余导弹数量
					missile_yc=missile_yc-plane_gjj_bjx-plane_hzj_bjx
				else
					if missile_yc<=plane_hzj_bjx
					then	
						loss_bjx_gjj=0
						loss_bjx_hzj=missile_yc*prob_yc
						--计算剩余导弹数量
						missile_yc=0
					else	
						--防止无攻击机时计算错误
					 if plane_gjj_bjx>0
						then
							loss_bjx_gjj=(missile_yc-plane_hzj_bjx)*prob_yc
						else
							loss_bjx_gjj=0
						end
						loss_bjx_hzj=plane_hzj_bjx*prob_yc
						--计算剩余导弹数量
						missile_yc=0
					end
				end			
			end
	 --采用二拦一策略&#x0A;	 
	 elseif tactic_yc==2
	 then
		loss_bjx_gjj=plane_gjj_bjx*prob_yc
		loss_bjx_hzj=plane_hzj_bjx*prob_yc
		--计算剩余导弹数量
		missile_yc=missile_yc-(plane_hzj_bjx+plane_gjj_bjx)*2
	 end
	
	 --对战机数量向下取整
	 loss_bjx_gjj=loss_bjx_gjj-loss_bjx_gjj%1
	 loss_bjx_hzj=loss_bjx_hzj-loss_bjx_hzj%1
	 
	 --计算进攻班加西方向剩余战机
	 plane_gjj_bjx=math.max(plane_gjj_bjx-loss_bjx_gjj,0) 
	 plane_hzj_bjx=math.max(plane_hzj_bjx-loss_bjx_hzj,0)
	 
	 --计算剩余战机数量
	 plane_gjj=math.max(plane_gjj-loss_bjx_gjj,0) 
	 plane_hzj=math.max(plane_hzj-loss_bjx_hzj,0) 
	 
elseif (plane_gjj_dlbl+plane_hzj_dlbl)>0
then

	if (plane_gjj_dlbl+plane_hzj_dlbl)*2<launcher_yc*3 and missile_yc>(plane_gjj_dlbl+plane_hzj_dlbl)*2
	then
		prob_yc=0.5
		tactic_yc=2
	else
		prob_yc=0.3
		tactic_yc=1
	end

 --------的黎波里交战过程----------------

 --采用一拦一策略&#x0A;	 
	 if tactic_yc==1
	 then
		if missile_yc>=launcher_yc*3 --导弹发射为三联发射
		then
			if launcher_yc*3>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_yc
				loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
				--计算剩余导弹数量
				missile_yc=missile_yc-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if launcher_yc*3<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=launcher_yc*3*prob_yc
					--计算剩余导弹数量
					missile_yc=missile_yc-launcher_yc*3
				else
					--防止无攻击机时计算错误
					if plane_gjj_dlbl>0
					then
						loss_dlbl_gjj=(launcher_yc*3-plane_hzj_dlbl)*prob_yc
					else
						loss_dlbl_gjj=0
					end
					loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
					--计算剩余导弹数量
					missile_yc=missile_yc-launcher_yc*3
				end
			end
		else
			if missile_yc>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_yc
				loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
				--计算剩余导弹数量
				missile_yc=missile_yc-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if missile_yc<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=missile_yc*prob_yc
					--计算剩余导弹数量
					missile_yc=0
				else	
					--防止无攻击机时计算错误
				 if plane_gjj_dlbl>0
					then
						loss_dlbl_gjj=(missile_yc-plane_hzj_dlbl)*prob_yc
					else
						loss_dlbl_gjj=0
					end
					loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
					--计算剩余导弹数量
					missile_yc=0
				end
			end			
		end
	 --采用二拦一策略&#x0A;	 
	 elseif tactic_yc==2
	 then
		loss_dlbl_gjj=plane_gjj_dlbl*prob_yc
		loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
		--计算剩余导弹数量
		missile_yc=missile_yc-(plane_hzj_dlbl+plane_gjj_dlbl)*2
	 end
	
	--对战机数量向下取整
	 loss_dlbl_gjj=loss_dlbl_gjj-loss_dlbl_gjj%1
	 loss_dlbl_hzj=loss_dlbl_hzj-loss_dlbl_hzj%1
	 
	 --计算进攻班加西方向剩余战机
	 plane_gjj_dlbl=math.max(plane_gjj_dlbl-loss_dlbl_gjj,0) 
	 plane_hzj_dlbl=math.max(plane_hzj_dlbl-loss_dlbl_hzj,0)	 
	 
	 --计算剩余战机数量
	 plane_gjj=math.max(plane_gjj-loss_dlbl_gjj,0) 
	 plane_hzj=math.max(plane_hzj-loss_dlbl_hzj,0) 
end		
 
 bcanEnable=true
 
 return bcanEnable, step, t,missile_yc,launcher_yc,plane_dzj_bjx,plane_dzj_dlbl,prob_yc,tactic_yc,loss_dlbl_dzj,loss_bjx_dzj,interference_bjx,interference_dlbl,prob_bjx,prob_dlbl,plane_dzj,plane_gjj_bjx,plane_hzj_bjx,plane_gjj_dlbl,plane_hzj_dlbl,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj
end