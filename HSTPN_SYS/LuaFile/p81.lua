function  p81(bcanEnable, step, n, tag1,output_plane_dzj,output_plane_gjj,output_plane_hzj,output_health_bjx,output_health_dlbl,out_health_radar_bjx,out_health_radar_dlbl,out_health_radar_yc,done,placeholder1,placeholder2,placeholder3,health_yc,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj,plane_dzj,missile_bjx,missile_dlbl,missile_yc,health_radar_bjx,health_radar_dlbl,health_radar_yc)
   
   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
   --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
   --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
   --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
   --��2�������ϵ����ֱ������0��
   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

   --TODO:Write your function here! 
 
  --[[ ��ʼֵ
   plane_dzj=18
   plane_gjj=12
   palane_hzj=30
   
   missile_bjx=10
   missile_dlbl=10
   missile_ycdd=10
   
   health_bjx=100
   halth_dlbl=100
  ]] 
   n=1
   
   plane_gjj=math.max(plane_gjj-(loss_bjx_gjj+loss_dlbl_gjj),0) 
   plane_hzj=math.max(plane_hzj-(loss_bjx_hzj+loss_dlbl_hzj),0) 
   
   if (health_bjx<=0 and health_dlbl<=0) 
   then
	tag1=1 --����ʤ��
	done=1
   elseif plane_hzj<12
   then
	tag1=3 --����ʤ��
	done=1
   else
	tag1=2 --����ս��
	done=0
   end
   ---flag_victory
   --tag1=2
   
   output_plane_dzj=plane_dzj+100
   output_plane_gjj=plane_gjj+100 
   output_plane_hzj=plane_hzj+100
   output_health_bjx=health_bjx+100
   output_health_dlbl=health_dlbl+100
   out_health_radar_bjx=health_radar_bjx+100
   out_health_radar_dlbl=health_radar_dlbl+100 
   out_health_radar_yc=health_radar_yc+100 
   
   bcanEnable=true

   return bcanEnable, step, n, tag1,output_plane_dzj,output_plane_gjj,output_plane_hzj,output_health_bjx,output_health_dlbl,out_health_radar_bjx,out_health_radar_dlbl,out_health_radar_yc,done,placeholder1,placeholder2,placeholder3,health_yc,health_bjx,health_dlbl,loss_bjx_gjj,loss_bjx_hzj,loss_dlbl_gjj,loss_dlbl_hzj,plane_gjj,plane_hzj,plane_dzj,missile_bjx,missile_dlbl,missile_yc,health_radar_bjx,health_radar_dlbl,health_radar_yc
end
