function  p2(bcanEnable, step,n, tag1, tag2,plane_dzj,plane_gjj,plane_hzj,health_bjx,health_dlbl,plane_dzj_bjx,plane_dzj_dlbl,plane_gjj_bjx,plane_gjj_dlbl,plane_hzj_bjx,plane_hzj_dlbl)
   
   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
   --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
   --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
   --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
   --��2�������ϵ����ֱ������0��
   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

   --TODO:Write your function here! 
 
    n=2
 
 	tag1=1
    tag2=2  
	
	--����ս�ɻ���Դ����
	if plane_dzj>=12
	then
		plane_dzj_bjx=6
		plane_dzj_dlbl=6
	elseif plane_dzj<12 and plane_dzj>=6
	then
      if (health_bjx>health_dlbl) 
	  then
		plane_dzj_bjx=6
		plane_dzj_dlbl=0
	  else
	    plane_dzj_bjx=0
	    plane_dzj_dlbl=6
	  end
	else
		plane_dzj_bjx=0
		plane_dzj_dlbl=0
	end
	
	--��������Դ����
	if plane_gjj>=16
	then
		plane_gjj_bjx=8
		plane_gjj_dlbl=8
	elseif plane_gjj<16 and plane_gjj>=8
	then
      if (health_bjx>health_dlbl) 
	  then
		plane_gjj_bjx=8
		plane_gjj_dlbl=0
	  else
	    plane_gjj_bjx=0
	    plane_gjj_dlbl=8
	  end
	else
		plane_gjj_bjx=0
		plane_gjj_dlbl=0
	end
	
	--��ը����Դ����
	if plane_hzj>=24
	then
		plane_hzj_bjx=12
		plane_hzj_dlbl=12
	elseif plane_hzj<24 and plane_hzj>=12
	then
      if (health_bjx>health_dlbl) 
	  then
		plane_hzj_bjx=12
		plane_hzj_dlbl=0
	  else
	    plane_hzj_bjx=0
	    plane_hzj_dlbl=12
	  end
	else
		plane_hzj_bjx=0
		plane_hzj_dlbl=0
	end
	
	bcanEnable=true
   
   return bcanEnable, step,n,tag1, tag2,plane_dzj,plane_gjj,plane_hzj,health_bjx,health_dlbl,plane_dzj_bjx,plane_dzj_dlbl,plane_gjj_bjx,plane_gjj_dlbl,plane_hzj_bjx,plane_hzj_dlbl
end
