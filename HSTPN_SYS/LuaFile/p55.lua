function  p55(bcanEnable, step,n, tag1,missile_bjx,launcher_bjx,plane_dzj_bjx,plane_gjj_bjx,plane_hzj_bjx,prob_bjx,tactic_bjx)
   
   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
   --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
   --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
   --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
   --��2�������ϵ����ֱ������0��
   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

   --TODO:Write your function here! 

	n=1
	
    --������Դ����(һ��һor����һ)
	--tactic_bjx=1Ϊһ��һ
	--tactic_bjx=2Ϊ����һ
	
	if  (plane_gjj_bjx+plane_hzj_bjx)*2<launcher_bjx*3 and missile_bjx>(plane_gjj_bjx+plane_hzj_bjx)*2
	then
		tag1=1
		prob_bjx=0.5
		tactic_bjx=2
	else
		tag1=2
		prob_bjx=0.3
		tactic_bjx=1
	end
	
	bcanEnable=true
   
   return bcanEnable, step,n,tag1,missile_bjx,launcher_bjx,plane_dzj_bjx,plane_gjj_bjx,plane_hzj_bjx,prob_bjx,tactic_bjx
end
