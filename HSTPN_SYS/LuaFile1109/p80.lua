function p80(bcanEnable, step, t,missile_dlbl,launcher_dlbl,plane_gjj_dlbl,plane_hzj_dlbl,prob_dlbl,tactic_dlbl,loss_dlbl_gjj,loss_dlbl_hzj,health_dlbl, health_radar_dlbl)
 
 --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
 --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
 --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
 --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
 --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
 --��2�������ϵ����ֱ������0��
 --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

 --TODO:Write your function here! 

 t=0
 
 --ÿ���Ȱ���ʧ����0
 loss_dlbl_gjj=0
 loss_dlbl_hzj=0
 
--------���貨�ｻս����----------------
 if (plane_gjj_dlbl+plane_hzj_dlbl)>0
 then	
 --����һ��һ����&#x0A;	 
 if tactic_dlbl==1
	 then
		if missile_dlbl>=launcher_dlbl*3 --��������Ϊ��������
		then
			if launcher_dlbl*3>=(plane_gjj_dlbl+plane_hzj_dlbl)
			then
				loss_dlbl_gjj=plane_gjj_dlbl*prob_dlbl
				health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
				prob_dlbl=health_radar_dlbl/100*prob_dlbl
				loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
				--����ʣ�ർ������
				missile_dlbl=missile_dlbl-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if launcher_dlbl*3<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=launcher_dlbl*3*prob_dlbl
					health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
					--����ʣ�ർ������
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
					--����ʣ�ർ������
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
				--����ʣ�ർ������
				missile_dlbl=missile_dlbl-plane_gjj_dlbl-plane_hzj_dlbl
			else
				if missile_dlbl<=plane_hzj_dlbl
				then	
					loss_dlbl_gjj=0
					loss_dlbl_hzj=missile_dlbl*prob_dlbl
					health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
					--����ʣ�ർ������
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
					--����ʣ�ർ������
					missile_dlbl=0
				end
			end			
		end
 --���ö���һ����&#x0A;	 
 elseif tactic_dlbl==2
	 then
		loss_dlbl_gjj=plane_gjj_dlbl*prob_dlbl
		health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
		prob_dlbl=health_radar_dlbl/100*prob_dlbl
		loss_dlbl_hzj=plane_hzj_dlbl*prob_dlbl
		--����ʣ�ർ������
		missile_dlbl=missile_dlbl-(plane_hzj_dlbl+plane_gjj_dlbl)*2
	 end
 end
 
 --��ս����������ȡ��
 loss_dlbl_gjj=loss_dlbl_gjj-loss_dlbl_gjj%1
 loss_dlbl_hzj=loss_dlbl_hzj-loss_dlbl_hzj%1
 
 --������貨������ֵ
 --һ�ܺ�ը���Ի������8���˺�
 health_dlbl=math.max((health_dlbl-(plane_hzj_dlbl-loss_dlbl_hzj)*8),0) 
 --health_radar_dlbl=math.max(health_radar_dlbl-(plane_gjj_dlbl-loss_dlbl_gjj)*5,0)
 
 bcanEnable=true
 
 return bcanEnable, step, t,missile_dlbl,launcher_dlbl,plane_gjj_dlbl,plane_hzj_dlbl,prob_dlbl,tactic_dlbl,loss_dlbl_gjj,loss_dlbl_hzj,health_dlbl, health_radar_dlbl
end