function S_p7(bcanEnable, random_num,flag_bjx)
 
 --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
 --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
 --Note:�ڶ�������Ϊrandom_num���û��������趨�ĸ�����ֵ
 --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

 --TODO:Write your function here! 

 if flag_bjx==1 
 then
		random_num=1
		flag_bjx=0
 else
		random_num=0
 end
 
 bcanEnable=true
 
 return bcanEnable,random_num,flag_bjx
end
