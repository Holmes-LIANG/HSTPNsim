function p94(bcanEnable, step,t,plane_dzj,plane_gjj,plane_hzj,missile_bjx,missile_dlbl,missile_yc,health_bjx,health_dlbl,health_yc,health_radar_bjx,health_radar_dlbl,health_radar_yc,inc)
 
 --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���
 --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������
 --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������
 --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...
 --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����
 --��2�������ϵ����ֱ������0��
 --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����

 --TODO:Write your function here! 
 

 plane_dzj=18
 plane_gjj=18
 plane_hzj=32
 
 missile_bjx=100
 missile_dlbl=100
 missile_yc=100
 
 
 health_bjx=100
 health_dlbl=100
 health_yc=100
 health_radar_bjx=100
 health_radar_dlbl=100
 health_radar_yc=100

-- plane_hzj=plane_hzj-inc
 
 inc=inc+1

 --done=0 --ս����ɱ�־λ���㣬���¿�ʼһ��ս��
 bcanEnable=true
 
 return bcanEnable, step, t,plane_dzj,plane_gjj,plane_hzj,missile_bjx,missile_dlbl,missile_yc,health_bjx,health_dlbl,health_yc,health_radar_bjx,health_radar_dlbl,health_radar_yc,inc
end
