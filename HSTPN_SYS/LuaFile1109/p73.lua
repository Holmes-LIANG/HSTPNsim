function  p73(bcanEnable, step,n, tag1,missile_dlbl,launcher_dlbl,plane_dzj_dlbl,plane_gjj_dlbl,plane_hzj_dlbl,prob_dlbl,tactic_dlbl)
   
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
	if (plane_gjj_dlbl+plane_hzj_dlbl)*2<launcher_dlbl*3 and missile_dlbl>(plane_gjj_dlbl+plane_hzj_dlbl)
	then
		tag1=1
		prob_dlbl=0.8
		tactic_dlbl=2 --����һ����
	else
		tag1=2
		prob_dlbl=0.5
		tactic_dlbl=1 --һ��һ����
	end
	
	bcanEnable=true
   
   return bcanEnable, step,n,tag1,missile_dlbl,launcher_dlbl,plane_dzj_dlbl,plane_gjj_dlbl,plane_hzj_dlbl,prob_dlbl,tactic_dlbl
end