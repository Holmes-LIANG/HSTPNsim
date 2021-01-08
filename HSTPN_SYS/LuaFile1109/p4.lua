PI=3.1415926
K = 1.380649*10^-23--������������
Pt = 1000000 -- ���书��
G = 50 -- ��������
lamda = 0.03 -- �״ﲨ��
sigma = 50 --Ŀ�귴�����
tau = 1 --������&#x0D;
T = 290 -- ��������ѧ�¶�&#x0D;
Br = 1000000 -- ����&#x0D;
L = 2.5 -- �������&#x0D;
F = 12 -- ���ջ�����ϵ��
n = 4-- �ź�������
h =4.75*math.sqrt(n)+n -- �״�������ֵ
PJ = 500 --���Ż��ķ��书��
GJ= 7--���Ż����״﷽��֮�������
GI= 50 --�״��ڸ��Ż�����֮�������
BJ=4000000--���Ż������źŴ���&#x0D;
L1 = 4 --�����źŵĴ������&#x0D;
RJ=300000 --�״�����Ż�֮��ľ���

function R(t)
	juli= 10000
	return juli 
end		

function SAN(t)
	san= (Pt*G^2*lamda^2*sigma)/((4*PI)^3*K*T*Br*L*F*R(t)^4)
	return san 
end	

function SANr(t)
	san= (Pt*G^2*RJ^2*sigma*L1*BJ)/(4*PI*PJ*R(t)^4*GI*GJ*L*Br)
	return san 	
end

function Rmax(t)
	rmax= ((Pt*n*tau*G^2*lamda^2*sigma)/((4*PI)^3*K*T*L*F*SANr(t)))^(1/4)
	return rmax 
end	
	
function Prd(t)
	
		pr= math.exp(-4.75/(math.sqrt(n)*SAN(t)))
 prd=1-(1-pr)
	
	return prd
end

function Prdr(t)
	
		pr= math.exp(-4.75/(math.sqrt(n)*SANr(t)))
 prd=1-(1-pr)
	
	return prd
end

function p4(bcanEnable, step, t)
 
 --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
 --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
 --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
 --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
 --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
 --��2�������ϵ����ֱ������0��
 --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

 --TODO:Write your function here! 

 t=0
 bcanEnable=true
 return bcanEnable, step, t
end
