function  p34(bcanEnable, step, n, tag1,tag2,tag3,flag_dlbl,flag_yc,flag_dzj_dlbl,plane_dzj_dlbl,plane_gjj_dlbl,plane_hzj_dlbl)
   
   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
   --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
   --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
   --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
   --��2�������ϵ����ֱ������0��
   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

   --TODO:Write your function here! 

   n=3
   
   flag_dlbl=1
   flag_yc=1
   
   tag1=1
   tag2=2
   tag3=3
   
   --����ս�ɻ�ֻ����һ��
   --[[
   if plane_dzj_dlbl>0
   then
	   if(flag_dzj_dlbl==0)
	   then
		flag_dzj_dlbl=1
	   else
		plane_dzj_dlbl=0
		end
	end
	]]--
	--��û�к�ը���򲻽��й���
	if plane_hzj_dlbl==0
	then
		plane_dzj_dlbl=0
		plane_gjj_dlbl=0
		plane_hzj_dlbl=0	
    end
	
    bcanEnable=true
   
   return bcanEnable, step,n, tag1, tag2, tag3,flag_dlbl,flag_yc,flag_dzj_dlbl,plane_dzj_dlbl,plane_gjj_dlbl,plane_hzj_dlbl
end