function  p64(bcanEnable, step,n, tag1,missile_yc,launcher_yc,plane_dzj_bjx,plane_dzj_dlbl,prob_yc,tactic_yc)
   
   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
   --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
   --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
   --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
   --��2�������ϵ����ֱ������0��
   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

   --TODO:Write your function here! 
	n=1
	
    --�Ե���ս�ɻ����е�����Դ����(һ��һor����һ)
	if (plane_dzj_bjx+plane_dzj_dlbl)*2<launcher_yc*3 and missile_yc>(plane_dzj_bjx+plane_dzj_dlbl)*2 
	then
		tag1=1
		prob_yc=0.5
		tactic_yc=2
	else
		tag1=2
		prob_yc=0.3
		tactic_yc=1
	end
	
	bcanEnable=true
   
   return bcanEnable, step,n,tag1,missile_yc,launcher_yc,plane_dzj_bjx,plane_dzj_dlbl,prob_yc,tactic_yc
end
