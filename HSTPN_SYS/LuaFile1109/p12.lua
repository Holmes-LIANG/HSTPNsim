-- ��ʼ������1
PI = 3.1415926535898
--ST = 0.67 -- ��ͷ�����չ���1
SL = 1.37
mT = 180 -- ����������ͷ������
Ml = 5000 -- ���ص���ʼ����1
R0 = 6371008.77138
fM = 3.986004418e14
L = 9230100 -- �������������ڵ������������
--���㵯ͷ�����չ���
function STS(hx_enable)
	if hx_enable==1 then
		ST = 1.67
	else
		ST = 0.67
	end
	return ST
end		
-- ��������ܶ�rou
function rou(h)
	if h<=11.019e3 then
		w = 1-h/44.3308/1000
		rou1 = 1.225 * w^4.2559
	elseif h>11.0191e3 and h<=24.0631e3 then
		w = math.exp((14.9647-h/1000)/6.3416)
		rou1 = 1.225*0.15898*w
	elseif (h>24601.3) and (h<=32161.9) then
		w=1+(h/1000-24.9021)/221.552
		rou1=1.225*0.032722* w^(-35.1629)
	elseif (h>32161.9) and (h<=47350.1) then
		w=1+(h/1000-39.7499)/89.4107
		rou1=1.225*3.2618e-3* w^(-13.2011)
	elseif (h>47350.1) and (h<=51412.5) then
		w=math.exp((48.6252-h/1000)/7.9233)
		rou1=1.225*9.492e-4 *w
	elseif (h>51412.5) and (h<=71802) then
		w=1-(h/1000-59.439)/88.2218
		rou1=1.225*2.5280e-4 * w^11.2011
	elseif (h>71802) and (h<=86000) then
		w=1-(h/1000-78.0303)/100.295
		rou1=1.225*1.7632e-5 * w^16.0816
	elseif (h>86000) and (h<=91000) then
		w=math.exp((87.2848-h/1000)/5.47)
		rou1=1.225*3.6411e-6* w
	elseif (h>91000) and (h<=200000) then
		rou1=math.exp(-3.41173e-6* (h/1000)^3+0.0018157*(h/1000)^2-0.337816*h/1000+12.63404)
	else 
		rou1=math.exp(-9.595-9.7875e-3*(h/1000-200)+7.0725e-6*(h/1000-200)*(h/1000-400)) 
	end	
	return rou1
end

-- ���㹥��alpha
function alphaT(y, jd_enable ,hx_enable)
	if (jd_enable == 1) or (hx_enable == 1) then
		if (y>=50000) and (y<80000) then
			alpha1=0
		elseif (y>=30000) and (y<50000) then
			alpha1=15/180*PI;
		else
			alpha1=25/180*PI;
		end
	else
		if (y>=50000) and (y<80000) then
			alpha1=0
		elseif (y>=30000) and (y<50000) then
			alpha1=0/180*PI;
		else
			alpha1=0/180*PI;
		end
	end
	return alpha1
end

function alphaL(y)
		if (y>=50000) and (y<80000) then
			alpha1=0;
		elseif (y>=30000) and (y<50000) then
			alpha1=25/180*PI
		else
			alpha1=25/180*PI
		end
	return alpha1
end

-- ����g&#x0D;
function g(y)
	r = y + R0
	g1 = -fM / (r^2)
	return g1
end

-- ��������X
function XT1(v, y, jd_enable, hx_enable)
	q = 0.5 * rou(y) * v^2
	Cx0 = 0.04
	Cxa = 0.2
	Cx = Cx0 + Cxa * (alphaT(y, jd_enable, hx_enable))^2
	return q * STS(hx_enable) * Cx
end
function XL1(v,y)
	q = 0.5 * rou(y) * v^2
	Cx0 = 0.04
	Cxa = 0.2
	Cx = Cx0 + Cxa * (alphaL(y))^2
	return q *SL * Cx
end

-- ��������Y
function YT1(v,y, jd_enable, hx_enable)
	q = 0.5 * rou(y) * v^2
	if hx_enable == 1 then
		Cy = 0.15
	else
		Cy = 0.1
	end
	return q * STS(hx_enable) * Cy * alphaT(y, jd_enable ,hx_enable)
end
function YL1(v,y)
	q = 0.5 * rou(y) * v^2
	Cy = 0.1
	return q * SL * Cy * alphaL(y)
end

-- �������ص�m
function mL(t,step)
	Mr = 4546 -- �ƽ���������
	te = 65 -- �ƽ������ĵ�ʱ��&#x0D;
	vr = 4546 / 65.0 -- �ƽ������ĵ��ٶ�&#x0D;
	
	if 65-t<=step then
		m1 = Ml - vr * t
	else
		m1 = 350
	end
	
	return m1
end

-- �������ص�������P
function PL(t,step)
	if 65-t<=step then
		P1 = 4.824901861928814e5
	else
		P1 = 0
	end
	return P1
end
function p12(bcanEnable, step, flag, xL_ini, yL_ini, r_least, Prob, xT, yT, vT, thetaT, vL, thetaL, xL, yL, t, wkesai, wthetaT, wthetaL, walphaT, walphaL, deta, p1_initime, p27_initime, hx_enable, jd_enable)

 --TODO:Write your function here! 
	if flag == 0 then
		-- ��ʼ������������ر���ֵ
		vT = 6795--v_ini
		thetaT = -0.6629--theta_ini
		xT = 9055100--x_ini
		yT = 149027--y_ini
		-- ��ʼ�����ص���ر���ֵ
		--alphaL = 0
		vL = 0.0
		thetaL = PI / 2
		--math.randomseed(tostring(os.time()):reverse():sub(1, 7))
		xL = 0.0--math.random(-15000,15000) 
		yL = 0.0
		-- t
		t = p1_initime + p27_initime
		-- ΢�ֶԲ߳�ʼ������
		wkesai = 0.51
		wthetaT = PI - thetaT
		wthetaL = PI/2
		walphaT = wthetaT - wkesai
		walphaL = wthetaL - wkesai
		
		xL_ini = xL
		yL_ini = yL
		r_least = 10000000
		deta = 3000
		
		flag = 1
	end
	
	--table_x = {} -- �ֱ����ڴ洢x��y��rd��ֵ
	--table_y = {}
	--table_r = {}
	--count = 1 -- ���ڼ���һ�������˶��ٸ�����
	
	--while yT > 0 do
	
		-- ��������
		dvT = -XT1(vT,yT, jd_enable, hx_enable)/mT + g(yT) * math.sin(thetaT)
		dthetaT = YT1(vT,yT,jd_enable, hx_enable)/(mT*vT) - (vT/(R0+yT) - g(yT)/vT) * math.cos(thetaT)
		vT = vT + dvT * step
		thetaT = thetaT + dthetaT * step
		xT = xT + vT * math.cos(thetaT) * step
		yT = yT + vT * math.sin(thetaT) * step 
		
		-- ���ص�
		dvL = (PL(t,step)*math.cos(alphaL(yL)) - XL1(vL,yL) - mL(t,step)*g(yL)*math.sin(thetaL)) / mL(t,step)
		vL = vL + dvL * step
		if wthetaL >= 0 then
			thetaL = wthetaL
		else
			thetaL = 0
		end
		xL = xL + vL * math.cos(thetaL) * step
		yL = yL + vL * math.sin(thetaL) * step
		
		-- ΢�ֶԲ�&#x0D;
		r = math.sqrt(((L-xT) - xL)^2 + (yT - yL)^2) -- ��Ŀ����
		--wthetaT = PI - thetaT
		--walphaT = wthetaT - wkesai
		dr = vT*math.cos(walphaT) - vL*math.cos(walphaL)
		--r = r + dr * step
		dwkesai = (vT*math.sin(walphaT) - vL*math.sin(walphaL)) / r
		--wkesai = wkesai + dwkesai * step
		
		wthetaT = PI - thetaT
		walphaT = wthetaT - wkesai
		r = r + dr * step
		wkesai = wkesai + dwkesai * step
		
		K = 1.35 -- �Ƶ���
		wthetaL = wthetaL + K * dwkesai * step
		walphaL = wthetaL - wkesai
		
	--	table_x[count] = xL
	--	table_y[count] = yL
	--	table_r[count] = r
	--	count = count + 1
		
		t = t + step
	
--	end
	--------------------------------------------------------�������ظ���-------------
	-- �ҵ�table_r ����С��������Ϊ�Ѱ���
	--r_least = --table_r[1]
	
	
	--ps_count = 1
	--for i=1,#table_r do
	
	--	a = table_r[i]
	--	if a < r_least then
	--		r_least = a
	--		ps_count = i
	--	end
	
	--end
	
	-- ����xL�ķ���&#x0D;
	--sum = 0
	--for i=1,ps_count do
	--	sum = sum + table_x[i]
	--end
	--aver_x = sum / (ps_count)
	--sum = 0
	--for i=1,ps_count do
	--	sum = sum + (table_x[i]-aver_x)^2
	--end
	--var_x = sum / (ps_count)
	--stand_x = math.sqrt(var_x)
	
	-- ����yL�ķ���&#x0D;
	--sum = 0
	--for i=1,ps_count do
	--	sum = sum + table_y[i]
	--end
	--aver_y = sum / (ps_count)
	--sum = 0
	--for i=1,ps_count do
	--	sum = sum + (table_y[i]-aver_y)^2
	--end
	--var_y = sum / (ps_count)
	--stand_y = math.sqrt(var_y)
	
	-- ���㵯����׼��&#x0D;
	--stand_xy = (stand_x + stand_y) / 2.0
	
	-- �������ظ���&#x0D;
	--Prob = 1 - math.exp(-(r_least^2)/(2*stand_xy^2))
	
	----==============================================
	--r_least = r_least / 5
	--Prob = 1 - math.exp(-(2*stand_xy^2)/(r_least^2))
	----==============================================
	
	--------------------ʩ��Ӱ�����غ�����ظ���---------------------------------------
	--zhongy = 0.75 -- �Ե�Ŀ����r��Ӱ������
	--duodt = 0.55
	--jidong = 0.85
	
	--if Strgy_ZhYouE==1 and Strgy_DDT==0 and Strgy_JiDong==0 then
	--	index = zhongy
	--	Prob = Prob * index
	--elseif Strgy_ZhYouE==0 and Strgy_DDT==1 and Strgy_JiDong==0 then
	--	index = duodt
	--	Prob = Prob * index
	--elseif Strgy_ZhYouE==0 and Strgy_DDT==0 and Strgy_JiDong==1 then
	--	index = jidong
	--	Prob = Prob * index
	--elseif Strgy_ZhYouE==1 and Strgy_DDT==0 and Strgy_JiDong==1 then
	--	index = zhongy * jidong
	--	Prob = Prob * index
	--elseif Strgy_ZhYouE==0 and Strgy_DDT==1 and Strgy_JiDong==1 then
	--	index = duodt * jidong
	--	Prob = Prob * index
	--else -- û���κδ�ʩ
	--	index = 1
	--	Prob = Prob * index
	--end
	
	--if Strgy_YiLanYi == 0 and Strgy_ErLanYi == 1 then
	--	Prob = 1-(1-Prob)^2
	--end
	if r <= r_least and flag == 1 then 
		r_least = r
	else
		flag = 2 
	end
	
--[[	if r >= r_least and then
		r_least = r
	end
	]]
	if flag == 2 then
	-- �������
		Prob = 1 - math.exp (- deta ^2 / (2 *r_least^2))
	end
	
	if (yT-0) <= step then
		bcanEnable = true
		flag = 0
	else
		bcanEnable = false
	end
		

 return bcanEnable, step, flag, xL_ini, yL_ini, r_least, Prob, xT, yT, vT, thetaT, vL, thetaL, xL, yL, t, wkesai, wthetaT, wthetaL, walphaT, walphaL, deta
end