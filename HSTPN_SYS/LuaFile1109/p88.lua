function p88(bcanEnable, step, t,missile_yc,launcher_yc,plane_dzj_bjx,plane_dzj_dlbl,prob_yc,tactic_yc,loss_dlbl_dzj,loss_bjx_dzj,interference_bjx,interference_dlbl,prob_bjx,prob_dlbl,plane_dzj,plane_gjj_bjx,plane_hzj_bjx,plane_gjj_dlbl,plane_hzj_dlbl,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj)
 
 --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
 --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
 --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
 --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
 --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
 --��2�������ϵ����ֱ������0��
 --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

 --TODO:Write your function here! 

 ---------Զ�̵�����ս����-----------
 --[[
 if ((plane_dzj_dlbl>0 and plane_dzj_dlbl>0) and (health_dlbl<=health_dlbl)) or (plane_dzj_dlbl>0 and plane_dzj_dlbl=0)
 then
	if health_dlbl>=health_dlbl
]]		

--ÿ���Ȱ���ʧ����0
loss_dlbl_dzj=0
loss_bjx_dzj=0
--ÿ���Ȱ�Ӱ��ֵ��0
interference_bjx=0
interference_dlbl=0
--���Ӹ��ŷɻ�ֻ����һ�θ���&#x0D;
 if (plane_dzj_bjx+plane_dzj_dlbl)>0
 then	
 --����һ��һ����&#x0A;	 
	 if tactic_yc==1
		 then
			if missile_yc>=launcher_yc*3 --��������Ϊ��������
			then
				if launcher_yc*3>=(plane_dzj_bjx+plane_dzj_dlbl)
				then
					loss_bjx_dzj=plane_dzj_bjx*prob_yc
					loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
					--����ʣ�ർ������
					missile_yc=missile_yc-plane_dzj_bjx-plane_dzj_dlbl
				else
				--���ȱ������貨��&#x0D;
					if launcher_yc*3<=plane_dzj_dlbl
					then	
						loss_bjx_dzj=0
						loss_dlbl_dzj=launcher_yc*3*prob_yc
						--����ʣ�ർ������
						missile_yc=missile_yc-launcher_yc*3
					else
					--��ֹ�޹�����ʱ�������
						if plane_dzj_bjx>0
						then
							loss_bjx_dzj=(launcher_yc*3-plane_dzj_dlbl)*prob_yc
						else
							loss_bjx_dzj=0
						end
						loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
						--����ʣ�ർ������
						missile_yc=missile_yc-launcher_yc*3
					end
				end
			else
				if missile_yc>=(plane_dzj_bjx+plane_dzj_dlbl)
				then
					loss_bjx_dzj=plane_dzj_bjx*prob_yc
					loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
					--����ʣ�ർ������
					missile_yc=missile_yc-plane_dzj_bjx-plane_dzj_dlbl
				else
					if missile_yc<=plane_dzj_dlbl
					then	
						loss_bjx_dzj=0
						loss_dlbl_dzj=missile_yc*prob_yc
						--����ʣ�ർ������
						missile_yc=0
					else	
						--��ֹ�޹�����ʱ�������
					 if plane_dzj_bjx>0
						then
							loss_bjx_dzj=(missile_yc-plane_dzj_dlbl)*prob_yc
						else
							loss_bjx_dzj=0
						end
						loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
						--����ʣ�ർ������
						missile_yc=0
					end
				end			
			end
	 --���ö���һ����&#x0A;	 
	 elseif tactic_yc==2
	 then
		loss_bjx_dzj=plane_dzj_bjx*prob_yc
		loss_dlbl_dzj=plane_dzj_dlbl*prob_yc
		--����ʣ�ർ������
		missile_yc=missile_yc-(plane_dzj_bjx+plane_dzj_dlbl)*2
	 end
	 
	 --����ʧ�ĵ���ս�ɻ���������ȡ��
	 loss_bjx_dzj=loss_bjx_dzj-loss_bjx_dzj%1
	 loss_dlbl_dzj=loss_dlbl_dzj-loss_dlbl_dzj%1
	 
	 --����ʣ��ĵ���ս�ɻ�����
	 plane_dzj=plane_dzj-(loss_bjx_dzj+loss_dlbl_dzj)
	 
	 --������Ӹ��Ŷ԰�����͵��貨���Ӱ��
	 --interference_bjx=math.min(0.4,(plane_dzj_bjx-loss_bjx_dzj)*0.1)
	 --interference_dlbl=math.min(0.4,(plane_dzj_dlbl-loss_dlbl_dzj)*0.1)

 end
 --������Ӹ��Ŷ԰�����͵��貨���Ӱ��
 interference_bjx=math.min(0.4,(plane_dzj_bjx-loss_bjx_dzj)*0.1)
 interference_dlbl=math.min(0.4,(plane_dzj_dlbl-loss_dlbl_dzj)*0.1)
 prob_bjx=prob_bjx*(1-interference_bjx)
 prob_dlbl=prob_dlbl*(1-interference_dlbl)
 
 
--20200108
--�԰����/���貨����з���

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
	
 --------�������ս����----------------

 --����һ��һ����&#x0A;	 
	 if tactic_yc==1
		 then
			if missile_yc>=launcher_yc*3 --��������Ϊ��������
			then
				if launcher_yc*3>=(plane_gjj_bjx+plane_hzj_bjx)
				then
					loss_bjx_gjj=plane_gjj_bjx*prob_yc
					loss_bjx_hzj=plane_hzj_bjx*prob_yc
					--����ʣ�ർ������
					missile_yc=missile_yc-plane_gjj_bjx-plane_hzj_bjx
				else
					if launcher_yc*3<=plane_hzj_bjx
					then	
						loss_bjx_gjj=0
						loss_bjx_hzj=launcher_yc*3*prob_yc
						--����ʣ�ർ������
						missile_yc=missile_yc-launcher_yc*3
					else
						--��ֹ�޹�����ʱ�������
						if plane_gjj_bjx>0
						then
							loss_bjx_gjj=(launcher_yc*3-plane_hzj_bjx)*prob_yc
						else
							loss_bjx_gjj=0
						end
						loss_bjx_hzj=plane_hzj_bjx*prob_yc
						--����ʣ�ർ������
						missile_yc=missile_yc-launcher_yc*3
					end
				end
			else
				if missile_yc>=(plane_gjj_bjx+plane_hzj_bjx)
				then
					loss_bjx_gjj=plane_gjj_bjx*prob_yc
					loss_bjx_hzj=plane_hzj_bjx*prob_yc
					--����ʣ�ർ������
					missile_yc=missile_yc-plane_gjj_bjx-plane_hzj_bjx
				else
					if missile_yc<=plane_hzj_bjx
					then	
						loss_bjx_gjj=0
						loss_bjx_hzj=missile_yc*prob_yc
						--����ʣ�ർ������
						missile_yc=0
					else	
						--��ֹ�޹�����ʱ�������
					 if plane_gjj_bjx>0
						then
							loss_bjx_gjj=(missile_yc-plane_hzj_bjx)*prob_yc
						else
							loss_bjx_gjj=0
						end
						loss_bjx_hzj=plane_hzj_bjx*prob_yc
						--����ʣ�ർ������
						missile_yc=0
					end
				end			
			end
	 --���ö���һ����&#x0A;	 
	 elseif tactic_yc==2
	 then
		loss_bjx_gjj=plane_gjj_bjx*prob_yc
		loss_bjx_hzj=plane_hzj_bjx*prob_yc
		--����ʣ�ർ������
		missile_yc=missile_yc-(plane_hzj_bjx+plane_gjj_bjx)*2
	 end
	
	 --��ս����������ȡ��
	 loss_bjx_gjj=loss_bjx_gjj-loss_bjx_gjj%1
	 loss_bjx_hzj=loss_bjx_hzj-loss_bjx_hzj%1
	 
	 --����������������ʣ��ս��
	 plane_gjj_bjx=math.max(plane_gjj_bjx-loss_bjx_gjj,0) 
	 plane_hzj_bjx=math.max(plane_hzj_bjx-loss_bjx_hzj,0)
	 
	 --����ʣ��ս������
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

 --------���貨�ｻս����----------------

 --����һ��һ����&#x0A;	 
	 if tactic_yc==1
	 then
		if missile_yc>=launcher_yc*3 --��������Ϊ��������
		then
			if launcher_yc*3>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_yc
				loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
				--����ʣ�ർ������
				missile_yc=missile_yc-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if launcher_yc*3<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=launcher_yc*3*prob_yc
					--����ʣ�ർ������
					missile_yc=missile_yc-launcher_yc*3
				else
					--��ֹ�޹�����ʱ�������
					if plane_gjj_dlbl>0
					then
						loss_dlbl_gjj=(launcher_yc*3-plane_hzj_dlbl)*prob_yc
					else
						loss_dlbl_gjj=0
					end
					loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
					--����ʣ�ർ������
					missile_yc=missile_yc-launcher_yc*3
				end
			end
		else
			if missile_yc>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_yc
				loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
				--����ʣ�ർ������
				missile_yc=missile_yc-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if missile_yc<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=missile_yc*prob_yc
					--����ʣ�ർ������
					missile_yc=0
				else	
					--��ֹ�޹�����ʱ�������
				 if plane_gjj_dlbl>0
					then
						loss_dlbl_gjj=(missile_yc-plane_hzj_dlbl)*prob_yc
					else
						loss_dlbl_gjj=0
					end
					loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
					--����ʣ�ർ������
					missile_yc=0
				end
			end			
		end
	 --���ö���һ����&#x0A;	 
	 elseif tactic_yc==2
	 then
		loss_dlbl_gjj=plane_gjj_dlbl*prob_yc
		loss_dlbl_hzj=plane_hzj_dlbl*prob_yc
		--����ʣ�ർ������
		missile_yc=missile_yc-(plane_hzj_dlbl+plane_gjj_dlbl)*2
	 end
	
	--��ս����������ȡ��
	 loss_dlbl_gjj=loss_dlbl_gjj-loss_dlbl_gjj%1
	 loss_dlbl_hzj=loss_dlbl_hzj-loss_dlbl_hzj%1
	 
	 --����������������ʣ��ս��
	 plane_gjj_dlbl=math.max(plane_gjj_dlbl-loss_dlbl_gjj,0) 
	 plane_hzj_dlbl=math.max(plane_hzj_dlbl-loss_dlbl_hzj,0)	 
	 
	 --����ʣ��ս������
	 plane_gjj=math.max(plane_gjj-loss_dlbl_gjj,0) 
	 plane_hzj=math.max(plane_hzj-loss_dlbl_hzj,0) 
end		
 
 bcanEnable=true
 
 return bcanEnable, step, t,missile_yc,launcher_yc,plane_dzj_bjx,plane_dzj_dlbl,prob_yc,tactic_yc,loss_dlbl_dzj,loss_bjx_dzj,interference_bjx,interference_dlbl,prob_bjx,prob_dlbl,plane_dzj,plane_gjj_bjx,plane_hzj_bjx,plane_gjj_dlbl,plane_hzj_dlbl,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj
end