function p79(bcanEnable, step, t,missile_bjx,launcher_bjx,plane_gjj_bjx,plane_hzj_bjx,prob_bjx,tactic_bjx,loss_bjx_gjj,loss_bjx_hzj,health_bjx, health_radar_bjx)
 
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
 loss_bjx_gjj=0
 loss_bjx_hzj=0
 
 --------�������ս����----------------
 if (plane_gjj_bjx+plane_hzj_bjx)>0
 then	
 --����һ��һ����&#x0A;	 
	 if tactic_bjx==1
	 then
		if missile_bjx>=launcher_bjx*3 --��������Ϊ��������
		then
			if launcher_bjx*3>=(plane_gjj_bjx+plane_hzj_bjx)
			then
				loss_bjx_gjj=plane_gjj_bjx*prob_bjx
				health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
				prob_bjx=health_radar_bjx/100*prob_bjx
				loss_bjx_hzj=plane_hzj_bjx*prob_bjx
				--����ʣ�ർ������
				missile_bjx=missile_bjx-plane_gjj_bjx-plane_hzj_bjx
			else
				if launcher_bjx*3<=plane_hzj_bjx
				then	
					loss_bjx_gjj=0
					loss_bjx_hzj=launcher_bjx*3*prob_bjx
					health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
					--����ʣ�ർ������
					missile_bjx=missile_bjx-launcher_bjx*3
				else
					--��ֹ�޹�����ʱ�������
					if plane_gjj_bjx>0
					then
						loss_bjx_gjj=(launcher_bjx*3-plane_hzj_bjx)*prob_bjx
						health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
						prob_bjx=health_radar_bjx/100*prob_bjx
					else
						loss_bjx_gjj=0
					end
					loss_bjx_hzj=plane_hzj_bjx*prob_bjx
					--����ʣ�ർ������
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
				--����ʣ�ർ������
				missile_bjx=missile_bjx-plane_gjj_bjx-plane_hzj_bjx
			else
				if missile_bjx<=plane_hzj_bjx
				then	
					loss_bjx_gjj=0
					loss_bjx_hzj=missile_bjx*prob_bjx
					health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
					--����ʣ�ർ������
					missile_bjx=0
				else	
				--��ֹ�޹�����ʱ�������
				 if plane_gjj_bjx>0
					then
						loss_bjx_gjj=(missile_bjx-plane_hzj_bjx)*prob_bjx
						health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
						prob_bjx=health_radar_bjx/100*prob_bjx
					else
						loss_bjx_gjj=0
					end
					loss_bjx_hzj=plane_hzj_bjx*prob_bjx
					--����ʣ�ർ������
					missile_bjx=0
				end
			end			
		end
	 --���ö���һ����&#x0A;	 
	 elseif tactic_bjx==2
	 then
		loss_bjx_gjj=plane_gjj_bjx*prob_bjx
		health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
		prob_bjx=health_radar_bjx/100*prob_bjx
		loss_bjx_hzj=plane_hzj_bjx*prob_bjx
		--����ʣ�ർ������
		missile_bjx=missile_bjx-(plane_hzj_bjx+plane_gjj_bjx)*2
	 end
 end
 
 --��ս����������ȡ��
 loss_bjx_gjj=loss_bjx_gjj-loss_bjx_gjj%1
 loss_bjx_hzj=loss_bjx_hzj-loss_bjx_hzj%1
 
 --������������ֵ
 --һ�ܺ�ը���Ի������8���˺�
 health_bjx=math.max(health_bjx-(plane_hzj_bjx-loss_bjx_hzj)*8,0)
 --health_radar_bjx=math.max(health_radar_bjx-(plane_gjj_bjx-loss_bjx_gjj)*5,0)
 
 bcanEnable=true
 
 return bcanEnable, step, t,missile_bjx,launcher_bjx,plane_gjj_bjx,plane_hzj_bjx,prob_bjx,tactic_bjx,loss_bjx_gjj,loss_bjx_hzj,health_bjx, health_radar_bjx
end
