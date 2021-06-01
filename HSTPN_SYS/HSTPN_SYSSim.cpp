// HSTPN_SYSSim.cpp : ʵ���ļ�
//



#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "HSTPN_SYSSim.h"


bool CHSTPN_SYSSim::startFlag = true;
vector<unordered_map<string, int>> CHSTPN_SYSSim::mapMoveData = {};
// CHSTPN_SYSSim

IMPLEMENT_DYNAMIC(CHSTPN_SYSSim, CWnd)

CHSTPN_SYSSim::CHSTPN_SYSSim()
{

}

CHSTPN_SYSSim::~CHSTPN_SYSSim()
{
}


BEGIN_MESSAGE_MAP(CHSTPN_SYSSim, CWnd)

END_MESSAGE_MAP()



// CHSTPN_SYSSim ��Ϣ�������


// �������ʱ��Petri�������㷨
#if 0
UINT CHSTPN_SYSSim::HSTPN_Simulation(LPVOID psimnet)
{
	//==1.��ȡ��������̵߳Ĳ���
	HSTPN_DATA              * hstpndata          = (HSTPN_DATA*)psimnet;
	vector<CPlace*>           vector_place       = hstpndata->vector_place;
	vector<CTransition*>      vector_trans       = hstpndata->vector_trans;
	CEvent                  * m_stopsimevent     = hstpndata->m_stopsimevent;
	double                    Time_Step          = hstpndata->m_timestep;
	vector<CCTRLShowMatrix*>  vector_outputmodel = hstpndata->vector_outputmodel;
	CHSTPN_SYSDoc           * m_pdoc             = hstpndata->m_pDoc;
	int t_interval = 500;

	//==2.��ʼ��һЩ����
	bool                  b_finishFlag = false;                                                              // ����һ��bool��������Petri��������ɣ���ΪTRUE,����ΪFALSE��
	bool                  btimeinitial = true;                                                               // ��־ֻ�е�һ�α�������ʱ����¼����ʱ�ӿ�����ʱ����ֵ
	vector<double>        arrytimets;                                                                        // ��HSTPN����֮ǰ�������������е�ʱ��ֵ��������
	arrytimets.clear();
	vector<CPlace*>       PActived;                                                                          // ���弤��������� PActived
	vector<CTransition*>  TFired;                                                                            // ����ʹ�ܱ�Ǩ���� TFired

	//==3.��ʼ�����㷨
	do{
		if(WaitForSingleObject(m_stopsimevent->m_hObject,(DWORD)0.001)!=WAIT_OBJECT_0)                       // ͨ��ֹͣ��ť�������߳��Ƿ��˳�
		{
			
			int n_haveDscrtRC = 0;                                                                           // ��ʼ����������������ͳ��PActived�и��ֿ���������
			int n_isallTime   = 0;                                                                           // n_haveDscrtRC��ʾ�ʱ��������������n_isallTime��ʾʱ�ӿ���������
			

			//==4.��ȡHSTPNģ���д��ڼ���״̬�Ŀ�����������PActived
			for(vector<CPlace*>::iterator iterp1=vector_place.begin();iterp1!=vector_place.end();iterp1++)
			{
				// ����һ�α���ʱ��btimeinitialΪtrue������¼���п�����ʱ����ֵ(��Ŀ����Ϊ�˼�¼ʱ�ӿ�����ʱ����ֵ��
				if(btimeinitial)
				{
					arrytimets.push_back((**iterp1).dTime);
				}
				
				// �����ڼ���״̬�Ŀ���������PActived��ͬʱ��¼�ʱ��������ʱ�ӿ����ĸ���
				if((**iterp1).bActivation && !(**iterp1).bisAssigned)                                           // ͬʱҪ��֤�����Ǽ���״̬����û�о���״̬�ݻ�����
				{
					(**iterp1).Invalidate();                                                                    // ����Ϊ����״̬�����̸ı���ɫ
					(**iterp1).UpdateWindow();
					PActived.push_back(*iterp1);

					if((**iterp1).PlaceStyle == STYLE_DISCRETE || (**iterp1).PlaceStyle == STYLE_RAND || (**iterp1).PlaceStyle == STYLE_TCTRL || (**iterp1).PlaceStyle == STYLE_LCTRL)
					{
						n_haveDscrtRC++;
					}
					if((**iterp1).PlaceStyle == STYLE_TIME)
					{
						n_isallTime++;
					}
				}
			}
			btimeinitial = false;


			//==5.�ж�PActived�Ƿ�Ϊ�գ���Ϊ�գ������Petri�����棬 ����Ϊ�գ������ִ��
			int n_PActived1 = PActived.size();                                                                  // ��ȡPActived���ϵĴ�С
			if(n_PActived1 == 0) 
			{
				b_finishFlag = TRUE;
				return 0;                                                                                       // return 0�����˳��̺߳���
			}
			else                 
			{

				//==6.�ж�PActived���Ƿ�����ɢ��������������Ϳ��ƿ���������n_haveDscrtRC!=0��������У�����������ִ��7
				if(n_haveDscrtRC!=0)
				{
					
					// ��ɢ��������������Ϳ��ƿ���������ʹ�ܣ�����PActived���Ƴ�;
					for(vector<CPlace*>::iterator iterp2=PActived.begin();iterp2!=PActived.end();iterp2++)      // ���ʱ������������ʹ��
					{
						if((**iterp2).PlaceStyle == STYLE_DISCRETE || (**iterp2).PlaceStyle == STYLE_RAND || 
						   (**iterp2).PlaceStyle == STYLE_TCTRL    || (**iterp2).PlaceStyle == STYLE_LCTRL   )
						{
							(**iterp2).bEnable=(**iterp2).isEnable();
							(**iterp2).bActivation = FALSE;
							(**iterp2).bisAssigned = TRUE;

							UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc);

							(**iterp2).Invalidate();                                                            // ����ʹ�ܺ����̸ı���ɫ
							(**iterp2).UpdateWindow();
							Sleep(t_interval);
						}
					}

					// ���п���ʹ�ܺ󣬼���ת���ж��Ƿ��б�Ǩ���㷢������
					goto _GET_TRANSFIRED_;
				}
				else
				{

				//==7.�ж�PActived���Ƿ�ֻʣʱ�ӿ���������ǣ�����ִ�У�����ִ��8
					if(n_isallTime == PActived.size())                                                         // �����ʣ��ʱ�ӿ���
					{
						
						// ���ȣ��ҵ�PActived��ʱ����̵Ŀ���������ʹ�ܣ�����PActived��ɾ����
						double timeTS        = PActived.at(0)->dTime;                                          // ���ڼ�¼������ʱ��
						int    mintime_posn  = 0;                                                              // ���ڼ�¼ʱ����̿�����λ��
						int    n_count2      = 0;                                                              // ���ڼ�¼������λ��
						for(vector<CPlace*>::iterator iterp4=PActived.begin();iterp4!=PActived.end();iterp4++) // �ҵ�ʱ����̵�ʱ�ӿ���
						{
							if((**iterp4).dTime < timeTS)
							{
								timeTS       = (**iterp4).dTime;
								mintime_posn = n_count2;
							}
							n_count2++;
						}

						UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc);

						PActived.at(mintime_posn)->dTime         = PActived.at(mintime_posn)->dTime - timeTS;
						PActived.at(mintime_posn)->bEnable       = PActived.at(mintime_posn)->isEnable();      // ���ʱ�ӿ���ʹ��
						PActived.at(mintime_posn)->bActivation   = FALSE;
						PActived.at(mintime_posn)->bisAssigned   = TRUE;

						PActived.at(mintime_posn)->Invalidate();                                               // ����ʹ�ܺ����̸ı���ɫ
						PActived.at(mintime_posn)->UpdateWindow();
						Sleep(t_interval);

						// ��ʱ�����ʹ�ܺ󣬽�������ʱ����ֵ���û�ԭ����ֵ 
						PActived.at(mintime_posn)->dTime         = timeTS;
						
						// ��ʹ�ܵ�ʱ�ӿ�����PActived�в���������ʱ�ӿ�����ʱ����ֵ���μ�ȥtimeTS
						vector<CPlace*>::iterator iterp5=PActived.begin();
						PActived.erase(iterp5+mintime_posn);                                                   // ��PActived��ɾ���ÿ���
						for(vector<CPlace*>::iterator iterp6=PActived.begin();iterp6!=PActived.end();iterp6++)
						{
							(**iterp6).dTime = (**iterp6).dTime - timeTS;
						}

						// ���п���ʹ�ܺ󣬼���ת���ж��Ƿ��б�Ǩ���㷢������
						goto _GET_TRANSFIRED_;	
					}
					else  
					{

					//==8.���ʣ�����������ʱ�ӿ���������������������ֻ����������������PActived�����п�����ʼ�����ݻ�
	_SINGLE_STEP_GO_:
						// PActived�п������ΰ������ݻ�
						for(vector<CPlace*>::iterator iterp7=PActived.begin();iterp7!=PActived.end();iterp7++) 
						{

							// ������Ϊʱ�ӿ���
							if((**iterp7).PlaceStyle == STYLE_TIME)
							{
								(**iterp7).dTime = (**iterp7).dTime - Time_Step;
								(**iterp7).bEnable = (**iterp7).isEnable();

								//UpdateOutputModel(vector_place,vector_outputmodel);
							}

							// ������Ϊ������������߿���
							if((**iterp7).PlaceStyle == STYLE_CNTNUS || (**iterp7).PlaceStyle == STYLE_DISIC)
							{
								(**iterp7).bEnable = (**iterp7).isEnable(Time_Step);

								UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc);
							}
						}

						// PActived�п��������ݻ�һ���������ж��Ƿ�������ʹ�������Ŀ���
						int          n_count3 = 0;                                                             // ������һ��������������,��ʾÿ�������� PActived �е�λ��
						vector<int>  vec_posn_P2;                                                              // ��¼ʹ�ܿ�����λ��

						for(vector<CPlace*>::iterator iterp8=PActived.begin();iterp8!=PActived.end();iterp8++)
						{
							if((**iterp8).bEnable)
							{
								vec_posn_P2.push_back(n_count3);

								(**iterp8).bActivation  = FALSE;
								(**iterp8).bisAssigned  = TRUE;

								(**iterp8).Invalidate();                                                       // ����ʹ�ܺ����̸ı������ɫ
								(**iterp8).UpdateWindow();
								Sleep(t_interval);

								
								if((**iterp8).PlaceStyle==STYLE_TIME)                                          // ���ʹ�ܵĿ���Ϊʱ�ӿ�����ʹ�ܺ󣬻ָ���ʱ����ֵΪ��ʼֵ
								{
									int ntimep=0;//�����������ã���ʾʱ�ӿ����ڿ������е�λ��
									for(vector<CPlace*>::iterator itertime_p  = vector_place.begin();
										                          itertime_p != vector_place.end();  itertime_p++) //�����������ҵ���ʱ�ӿ����ڿ������е�λ��
									{
										if((**iterp8).u_SubscriptofModel == (**itertime_p).u_SubscriptofModel)
										{
											(**iterp8).dTime=arrytimets.at(ntimep);
										}
										ntimep++;
									}
								}
								//-----�˴�������ÿ���Ϊ������������ô����������ִ��֮ǰ����������������ֵΪ��ʼ״̬+++++++++++++++++++++++5.7
								//if((**iterp8).PlaceStyle == STYLE_CNTNUS)
								//{
								//	for(vector<IOValue*>::iterator iter1=(**iterp8).arryOutputofLua/*arryIOputData*/.begin();iter1!= (**iterp8).arryOutputofLua/*arryIOputData*/.end();iter1++)
								//	{
								//		(**iter1).Value=0;
								//	}
								//}
								//------------++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++5.7
							}
							n_count3++;
						}

						// �����������ʹ�ܵĿ���������vec_posn_P2.size()��=0��������ת���ж��Ƿ��б�Ǩ���㷢�����������򣬼����ݻ���һ������
						int n_vec_posn_P2 = vec_posn_P2.size();
						if(n_vec_posn_P2!=0)
						{
							goto _GET_TRANSFIRED_;
						}
						else
						{
							goto _SINGLE_STEP_GO_;
						}
					}
				}

				//==10.ɨ��Petri�����ҵ����㷢�������ı�Ǩ����TFired������Ϊ�գ�TFired�б�Ǩ���η�������ת��ִ��4
_GET_TRANSFIRED_:

				PActived.clear();

				// �Ƚ���ֵ�ĸ���
				UpdateIOValueofModel(vector_place,m_pdoc);


				for(vector<CTransition*>::iterator itert1=vector_trans.begin();itert1!=vector_trans.end();itert1++)
				{
					(**itert1).bFire = (**itert1).isFire();                                                        // ���㷢�������ı�Ǩ��������ʶ��true

					if((**itert1).bFire)
					{
						(**itert1).Invalidate();                                                                   // ��Ǩ���������̸ı���ɫ
						(**itert1).UpdateWindow();
						Sleep(t_interval);
						TFired.push_back(*itert1);

						//������Ǩ������ǰ�ÿ���
						for(vector<CPlace*>::iterator iter = (**itert1).arryTransPre.begin();iter!=(**itert1).arryTransPre.end();++iter)
						{
							(**iter).bEnable = false;
							(**iter).Invalidate();
							(**iter).UpdateWindow();
						}

						//������Ǩ�����к��ÿ���
						for(vector<CPlace*>::iterator iter1 = (**itert1).arryTransPst.begin();iter1!=(**itert1).arryTransPst.end();++iter1)
						{
							(**iter1).bActivation = true;
							(**iter1).bisAssigned = false;
						}

						(**itert1).bFire = FALSE;
						(**itert1).Invalidate();                                                                 // ��Ǩ������ɺ����̸ı���ɫ
						(**itert1).UpdateWindow();
					}
				}

				if(TFired.size()==0)
				{
					b_finishFlag = false;//TRUE;
				}
				else
				{
					TFired.clear();
				}
			}
		}
		else
		{
			return 0;
		}

	}while(!b_finishFlag);
	
	return 0;
}

#endif
UINT CHSTPN_SYSSim::HSTPN_SimEngine(LPVOID psimnet)
{
	//==1.��ȡ��������̵߳Ĳ���
	HSTPN_DATA              * hstpndata          = (HSTPN_DATA*)psimnet;
	vector<CPlace*>           vector_place       = hstpndata->vector_place;
	vector<CTransition*>      vector_trans       = hstpndata->vector_trans;
	CEvent                  * m_stopsimevent     = hstpndata->m_stopsimevent;
	double                    Time_Step          = hstpndata->m_timestep;
	double                    Sim_Time           = hstpndata->m_simtime;
	double                    t_interval         = hstpndata->m_intervaltime;
	vector<CCTRLShowMatrix*>  vector_outputmodel = hstpndata->vector_outputmodel;
	CMainFrame              * m_pmainframe       = hstpndata->m_pmainframe;
	CHSTPN_SYSDoc           * m_pdoc             = hstpndata->m_pDoc;
	
//	m_pmainframe->m_wndStatusBar.EnablePaneProgressBar(1,Sim_Time,true);// ���÷�����ȵ����Χ
	//int t_interval = 10;
	// ÿ�ε����ʼ���水ť���Ƚ��������ģ���еı�����ȫ�����

	for(vector<CCTRLShowMatrix*>::iterator iter=vector_outputmodel.begin();iter!=vector_outputmodel.end();iter++)
	{
		if((*iter)->m_pSMat_OutValue != NULL)
		{
			(*iter)->m_pSMat_OutValue->m_Chart_SMatrix.DeleteAllItems();
		}
	}
	// ÿ�ε����ʼ���水ť���Ƚ��������ģ���е�csv�ļ�����open������
	OpenAllDataFile(vector_outputmodel);

	//==2.��ʼ��һЩ����
	bool                  b_finishFlag = false;                                                              // ����һ��bool��������Petri��������ɣ���ΪTRUE,����ΪFALSE��
	vector<CPlace*>       PActived;                                                                          // ���弤��������� PActived
	vector<CTransition*>  TFired;                                                                            // ����ʹ�ܱ�Ǩ���� TFired
	double                SimClock     = 0;                                                                  // ����ʱ����  
	

	PassByValue(m_pdoc);
    //==3.��ʼ�����㷨
	do{
		InitPlaceLocalData(vector_place, m_pdoc);//+++++12.18

_CHECK_SIMTIME_:
		//PassByValue(m_pdoc);
		if(WaitForSingleObject(m_stopsimevent->m_hObject,(DWORD)0.001)!=WAIT_OBJECT_0)                       // ͨ��ֹͣ��ť�������߳��Ƿ��˳�
		{
			m_pmainframe->SendMessage(WM_MESSAGE_CHANGEPROGRESS,SimClock*10000,0);// ����ǰ����������õ�������
			if(SimClock < Sim_Time)                                                                          // �ж��Ƿ�ﵽ����ʱ��
			{
				int TimACntus_n = 0;                                                                             // ��¼PActived������ʱ�ӿ��������������ĸ���
				//==4.��ȡHSTPNģ���д��ڼ���״̬�Ŀ�����������PActived
				for(vector<CPlace*>::iterator iterp1=vector_place.begin();iterp1!=vector_place.end();iterp1++)
				{
					// �����ڼ���״̬�Ŀ���������PActived��ͬʱ��¼�ʱ��������ʱ�ӿ����ĸ���
					if((**iterp1).bActivation && !(**iterp1).bisAssigned)                                           // ͬʱҪ��֤�����Ǽ���״̬����û�о���״̬�ݻ�����
					{
						PActived.push_back(*iterp1);
						if((*iterp1)->PlaceStyle==STYLE_CNTNUS || (*iterp1)->PlaceStyle==STYLE_TIME || (*iterp1)->PlaceStyle == STYLE_DISIC)
						{
							TimACntus_n++;
						}
					}
				}

				// �����������Ϊ����token
				for(vector<CPlace*>::iterator iterp2=PActived.begin();iterp2!=PActived.end();iterp2++)
				{
					(*iterp2)->Invalidate();
					(*iterp2)->UpdateWindow();
				}
				//==5.�ж�PActived�Ƿ�Ϊ�գ���Ϊ�գ������Petri�����棬 ����Ϊ�գ������ִ��
				int n_PActived1 = PActived.size();                                                                  // ��ȡPActived���ϵĴ�С
				if(n_PActived1 == 0) 
				{
					CloseAllDataFile(vector_outputmodel);
					b_finishFlag = TRUE;
					return 0;                                                                                       // return 0�����˳��̺߳���
				}
				else                 
				{
					Sleep(t_interval);
					//==6.ʱ�����ƽ�һ��������PActived�ڵ����п����ݻ�һ������,��Ϊ���е����ģ�鸳ֵ
					// ʱ�����ƽ�һ������
					/*if(TimACntus_n > 0)
					{
						SimClock = SimClock + Time_Step;
					}*/
					SimClock = SimClock + Time_Step;
					
					// PActived�ڵ����п����ݻ�һ������
					vector<int> vec_posenablePA;
					int enbPA_n = 0;
					for(vector<CPlace*>::iterator iter2=PActived.begin();iter2!=PActived.end();iter2++)
					{
						(*iter2)->bEnable = (*iter2)->isEnable(Time_Step);
						if((*iter2)->bEnable)
						{
							vec_posenablePA.push_back(enbPA_n);
						}
						enbPA_n++;
					}
					
					// PActived�ڵ����п����ݻ�һ������
					/*if(TimACntus_n > 0)
					{
						UpdateOutputModel(vector_place,vector_outputmodel );
					}*/
					UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc );

					//==7.�ж�PActived���Ƿ���ʹ�ܵĿ���������У�ʹ�ܿ�����token�ӿ��ı�Ϊʵ�ģ����û�У������ж��Ƿ񵽴����ʱ��
					if(0 != vec_posenablePA.size())
					{
						for(vector<int>::iterator iter3=vec_posenablePA.begin();iter3!=vec_posenablePA.end();iter3++)
						{
							PActived.at((*iter3))->Invalidate();
							PActived.at((*iter3))->UpdateWindow();
						}
						Sleep(t_interval);
					}
					else
					{
						// ���ؼ���Ƿ񵽴����ʱ��
					//	PActived.clear();//++++++++8.09

						UpdateIOValueofModel(vector_place,m_pdoc,PActived);//++++++++++++8.09  // ÿ���������¾ֲ���ȫ�ֱ������ֵ�ĺ���д�Ĳ���
						PActived.clear();// �ȸ��������
						goto _CHECK_SIMTIME_;
					}

					//==8.PActived������գ������¿��������������
				//	PActived.clear();

					UpdateIOValueofModel(vector_place,m_pdoc,PActived);
					PActived.clear();//
					//==9.ɨ��Petri�����ҵ�ʹ�ܱ�Ǩ����TFired
					for(vector<CTransition*>::iterator itert1=vector_trans.begin();itert1!=vector_trans.end();itert1++)
					{
						(**itert1).bFire = (**itert1).isFire();    // ���㷢�������ı�Ǩ��������ʶ��true

						if((**itert1).bFire)
						{
							TFired.push_back(*itert1);

							//������Ǩ������ǰ�ÿ���
							for(vector<CPlace*>::iterator iter = (*itert1)->arryTransPre.begin();iter!=(*itert1)->arryTransPre.end();++iter)
							{
								//��������߸Ľ�����
								if ((**iter).PlaceStyle == STYLE_DISIC&& (**iter).count){
									(**iter).count--;
									(**iter).bEnable = true;
								}
								else
								(**iter).bEnable = false;
							}

							//������Ǩ�����к��ÿ���
							for(vector<CPlace*>::iterator iter1 = (*itert1)->arryTransPst.begin();iter1!=(*itert1)->arryTransPst.end();++iter1)
							{
								(**iter1).bActivation = true;
								(**iter1).bisAssigned = false;
							}
							InitPlaceLocalData((*itert1)->arryTransPst, m_pdoc);//+++++12.18
						}
					}
					//==10.�ж�TFired�����Ƿ�Ϊ�գ����Ϊ�գ���ת���ж��Ƿ񵽴����ʱ�䣬�����Ϊ�գ���Ǩ���η���������ǰ�úͺ��ÿ�����token���޵�����
					if(0 == TFired.size())
					{
						b_finishFlag = false;
					}
					else
					{
						for(vector<CTransition*>::iterator itert2=TFired.begin();itert2!=TFired.end();itert2++)
						{
							// ǰ��token��ʧ
							for(vector<CPlace*>::iterator iter = (*itert2)->arryTransPre.begin();iter!=(*itert2)->arryTransPre.end();iter++)
							{
								(*iter)->Invalidate();
								(*iter)->UpdateWindow();
							}
							// ��Ǩ����ɫ
							(*itert2)->Invalidate();
							(*itert2)->UpdateWindow();
						}
						Sleep(t_interval);

						for(vector<CTransition*>::iterator itert3=TFired.begin();itert3!=TFired.end();itert3++)
						{
							// ��Ǩ��ذ�ɫ
							//TFired��OnPaint�иı�
							(*itert3)->bFire = false;
							(*itert3)->Invalidate();
							(*itert3)->UpdateWindow();
						}
						//Sleep(t_interval);
						TFired.clear();

						// ���ؼ���Ƿ񵽴����ʱ��
						goto _CHECK_SIMTIME_;
					}
				}
			}
			else
			{
				CloseAllDataFile(vector_outputmodel);
				return 0;
			}
		}
		else
		{
			CloseAllDataFile(vector_outputmodel);
			return 0;
		}

	}while(!b_finishFlag);
	
	return 0;
}

// ����������/���ֵ�ĸ��º���
void CHSTPN_SYSSim::UpdateIOValueofModel(vector<CPlace*> vector_place ,CHSTPN_SYSDoc * m_pdoc,vector<CPlace*> PActived)
{
	// �������ֵ�ĸ��·�������1��������������б�2��Ϊʱ�䡢���ʡ����Ƶ���Ҫ��ֵ�ı�����ֵ
	// ---------------------------------------------------������������б�-------------------------------Begin------------------------
//	for(vector<CPlace*>::iterator iterp1 = vector_place.begin();iterp1!=vector_place.end();iterp1++)// �������еĿ���--�ȸ�������б�
	for(vector<CPlace*>::iterator iterp1 = PActived.begin();iterp1!=PActived.end();++iterp1)
	{
		for(vector<IOValue*>::iterator iterout1 = (*iterp1)->arryIOputData.begin();iterout1 != (*iterp1)->arryIOputData.end();iterout1++)// ���������ı����б�--�������ֵ�������Ҫ��ֵ����ֵ
		{
			if((*iterout1)->IOType == "output" || (*iterout1)->IOType=="in/output")
			{
				if((*iterout1)->FromModel == "�����ʶ")
				{
					(*iterout1)->Value = (*iterp1)->bActivation;
				}
				if((*iterout1)->FromModel == "�����ʶ��ȡ����")
				{
					(*iterout1)->Value = !(*iterp1)->bActivation;
				}
				if((*iterout1)->FromModel == "ʹ�ܱ�ʶ")
				{
					(*iterout1)->Value = (*iterp1)->bEnable;
				}
				if((*iterout1)->FromModel == "ʹ�ܱ�ʶ��ȡ����")
				{
					(*iterout1)->Value = !(*iterp1)->bEnable;
				}
				if((*iterout1)->FromModel == "������ֵ")
				{
					(*iterout1)->Value = (*iterp1)->dProblty;
				}
				if((*iterout1)->FromModel == "ʱ����ֵ")
				{
					(*iterout1)->Value = (*iterp1)->d_TimeTS;//dTime;
				}
				if((*iterout1)->FromModel == "������")
				{
					(*iterout1)->Value = (*iterp1)->i_RandOutcome;
				}
				if((*iterout1)->FromModel == "��������ȡ����")
				{
					(*iterout1)->Value = !(*iterp1)->i_RandOutcome;
				}
				if((*iterout1)->FromModel == "�������ֵ")
				{
					(*iterout1)->Value = (*iterp1)->d_RandProblty;
				}
			}
			// ������������������ȫ�ֱ���
			if((*iterout1)->isQuoteG)
			{
				for(vector<IOValue*>::iterator iter=m_pdoc->arryIOputDataG.begin();iter!=m_pdoc->arryIOputDataG.end();iter++)
				{
					if((*iter)->Name == (*iterout1)->GlobalVluName)
					{
						double a = (*iterout1)->Value;
						for(vector<InValue>::iterator iter2=(*iterp1)->arryInputData.begin();iter2!=(*iterp1)->arryInputData.end();iter2++)
						{
							if((*iterout1)->Name == iter2->Name)
							{
								double b = iter2->Value;
								if(a != b)
								{
									(*iter)->Value = (*iterout1)->Value;
								}
								else{(*iter)->Value = (*iter)->Value;}
							}
						}
					}
				}
			}
		}
	}
	//ע�⣬���������ֵҪ��Դ������bisAssigned��ʶΪ�������½���
	for(vector<CPlace*>::iterator iterp2 = vector_place.begin();iterp2!=vector_place.end();iterp2++)// �������еĿ���--�ٸ��������б�
	{

		for(vector<IOValue*>::iterator iterin1 = (*iterp2)->arryIOputData.begin();iterin1 != (*iterp2)->arryIOputData.end();iterin1++)//������������������б������Ҫ��ֵ����ֵ
		{
			if((*iterin1)->IOType == "input" | (*iterin1)->IOType=="in/output")//++5.27
			{
				for(vector<CPlace*>::iterator iterp3 = vector_place.begin();iterp3!=vector_place.end();iterp3++)// �ڴα����������ҵ��������������Դ��ͬ�Ŀ�������
				{
					if((*iterin1)->FromModel == (*iterp3)->m_caption && (*iterp3)->bisAssigned) //�ҵ�����Դ�������ٻ�ȡ������б����ҵ������롰����Դ�б�������ͬ�������ı���
					{
						for(vector<IOValue*>::iterator iterout2 = (*iterp3)->arryIOputData.begin();iterout2 != (*iterp3)->arryIOputData.end();iterout2++)// ��������Դ�����������
						{
							if((*iterout2)->IOType == "output" | (*iterout2)->IOType=="in/output")//++5.27
							{
								if((*iterout2)->Name == (*iterin1)->FromMDVluName)
								{
									(*iterin1)->Value = (*iterout2)->Value;
									// ���������ȫ�ֱ���
									if((*iterin1)->isQuoteG)
									{
										for(vector<IOValue*>::iterator iterg=m_pdoc->arryIOputDataG.begin();iterg!=m_pdoc->arryIOputDataG.end();iterg++)
										{
											if((*iterg)->Name == (*iterin1)->GlobalVluName)
											{
												//(*iterg)->Value = (*iterin1)->Value;
												(*iterin1)->Value = (*iterg)->Value;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	// ---------------------------------------------------������������б�-------------------------------End--------------------------
	//----------------------------------------------------Ϊʱ�䡢���ʡ����Ƶ���Ҫ��ֵ�ı�����ֵ---------Begin------------------------
	for(vector<CPlace*>::iterator iterp3 = vector_place.begin();iterp3!=vector_place.end();iterp3++)// �������������ݿ����е�bisUserDef_prob��bisUserDef_time��bisUserDef_ctrl�����ж��Ƿ���Ҫ��ֵ�����ֵΪfalse�����ʾ�û�ѡ���ˡ���������ֵ��������Ҫ��ֵ
	{
		if(!(*iterp3)->bisUserDef_prob)  // ���������ֵ��Ҫ��ȡ
		{
			CString str_namevalue = (*iterp3)->str_Inputport_prob; //��ȡ������ֵ������Դ�����֣�����name��
			for(vector<IOValue*>::iterator iterin2 = (*iterp3)->arryIOputData.begin();iterin2 != (*iterp3)->arryIOputData.end();iterin2++)// ��������������������ҵ��ñ�������ɸ�ֵ
			{
				if((*iterin2)->IOType == "input" | (*iterin2)->IOType=="in/output")//++5.27
				{
					if((*iterin2)->Name == str_namevalue)
					{
						for(vector<CPlace*>::iterator iterp4 = vector_place.begin();iterp4 != vector_place.end();iterp4++)
						{
							if((*iterp4)->m_caption == (*iterin2)->FromModel && (*iterp4)->bisAssigned)// �ҵ�Դ����,ͬʱҪ��֤Դ������bisAssigned��ʶΪ��
							{
								(*iterp3)->dProblty = (*iterin2)->Value;
							}
						}			
					}
				}
			}

		}
		if(!(*iterp3)->bisUserDef_time) //���ʱ����ֵ��Ҫ��ȡ
		{
			CString str_namevalue = (**iterp3).str_Inputport_time; //��ȡʱ����ֵ������Դ�����֣�����name��
			for(vector<IOValue*>::iterator iterin2 = (*iterp3)->arryIOputData.begin();iterin2 != (*iterp3)->arryIOputData.end();iterin2++)// ��������������������ҵ��ñ�������ɸ�ֵ
			{
				if((*iterin2)->IOType == "input" | (*iterin2)->IOType=="in/output")//++5.27
				{
					if((*iterin2)->Name == str_namevalue)
					{
						for(vector<CPlace*>::iterator iterp4 = vector_place.begin();iterp4 != vector_place.end();iterp4++)
						{
							if((*iterp4)->m_caption == (*iterin2)->FromModel && (*iterp4)->bisAssigned)// �ҵ�Դ����,ͬʱҪ��֤Դ������bisAssigned��ʶΪ��
							{
								(*iterp3)->dTime = (*iterin2)->Value;
								(*iterp3)->d_TimeTS = (*iterp3)->dTime;
							}
						}
					
					}
				}
			}
		}
		if(!(*iterp3)->bisUserDef_ctrl) //������ƿ����ļ���ֵ��Ҫ��ȡ
		{
			CString str_namevalue = (*iterp3)->str_Inputport_ctrl; //��ȡ���ƿ����ļ���ֵ������Դ�����֣�����name��
			for(vector<IOValue*>::iterator iterin2 = (*iterp3)->arryIOputData.begin();iterin2 != (*iterp3)->arryIOputData.end();iterin2++)// ��������������������ҵ��ñ�������ɸ�ֵ
			{
				if((**iterin2).IOType == "input" | (*iterin2)->IOType=="in/output")//++5.27
				{
					if((**iterin2).Name == str_namevalue)
					{
						for(vector<CPlace*>::iterator iterp4 = vector_place.begin();iterp4 != vector_place.end();iterp4++)
						{
							if((*iterp4)->m_caption == (*iterin2)->FromModel && (*iterp4)->bisAssigned)// �ҵ�Դ����,ͬʱҪ��֤Դ������bisAssigned��ʶΪ��
							{
								if((*iterin2)->Value==0)
								{
									(*iterp3)->bActivation=false;
								}
								else
								{
									(*iterp3)->bActivation=true;
								}
							}
						}
					
					}
				}
			}
		}
	}





	// ��ֵ������ɺ󣬽����п�����bisAssigned��ʶ�ٴ�����Ϊfalse
	for(vector<CPlace*>::iterator iterp3 = vector_place.begin();iterp3!=vector_place.end();iterp3++)
	{
		(**iterp3).bisAssigned = FALSE;
	}
}

// ���������㷨
vector<CString> CHSTPN_SYSSim::IsLtdNodeDetect(vector<CPlace*> vector_place,vector<CTransition*> vector_trans)
{
	
	vector<CString> IsLtdNodeSet;
	IsLtdNodeSet.clear();
	// --�������п���
	for(vector<CPlace*>::iterator iterp=vector_place.begin();iterp!=vector_place.end();iterp++)
	{
		if((*iterp)->arryPlacePre.size()==0 && (*iterp)->arryPlacePst.size()==0)// �ҵ�ǰ�úͺ��þ�Ϊ0�Ŀ���
		{
			IsLtdNodeSet.push_back((*iterp)->m_caption);
		}
	}
	// --�������б�Ǩ
	for(vector<CTransition*>::iterator itert=vector_trans.begin();itert!=vector_trans.end();itert++)
	{
		if((*itert)->arryTransPre.size()==0 && (*itert)->arryTransPst.size()==0)// �ҵ�ǰ�úͺ��þ�Ϊ0�ı�Ǩ
		{
			IsLtdNodeSet.push_back((*itert)->m_caption);
		}
	}
	return IsLtdNodeSet;
}

// ģ�ͼ���㷨
UINT CHSTPN_SYSSim::ModelCheck(LPVOID m_modelcheckdata)
{
	// ��ȡ���ݵĲ���
	ModelCheckData * modelcheckdata = (ModelCheckData*)m_modelcheckdata;
	HSTPN_DATA * hstpndata = (HSTPN_DATA*)(modelcheckdata->m_hstpnData);
	vector<CPlace*> vector_place = hstpndata->vector_place;
	vector<CTransition*> vector_trans = hstpndata->vector_trans;
	BOOL m_btnIsLtdNode = modelcheckdata->m_btnIsLtdNode;
	BOOL m_btnConflict = modelcheckdata->m_btnConflict;
	BOOL m_btnContact = modelcheckdata->m_btnContact;
	BOOL m_btnDeadlock = modelcheckdata->m_btnDeadlock;
	BOOL m_btnTrap = modelcheckdata->m_btnTrap;
	// ����洢�ṹ�ļ��ϣ�
	vector<CString> IsLtdNodeSet;
	IsLtdNodeSet.clear();
	if(m_btnIsLtdNode)
	{
		IsLtdNodeSet = IsLtdNodeDetect(vector_place,vector_trans);
	}


	CWndAnalyLog m_wndanalylog(IsLtdNodeSet,1,NULL);
	m_wndanalylog.DoModal();



	return 0;
}

// ��������㷨
bool CHSTPN_SYSSim::isPureNet(vector<CPlace*> vector_place,vector<CTransition*> vector_trans)
{
	for(vector<CPlace*>::iterator iterp=vector_place.begin();iterp!=vector_place.end();iterp++)
	{
		for(vector<CTransition*>::iterator iterppre=(*iterp)->arryPlacePre.begin();iterppre!=(*iterp)->arryPlacePre.end();iterppre++)
		{
			for(vector<CTransition*>::iterator iterppst=(*iterp)->arryPlacePst.begin();iterppst!=(*iterp)->arryPlacePst.end();iterppst++)
			{
				if((*iterppre)->u_SubscriptofTrans == (*iterppst)->u_SubscriptofTrans)
				{
					return false; // if Pre-Transition == Post-Transition ,return false, that is it is not a pure net.
				}
			}
		}
	}
	return true;  // return true, that is it is a pure net.
}

// �����������
UINT CHSTPN_SYSSim::IncdnceMatCompute(LPVOID psimnet)
{
	HSTPN_DATA * hstpndata = (HSTPN_DATA*)psimnet;
	vector<CPlace*> vector_place = hstpndata->vector_place;
	vector<CTransition*> vector_trans = hstpndata->vector_trans;
	//if()
	vector<CString> arryltdnode = IsLtdNodeDetect(vector_place,vector_trans);//First ,Check Isolated node
	if(arryltdnode.size()!=0)
	{
		AfxMessageBox(L"���ڹ����ڵ㣬���Ƚ��й����ڵ�ļ�⣡");
	}
	else
	{
		bool b_ispurenet = isPureNet(vector_place,vector_trans);// Second, check the net that it is a pure net or not.
		if(!b_ispurenet)// if it is not a pure net ,pop-up Message Box , show that it is not a pure net ,can not calculate the incidence matrix.
		{
			AfxMessageBox(L"Petri��ģ�Ͳ��Ǵ�������������������");
		}
		else
		{
			vector<CString> IncdceMat_S_T;// ����ֲ����������ڱ���������󣬹��������ڴ�vector�а��б���
			IncdceMat_S_T.clear();

			int m = vector_place.size();//��ȡ�����ͱ�Ǩ�ĸ��������ڶ������Ĵ�С
			int n = vector_trans.size();
			vector<int> IncdnceMat_in(n*m);// �����������
			vector<int> IncdnceMat_out(n*m);// �����������

			int i = 1; // ��i��ʾ����������У���1��ʼ
			for(vector<CPlace*>::iterator iter1=vector_place.begin();iter1!=vector_place.end();iter1++)
			{
				// �����������
				for(vector<CTransition*>::iterator iter2=(*iter1)->arryPlacePre.begin();iter2!=(*iter1)->arryPlacePre.end();iter2++)
				{
					int j = 1;// j��ʾ����������
					for(vector<CTransition*>::iterator itert1=vector_trans.begin();itert1!=vector_trans.end();itert1++)// �ҵ���Ǩ�ڱ�Ǩ���е�λ��
					{
						if((*itert1)->u_SubscriptofTrans == (*iter2)->u_SubscriptofTrans)
						{
							break;
						}
						j++;
					}
					//IncdnceMat_in[(i-1)*n+j-1] = 1;
					IncdnceMat_in[(j-1)*m+i-1] = 1;
			
				}
				// �����������
				for(vector<CTransition*>::iterator iter3=(*iter1)->arryPlacePst.begin();iter3!=(*iter1)->arryPlacePst.end();iter3++)
				{
					int j = 1;
					for(vector<CTransition*>::iterator itert2=vector_trans.begin();itert2!=vector_trans.end();itert2++)
					{
						if((*itert2)->u_SubscriptofTrans == (*iter3)->u_SubscriptofTrans)
						{
							break;
						}
						j++;
					}
					//IncdnceMat_out[(i-1)*n+j-1] = 1;
					IncdnceMat_out[(j-1)*m+i-1] = 1;
				}
				i++;
			}
			// ��������������
			int l=0;
			for(vector<int>::iterator iter=IncdnceMat_in.begin();iter!=IncdnceMat_in.end();iter++)
			{
				CString str;
				str.Format(L"%d",IncdnceMat_in[l]-IncdnceMat_out[l]);
				IncdceMat_S_T.push_back(str);
				l++;
			}

			int sizeofP = vector_place.size();// ��ȡ�����ͱ�Ǩ�����������ݸ�������־����
			int sizeofT = vector_trans.size();

			CWndShowMatrix m_wndshowmatrix(IncdceMat_S_T,sizeofP,sizeofT,2,NULL);;
			m_wndshowmatrix.DoModal();
		}
	}
	return 0;
}

// OutputModelģ������ֵ��ֵ����
void CHSTPN_SYSSim::UpdateOutputModel(vector<CPlace*> vector_place,vector<CCTRLShowMatrix*> vector_outputmodel,CHSTPN_SYSDoc * m_pDoc )
{
	for(vector<CCTRLShowMatrix*>::iterator itero=vector_outputmodel.begin();itero!=vector_outputmodel.end();itero++)
	{
		//if((*itero)->OutputStyle == OUT_TOFILE)// �����OUT_TOFILE���͵����ģ�飬����Ϊ�䶨��һ��list�����ڴ��ÿ������
		//{
			list<string> str_valuerow;
		//}

		int subofOutData = 0;
		for(vector<int>::iterator iter=(*itero)->vec_OutValue.begin();iter!=(*itero)->vec_OutValue.end();iter++)
		{
			if((*itero)->OutputStyle == OUT_SHOWMATRIX)//�����OUT_SHOWMATRIXģ�飬�򽫱������α��浽vec_OutputData��
			{
				double *outvalue = new double();
				if((*itero)->OutPName == "ȫ�ֱ���") // ����������ȫ�ֱ���
				{
					(*outvalue) = m_pDoc->arryIOputDataG.at(*iter)->Value;
				}
				else // ���������ǿ����еľֲ�����
				{
					(*outvalue) = vector_place.at((*itero)->i_PposnOValue)->arryIOputData.at(*iter)->Value; //ȫ�����е��˴�����bug 
				}
				(*itero)->vec_OutputData.at(subofOutData)->push_back(outvalue);
			}
			else                                      // �����OUT_TOFILEģ�飬���Ƚ�һ�еı�����ֵ���浽list�У��ڵ���WriteDatatoFileд���ļ�
			{
				//CString str;
				//str.Format(_T("%d"),vector_place.at((*itero)->i_PposnOValue)->arryIOputData.at(*iter)->Value);
				ostringstream os_value;
				if((*itero)->OutPName == "ȫ�ֱ���")
				{
					os_value<<(m_pDoc->arryIOputDataG.at(*iter)->Value);
				}
				else
				{
					os_value<<(vector_place.at((*itero)->i_PposnOValue)->arryIOputData.at(*iter)->Value);
				}
				str_valuerow.push_back(os_value.str());
			}
			subofOutData++;
		}
		if(str_valuerow.size()!=0)
		{
			WriteDatatoFile((*itero)->m_pDatafile,str_valuerow);
		}
	}
}

// Output-OUT_TOFILEд���ļ�����
void CHSTPN_SYSSim::WriteDatatoFile(ofstream * file,const list<string> &vStr)
{
	//USES_CONVERSION;
	static TCHAR chComma = ',';
	int i = 0;
	for(list<string>::const_iterator striter=vStr.begin();striter!=vStr.end();striter++)
	{
		if(i > 0)
		{
			file->put(chComma);
		}
		*file << *striter;
		
		//const char* strvalue = T2A(*striter);
		//file->write(strvalue,strlen(strvalue));

		i++;
	}
	*file << endl;/**/
}

// �ر�OUT_TOFILEģ�����е������ļ�
void CHSTPN_SYSSim::CloseAllDataFile(vector<CCTRLShowMatrix*> vector_outputmodel)
{
	for(vector<CCTRLShowMatrix*>::iterator itero=vector_outputmodel.begin();itero!=vector_outputmodel.end();itero++)
	{
		if((*itero)->OutputStyle ==  OUT_TOFILE)
		{
			if((*itero)->m_pDatafile!=NULL)
				(*itero)->m_pDatafile->close();
		}
	}
}

// ��OUT_TOFILEģ�������е�datafile
void CHSTPN_SYSSim::OpenAllDataFile(vector<CCTRLShowMatrix*> vector_outputmodel)
{
	for(vector<CCTRLShowMatrix*>::iterator itero=vector_outputmodel.begin();itero!=vector_outputmodel.end();itero++)
	{
		if((*itero)->OutputStyle == OUT_TOFILE)
		{
			if((*itero)->m_pDatafile!=NULL)
			{
				USES_CONVERSION;
				const char * filepath = T2A((*itero)->str_FilePath);
				(*itero)->m_pDatafile->close();
				(*itero)->m_pDatafile->open(filepath);
			}
		}
	}
}

// �������ӡ�δ���״̬ת���������δʹ�ܣ���״̬ʱ���������еľֲ������Ƿ�������ȫ�ֱ�������������ˣ�����ȫ�ֱ���������Ϊ�ֲ�������ֵ��
void CHSTPN_SYSSim::InitPlaceLocalData(vector<CPlace*> vec_place,  CHSTPN_SYSDoc * m_pdoc)
{
	if(vec_place.size()!=0)
	{
		for(vector<CPlace*>::iterator iterp=vec_place.begin();iterp!=vec_place.end();iterp++)
		{
			if((*iterp)->arryIOputData.size()!=0)
			{
				for(vector<IOValue*>::iterator iter_io=(*iterp)->arryIOputData.begin();iter_io!=(*iterp)->arryIOputData.end();iter_io++)
				{
					if((*iter_io)->isQuoteG) // ������������ȫ�֣������ȫ�ֱ�
					{						
						for(vector<IOValue*>::iterator iterg=m_pdoc->arryIOputDataG.begin();iterg!=m_pdoc->arryIOputDataG.end();iterg++)
						{
							if((*iterg)->Name == (*iter_io)->GlobalVluName)
							{
								(*iter_io)->Value = (*iterg)->Value;
							}
						}
					}
				}
			}
		}
	}
}

//void UpdateMoveValue(CHSTPN_SYSDoc * m_pDoc)
//{
//	//for (vector<IOValue*>::iterator iter = m_pDoc->arryIOputDataG.begin(); iter != m_pDoc->arryIOputDataG.end(); iterp++)
//		arryMoveData.push_back(m_pDoc->arryIOputDataG);			
//}

vector<unordered_map<string, int>> CHSTPN_SYSSim::getMoveData()
{

	return mapMoveData;
}

void CHSTPN_SYSSim::PassByValue(CHSTPN_SYSDoc * m_pdoc) {
	unordered_map<string, int> mapMoveDataTmp;
	IOValue tmp;
	//ȥ���ظ�����
	//if(m_pdoc->arryIOputDataG.size()){
	//	for (vector<IOValue*>::iterator itero = m_pdoc->arryIOputDataG.begin(); itero != m_pdoc->arryIOputDataG.end(); itero++){
	//		tmp.Name = (*itero)->Name;
	//		tmp.Value = (*itero)->Value;

	//		CString cstr(tmp.Name);
	//		USES_CONVERSION;
	//		string str(W2A(cstr));

	//		mapMoveDataTmp[str] = tmp.Value;
	//	}
	//}
	//mapMoveData.push_back(mapMoveDataTmp);
	
	//ģ������
	//�����������ֵΪ0
	if (mapMoveDataTmp["health_bjx"] == 0 && mapMoveDataTmp["health_dlbl"] == 0) {
		return;
	}
	//��һ��ս��
	//mapMoveDataTmp["plane_hzj_bjx"] = 12;
	//mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	//mapMoveDataTmp["plane_hzj_yc"] = 0;
	//mapMoveDataTmp["plane_dzj_bjx"] = 6;
	//mapMoveDataTmp["plane_dzj_dlbl"] = 6;
	//mapMoveDataTmp["plane_dzj_yc"] = 6;
	//mapMoveDataTmp["plane_gjj_bjx"] = 8;
	//mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	//mapMoveDataTmp["plane_gjj_yc"] = 0;
	////�״�����ֵ
	//mapMoveDataTmp["health_radar_bjx"] = 100;
	//mapMoveDataTmp["health_radar_dlbl"] = 100;
	//mapMoveDataTmp["health_radar_yc"] = 100;
	////ս������
	//mapMoveDataTmp["plane_hzj"] = 36;
	//mapMoveDataTmp["plane_gjj"] = 20;
	//mapMoveDataTmp["plane_dzj"] = 12;
	////��������
	//mapMoveDataTmp["missile_bjx"] = 100;
	//mapMoveDataTmp["missile_dlbl"] = 100;
	//mapMoveDataTmp["missile_yc"] = 100;
	////��������ֵ
	//mapMoveDataTmp["health_bjx"] = 100;
	//mapMoveDataTmp["health_dlbl"] = 100;
	//mapMoveData.push_back(mapMoveDataTmp);
	//�ڶ���ս��

	mapMoveDataTmp["plane_hzj_bjx"] = 12;
	mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	mapMoveDataTmp["plane_hzj_yc"] = 0;
	mapMoveDataTmp["plane_dzj_bjx"] = 0;
	mapMoveDataTmp["plane_dzj_dlbl"] = 0;
	mapMoveDataTmp["plane_dzj_yc"] = 0;
	mapMoveDataTmp["plane_gjj_bjx"] = 8;
	mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	mapMoveDataTmp["plane_gjj_yc"] = 0;
	//�״�����ֵ
	mapMoveDataTmp["health_radar_bjx"] = mapMoveData[0]["health_radar_bjx"] - 25;
	mapMoveDataTmp["health_radar_dlbl"] = mapMoveData[0]["health_radar_dlbl"]-25;
	mapMoveDataTmp["health_radar_yc"] = mapMoveData[0]["health_radar_yc"];
	//ս������
	mapMoveDataTmp["plane_hzj"] = mapMoveData[0]["plane_hzj"]-6;
	mapMoveDataTmp["plane_gjj"] = mapMoveData[0]["plane_gjj"]-6;
	mapMoveDataTmp["plane_dzj"] = mapMoveData[0]["plane_dzj"]-6;
	//��������
	mapMoveDataTmp["missile_bjx"] = mapMoveData[0]["missile_bjx"] - 30;
	mapMoveDataTmp["missile_dlbl"] = mapMoveData[0]["missile_dlbl"] - 30;
	mapMoveDataTmp["missile_yc"] = mapMoveData[0]["missile_yc"] - 30;
	//��������ֵ
	mapMoveDataTmp["health_bjx"] = mapMoveData[0]["health_bjx"] - 45;
	mapMoveDataTmp["health_dlbl"] = mapMoveData[0]["health_dlbl"] - 45;
	mapMoveData.push_back(mapMoveDataTmp);
	//������ս��
	mapMoveDataTmp["plane_hzj_bjx"] = 12;
	mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	mapMoveDataTmp["plane_hzj_yc"] = 0;
	mapMoveDataTmp["plane_dzj_bjx"] = 0;
	mapMoveDataTmp["plane_dzj_dlbl"] = 0;
	mapMoveDataTmp["plane_dzj_yc"] = 0;
	mapMoveDataTmp["plane_gjj_bjx"] = 0;
	mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	mapMoveDataTmp["plane_gjj_yc"] = 0;
	//�״�����ֵ
	mapMoveDataTmp["health_radar_bjx"] = mapMoveData[1]["health_radar_bjx"] - 25;
	mapMoveDataTmp["health_radar_dlbl"] = mapMoveData[1]["health_radar_dlbl"] - 25;
	mapMoveDataTmp["health_radar_yc"] = mapMoveData[1]["health_radar_yc"];
	//ս������
	mapMoveDataTmp["plane_hzj"] = mapMoveData[1]["plane_hzj"] - 6;
	mapMoveDataTmp["plane_gjj"] = mapMoveData[1]["plane_gjj"] - 6;
	mapMoveDataTmp["plane_dzj"] = mapMoveData[0]["plane_hzj"] ;
	//��������
	mapMoveDataTmp["missile_bjx"] = mapMoveData[1]["missile_bjx"] - 30;
	mapMoveDataTmp["missile_dlbl"] = mapMoveData[1]["missile_dlbl"] - 30;
	mapMoveDataTmp["missile_yc"] = mapMoveData[1]["missile_yc"] - 30;
	//��������ֵ
	mapMoveDataTmp["health_bjx"] = mapMoveData[1]["health_bjx"] - 45;
	mapMoveDataTmp["health_dlbl"] = mapMoveData[1]["health_dlbl"] - 45;
	mapMoveData.push_back(mapMoveDataTmp);
}

