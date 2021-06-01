// HSTPN_SYSSim.cpp : 实现文件
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



// CHSTPN_SYSSim 消息处理程序


// 混杂随机时延Petri网仿真算法
#if 0
UINT CHSTPN_SYSSim::HSTPN_Simulation(LPVOID psimnet)
{
	//==1.获取输入仿真线程的参数
	HSTPN_DATA              * hstpndata          = (HSTPN_DATA*)psimnet;
	vector<CPlace*>           vector_place       = hstpndata->vector_place;
	vector<CTransition*>      vector_trans       = hstpndata->vector_trans;
	CEvent                  * m_stopsimevent     = hstpndata->m_stopsimevent;
	double                    Time_Step          = hstpndata->m_timestep;
	vector<CCTRLShowMatrix*>  vector_outputmodel = hstpndata->vector_outputmodel;
	CHSTPN_SYSDoc           * m_pdoc             = hstpndata->m_pDoc;
	int t_interval = 500;

	//==2.初始化一些变量
	bool                  b_finishFlag = false;                                                              // 定义一个bool变量，当Petri网推演完成，即为TRUE,否则为FALSE。
	bool                  btimeinitial = true;                                                               // 标志只有第一次遍历库所时，记录所有时延库所的时间阈值
	vector<double>        arrytimets;                                                                        // 在HSTPN推演之前，将各个库所中的时间值保存起来
	arrytimets.clear();
	vector<CPlace*>       PActived;                                                                          // 定义激活库所集合 PActived
	vector<CTransition*>  TFired;                                                                            // 定义使能变迁集合 TFired

	//==3.开始推演算法
	do{
		if(WaitForSingleObject(m_stopsimevent->m_hObject,(DWORD)0.001)!=WAIT_OBJECT_0)                       // 通过停止按钮，控制线程是否退出
		{
			
			int n_haveDscrtRC = 0;                                                                           // 初始化几个变量，用来统计PActived中各种库所的数量
			int n_isallTime   = 0;                                                                           // n_haveDscrtRC表示质变类库所的数量；n_isallTime表示时延库所的数量
			

			//==4.获取HSTPN模型中处于集合状态的库所，保存入PActived
			for(vector<CPlace*>::iterator iterp1=vector_place.begin();iterp1!=vector_place.end();iterp1++)
			{
				// 当第一次遍历时（btimeinitial为true），记录所有库所的时间阈值(但目的是为了记录时延库所的时间阈值）
				if(btimeinitial)
				{
					arrytimets.push_back((**iterp1).dTime);
				}
				
				// 将处于激活状态的库所保存入PActived，同时记录质变类库所和时延库所的个数
				if((**iterp1).bActivation && !(**iterp1).bisAssigned)                                           // 同时要保证库所是激活状态，但没有经过状态演化过程
				{
					(**iterp1).Invalidate();                                                                    // 发现为激活状态，立刻改变颜色
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


			//==5.判断PActived是否为空，若为空，则结束Petri网仿真， 若不为空，则继续执行
			int n_PActived1 = PActived.size();                                                                  // 获取PActived集合的大小
			if(n_PActived1 == 0) 
			{
				b_finishFlag = TRUE;
				return 0;                                                                                       // return 0，即退出线程函数
			}
			else                 
			{

				//==6.判断PActived中是否有离散库所、随机库所和控制库所（即，n_haveDscrtRC!=0），如果有，继续；否则执行7
				if(n_haveDscrtRC!=0)
				{
					
					// 离散库所、随机库所和控制库所按规则使能，并从PActived中移除;
					for(vector<CPlace*>::iterator iterp2=PActived.begin();iterp2!=PActived.end();iterp2++)      // 令质变类库所按规则使能
					{
						if((**iterp2).PlaceStyle == STYLE_DISCRETE || (**iterp2).PlaceStyle == STYLE_RAND || 
						   (**iterp2).PlaceStyle == STYLE_TCTRL    || (**iterp2).PlaceStyle == STYLE_LCTRL   )
						{
							(**iterp2).bEnable=(**iterp2).isEnable();
							(**iterp2).bActivation = FALSE;
							(**iterp2).bisAssigned = TRUE;

							UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc);

							(**iterp2).Invalidate();                                                            // 库所使能后，立刻改变颜色
							(**iterp2).UpdateWindow();
							Sleep(t_interval);
						}
					}

					// 当有库所使能后，即跳转到判断是否有变迁满足发生条件
					goto _GET_TRANSFIRED_;
				}
				else
				{

				//==7.判断PActived中是否只剩时延库所，如果是，继续执行，否则执行8
					if(n_isallTime == PActived.size())                                                         // 如果仅剩余时延库所
					{
						
						// 首先，找到PActived中时延最短的库所，另其使能，并从PActived中删除；
						double timeTS        = PActived.at(0)->dTime;                                          // 用于记录库所的时间
						int    mintime_posn  = 0;                                                              // 用于记录时延最短库所的位置
						int    n_count2      = 0;                                                              // 用于记录库所的位置
						for(vector<CPlace*>::iterator iterp4=PActived.begin();iterp4!=PActived.end();iterp4++) // 找到时延最短的时延库所
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
						PActived.at(mintime_posn)->bEnable       = PActived.at(mintime_posn)->isEnable();      // 最短时延库所使能
						PActived.at(mintime_posn)->bActivation   = FALSE;
						PActived.at(mintime_posn)->bisAssigned   = TRUE;

						PActived.at(mintime_posn)->Invalidate();                                               // 库所使能后，立刻改变颜色
						PActived.at(mintime_posn)->UpdateWindow();
						Sleep(t_interval);

						// 当时间库所使能后，将库所的时间阈值设置会原来的值 
						PActived.at(mintime_posn)->dTime         = timeTS;
						
						// 将使能的时延库所从PActived中擦除，其他时延库所的时延阈值依次减去timeTS
						vector<CPlace*>::iterator iterp5=PActived.begin();
						PActived.erase(iterp5+mintime_posn);                                                   // 从PActived中删除该库所
						for(vector<CPlace*>::iterator iterp6=PActived.begin();iterp6!=PActived.end();iterp6++)
						{
							(**iterp6).dTime = (**iterp6).dTime - timeTS;
						}

						// 当有库所使能后，即跳转到判断是否有变迁满足发生条件
						goto _GET_TRANSFIRED_;	
					}
					else  
					{

					//==8.如果剩余库所不仅有时延库所，还有连续库所（或只有连续库所），则PActived中所有库所开始单步演化
	_SINGLE_STEP_GO_:
						// PActived中库所依次按步长演化
						for(vector<CPlace*>::iterator iterp7=PActived.begin();iterp7!=PActived.end();iterp7++) 
						{

							// 当库所为时延库所
							if((**iterp7).PlaceStyle == STYLE_TIME)
							{
								(**iterp7).dTime = (**iterp7).dTime - Time_Step;
								(**iterp7).bEnable = (**iterp7).isEnable();

								//UpdateOutputModel(vector_place,vector_outputmodel);
							}

							// 当库所为连续库所或决策库所
							if((**iterp7).PlaceStyle == STYLE_CNTNUS || (**iterp7).PlaceStyle == STYLE_DISIC)
							{
								(**iterp7).bEnable = (**iterp7).isEnable(Time_Step);

								UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc);
							}
						}

						// PActived中库所依次演化一个步长后，判断是否有满足使能条件的库所
						int          n_count3 = 0;                                                             // 仅仅起到一个计数器的作用,表示每个库所在 PActived 中的位置
						vector<int>  vec_posn_P2;                                                              // 记录使能库所的位置

						for(vector<CPlace*>::iterator iterp8=PActived.begin();iterp8!=PActived.end();iterp8++)
						{
							if((**iterp8).bEnable)
							{
								vec_posn_P2.push_back(n_count3);

								(**iterp8).bActivation  = FALSE;
								(**iterp8).bisAssigned  = TRUE;

								(**iterp8).Invalidate();                                                       // 库所使能后，立刻改变库所颜色
								(**iterp8).UpdateWindow();
								Sleep(t_interval);

								
								if((**iterp8).PlaceStyle==STYLE_TIME)                                          // 如果使能的库所为时延库所，使能后，恢复其时延阈值为初始值
								{
									int ntimep=0;//计数器的作用，表示时延库所在库所集中的位置
									for(vector<CPlace*>::iterator itertime_p  = vector_place.begin();
										                          itertime_p != vector_place.end();  itertime_p++) //遍历库所，找到该时延库所在库所集中的位置
									{
										if((**iterp8).u_SubscriptofModel == (**itertime_p).u_SubscriptofModel)
										{
											(**iterp8).dTime=arrytimets.at(ntimep);
										}
										ntimep++;
									}
								}
								//-----此处，如果该库所为连续库所，那么在连续库所执行之前，更新连续库所的值为初始状态+++++++++++++++++++++++5.7
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

						// 如果存在满足使能的库所（即，vec_posn_P2.size()！=0），则跳转到判断是否有变迁满足发生条件，否则，继续演化下一个步长
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

				//==10.扫描Petri网，找到满足发生条件的变迁集合TFired，若不为空，TFired中变迁依次发生，并转向执行4
_GET_TRANSFIRED_:

				PActived.clear();

				// 先进行值的更新
				UpdateIOValueofModel(vector_place,m_pdoc);


				for(vector<CTransition*>::iterator itert1=vector_trans.begin();itert1!=vector_trans.end();itert1++)
				{
					(**itert1).bFire = (**itert1).isFire();                                                        // 满足发生条件的变迁，发生标识置true

					if((**itert1).bFire)
					{
						(**itert1).Invalidate();                                                                   // 变迁发生后，立刻改变颜色
						(**itert1).UpdateWindow();
						Sleep(t_interval);
						TFired.push_back(*itert1);

						//遍历变迁的所有前置库所
						for(vector<CPlace*>::iterator iter = (**itert1).arryTransPre.begin();iter!=(**itert1).arryTransPre.end();++iter)
						{
							(**iter).bEnable = false;
							(**iter).Invalidate();
							(**iter).UpdateWindow();
						}

						//遍历变迁的所有后置库所
						for(vector<CPlace*>::iterator iter1 = (**itert1).arryTransPst.begin();iter1!=(**itert1).arryTransPst.end();++iter1)
						{
							(**iter1).bActivation = true;
							(**iter1).bisAssigned = false;
						}

						(**itert1).bFire = FALSE;
						(**itert1).Invalidate();                                                                 // 变迁发生完成后，立刻改变颜色
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
	//==1.获取输入仿真线程的参数
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
	
//	m_pmainframe->m_wndStatusBar.EnablePaneProgressBar(1,Sim_Time,true);// 设置仿真进度的最大范围
	//int t_interval = 10;
	// 每次点击开始仿真按钮后，先将所有输出模块中的表格的行全部清除

	for(vector<CCTRLShowMatrix*>::iterator iter=vector_outputmodel.begin();iter!=vector_outputmodel.end();iter++)
	{
		if((*iter)->m_pSMat_OutValue != NULL)
		{
			(*iter)->m_pSMat_OutValue->m_Chart_SMatrix.DeleteAllItems();
		}
	}
	// 每次点击开始仿真按钮后，先将所有输出模块中的csv文件调用open方法打开
	OpenAllDataFile(vector_outputmodel);

	//==2.初始化一些变量
	bool                  b_finishFlag = false;                                                              // 定义一个bool变量，当Petri网推演完成，即为TRUE,否则为FALSE。
	vector<CPlace*>       PActived;                                                                          // 定义激活库所集合 PActived
	vector<CTransition*>  TFired;                                                                            // 定义使能变迁集合 TFired
	double                SimClock     = 0;                                                                  // 定义时间中  
	

	PassByValue(m_pdoc);
    //==3.开始推演算法
	do{
		InitPlaceLocalData(vector_place, m_pdoc);//+++++12.18

_CHECK_SIMTIME_:
		//PassByValue(m_pdoc);
		if(WaitForSingleObject(m_stopsimevent->m_hObject,(DWORD)0.001)!=WAIT_OBJECT_0)                       // 通过停止按钮，控制线程是否退出
		{
			m_pmainframe->SendMessage(WM_MESSAGE_CHANGEPROGRESS,SimClock*10000,0);// 将当前仿真进度设置到进度条
			if(SimClock < Sim_Time)                                                                          // 判断是否达到仿真时间
			{
				int TimACntus_n = 0;                                                                             // 记录PActived集合中时延库所和连续库所的个数
				//==4.获取HSTPN模型中处于集合状态的库所，保存入PActived
				for(vector<CPlace*>::iterator iterp1=vector_place.begin();iterp1!=vector_place.end();iterp1++)
				{
					// 将处于激活状态的库所保存入PActived，同时记录质变类库所和时延库所的个数
					if((**iterp1).bActivation && !(**iterp1).bisAssigned)                                           // 同时要保证库所是激活状态，但没有经过状态演化过程
					{
						PActived.push_back(*iterp1);
						if((*iterp1)->PlaceStyle==STYLE_CNTNUS || (*iterp1)->PlaceStyle==STYLE_TIME || (*iterp1)->PlaceStyle == STYLE_DISIC)
						{
							TimACntus_n++;
						}
					}
				}

				// 激活库所表现为空心token
				for(vector<CPlace*>::iterator iterp2=PActived.begin();iterp2!=PActived.end();iterp2++)
				{
					(*iterp2)->Invalidate();
					(*iterp2)->UpdateWindow();
				}
				//==5.判断PActived是否为空，若为空，则结束Petri网仿真， 若不为空，则继续执行
				int n_PActived1 = PActived.size();                                                                  // 获取PActived集合的大小
				if(n_PActived1 == 0) 
				{
					CloseAllDataFile(vector_outputmodel);
					b_finishFlag = TRUE;
					return 0;                                                                                       // return 0，即退出线程函数
				}
				else                 
				{
					Sleep(t_interval);
					//==6.时间钟推进一个步长，PActived内的所有库所演化一个步长,并为所有的输出模块赋值
					// 时间钟推进一个步长
					/*if(TimACntus_n > 0)
					{
						SimClock = SimClock + Time_Step;
					}*/
					SimClock = SimClock + Time_Step;
					
					// PActived内的所有库所演化一个步长
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
					
					// PActived内的所有库所演化一个步长
					/*if(TimACntus_n > 0)
					{
						UpdateOutputModel(vector_place,vector_outputmodel );
					}*/
					UpdateOutputModel(vector_place,vector_outputmodel,m_pdoc );

					//==7.判断PActived内是否有使能的库所，如果有，使能库所的token从空心变为实心，如果没有，返回判断是否到达仿真时间
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
						// 返回检测是否到达仿真时间
					//	PActived.clear();//++++++++8.09

						UpdateIOValueofModel(vector_place,m_pdoc,PActived);//++++++++++++8.09  // 每个步长更新局部和全局变量表的值的函数写的不对
						PActived.clear();// 先更新再清空
						goto _CHECK_SIMTIME_;
					}

					//==8.PActived集合清空，并更新库所的输入输出表
				//	PActived.clear();

					UpdateIOValueofModel(vector_place,m_pdoc,PActived);
					PActived.clear();//
					//==9.扫描Petri网，找到使能变迁集合TFired
					for(vector<CTransition*>::iterator itert1=vector_trans.begin();itert1!=vector_trans.end();itert1++)
					{
						(**itert1).bFire = (**itert1).isFire();    // 满足发生条件的变迁，发生标识置true

						if((**itert1).bFire)
						{
							TFired.push_back(*itert1);

							//遍历变迁的所有前置库所
							for(vector<CPlace*>::iterator iter = (*itert1)->arryTransPre.begin();iter!=(*itert1)->arryTransPre.end();++iter)
							{
								//多任务决策改进库所
								if ((**iter).PlaceStyle == STYLE_DISIC&& (**iter).count){
									(**iter).count--;
									(**iter).bEnable = true;
								}
								else
								(**iter).bEnable = false;
							}

							//遍历变迁的所有后置库所
							for(vector<CPlace*>::iterator iter1 = (*itert1)->arryTransPst.begin();iter1!=(*itert1)->arryTransPst.end();++iter1)
							{
								(**iter1).bActivation = true;
								(**iter1).bisAssigned = false;
							}
							InitPlaceLocalData((*itert1)->arryTransPst, m_pdoc);//+++++12.18
						}
					}
					//==10.判断TFired集合是否为空，如果为空，跳转到判断是否到达仿真时间，如果不为空，变迁依次发生，更新前置和后置库所，token从无到空心
					if(0 == TFired.size())
					{
						b_finishFlag = false;
					}
					else
					{
						for(vector<CTransition*>::iterator itert2=TFired.begin();itert2!=TFired.end();itert2++)
						{
							// 前置token消失
							for(vector<CPlace*>::iterator iter = (*itert2)->arryTransPre.begin();iter!=(*itert2)->arryTransPre.end();iter++)
							{
								(*iter)->Invalidate();
								(*iter)->UpdateWindow();
							}
							// 变迁变绿色
							(*itert2)->Invalidate();
							(*itert2)->UpdateWindow();
						}
						Sleep(t_interval);

						for(vector<CTransition*>::iterator itert3=TFired.begin();itert3!=TFired.end();itert3++)
						{
							// 变迁变回白色
							//TFired中OnPaint中改变
							(*itert3)->bFire = false;
							(*itert3)->Invalidate();
							(*itert3)->UpdateWindow();
						}
						//Sleep(t_interval);
						TFired.clear();

						// 返回检测是否到达仿真时间
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

// 库所的输入/输出值的更新函数
void CHSTPN_SYSSim::UpdateIOValueofModel(vector<CPlace*> vector_place ,CHSTPN_SYSDoc * m_pdoc,vector<CPlace*> PActived)
{
	// 输入输出值的更新分两步：1）更新输入输出列表；2）为时间、概率、控制等需要赋值的变量赋值
	// ---------------------------------------------------更新输入输出列表-------------------------------Begin------------------------
//	for(vector<CPlace*>::iterator iterp1 = vector_place.begin();iterp1!=vector_place.end();iterp1++)// 遍历所有的库所--先更新输出列表
	for(vector<CPlace*>::iterator iterp1 = PActived.begin();iterp1!=PActived.end();++iterp1)
	{
		for(vector<IOValue*>::iterator iterout1 = (*iterp1)->arryIOputData.begin();iterout1 != (*iterp1)->arryIOputData.end();iterout1++)// 遍历库所的变量列表--所有输出值，如果需要赋值，则赋值
		{
			if((*iterout1)->IOType == "output" || (*iterout1)->IOType=="in/output")
			{
				if((*iterout1)->FromModel == "激活标识")
				{
					(*iterout1)->Value = (*iterp1)->bActivation;
				}
				if((*iterout1)->FromModel == "激活标识（取反）")
				{
					(*iterout1)->Value = !(*iterp1)->bActivation;
				}
				if((*iterout1)->FromModel == "使能标识")
				{
					(*iterout1)->Value = (*iterp1)->bEnable;
				}
				if((*iterout1)->FromModel == "使能标识（取反）")
				{
					(*iterout1)->Value = !(*iterp1)->bEnable;
				}
				if((*iterout1)->FromModel == "概率阈值")
				{
					(*iterout1)->Value = (*iterp1)->dProblty;
				}
				if((*iterout1)->FromModel == "时间阈值")
				{
					(*iterout1)->Value = (*iterp1)->d_TimeTS;//dTime;
				}
				if((*iterout1)->FromModel == "随机结果")
				{
					(*iterout1)->Value = (*iterp1)->i_RandOutcome;
				}
				if((*iterout1)->FromModel == "随机结果（取反）")
				{
					(*iterout1)->Value = !(*iterp1)->i_RandOutcome;
				}
				if((*iterout1)->FromModel == "随机概率值")
				{
					(*iterout1)->Value = (*iterp1)->d_RandProblty;
				}
			}
			// 如果该输出变量引用了全局变量
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
	//注意，输入变量赋值要在源库所的bisAssigned标识为真的情况下进行
	for(vector<CPlace*>::iterator iterp2 = vector_place.begin();iterp2!=vector_place.end();iterp2++)// 遍历所有的库所--再更新输入列表
	{

		for(vector<IOValue*>::iterator iterin1 = (*iterp2)->arryIOputData.begin();iterin1 != (*iterp2)->arryIOputData.end();iterin1++)//遍历库所的输入变量列表，如果需要赋值，则赋值
		{
			if((*iterin1)->IOType == "input" | (*iterin1)->IOType=="in/output")//++5.27
			{
				for(vector<CPlace*>::iterator iterp3 = vector_place.begin();iterp3!=vector_place.end();iterp3++)// 在次遍历库所，找到与输入变量数据源相同的库所对象
				{
					if((*iterin1)->FromModel == (*iterp3)->m_caption && (*iterp3)->bisAssigned) //找到数据源库所后，再获取其输出列表，并找到其中与“数据源中变量”相同变量名的变量
					{
						for(vector<IOValue*>::iterator iterout2 = (*iterp3)->arryIOputData.begin();iterout2 != (*iterp3)->arryIOputData.end();iterout2++)// 遍历数据源的输出变量表
						{
							if((*iterout2)->IOType == "output" | (*iterout2)->IOType=="in/output")//++5.27
							{
								if((*iterout2)->Name == (*iterin1)->FromMDVluName)
								{
									(*iterin1)->Value = (*iterout2)->Value;
									// 如果引用了全局变量
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
	// ---------------------------------------------------更新输入输出列表-------------------------------End--------------------------
	//----------------------------------------------------为时间、概率、控制等需要赋值的变量赋值---------Begin------------------------
	for(vector<CPlace*>::iterator iterp3 = vector_place.begin();iterp3!=vector_place.end();iterp3++)// 遍历库所，根据库所中的bisUserDef_prob、bisUserDef_time、bisUserDef_ctrl变量判断是否需要赋值，如果值为false，则表示用户选择了“来自输入值”，则需要赋值
	{
		if(!(*iterp3)->bisUserDef_prob)  // 如果概率阈值需要获取
		{
			CString str_namevalue = (*iterp3)->str_Inputport_prob; //获取概率阈值，数据源的名字（变量name）
			for(vector<IOValue*>::iterator iterin2 = (*iterp3)->arryIOputData.begin();iterin2 != (*iterp3)->arryIOputData.end();iterin2++)// 遍历库所的输入变量表，找到该变量，完成赋值
			{
				if((*iterin2)->IOType == "input" | (*iterin2)->IOType=="in/output")//++5.27
				{
					if((*iterin2)->Name == str_namevalue)
					{
						for(vector<CPlace*>::iterator iterp4 = vector_place.begin();iterp4 != vector_place.end();iterp4++)
						{
							if((*iterp4)->m_caption == (*iterin2)->FromModel && (*iterp4)->bisAssigned)// 找到源库所,同时要保证源库所的bisAssigned标识为真
							{
								(*iterp3)->dProblty = (*iterin2)->Value;
							}
						}			
					}
				}
			}

		}
		if(!(*iterp3)->bisUserDef_time) //如果时间阈值需要获取
		{
			CString str_namevalue = (**iterp3).str_Inputport_time; //获取时间阈值，数据源的名字（变量name）
			for(vector<IOValue*>::iterator iterin2 = (*iterp3)->arryIOputData.begin();iterin2 != (*iterp3)->arryIOputData.end();iterin2++)// 遍历库所的输入变量表，找到该变量，完成赋值
			{
				if((*iterin2)->IOType == "input" | (*iterin2)->IOType=="in/output")//++5.27
				{
					if((*iterin2)->Name == str_namevalue)
					{
						for(vector<CPlace*>::iterator iterp4 = vector_place.begin();iterp4 != vector_place.end();iterp4++)
						{
							if((*iterp4)->m_caption == (*iterin2)->FromModel && (*iterp4)->bisAssigned)// 找到源库所,同时要保证源库所的bisAssigned标识为真
							{
								(*iterp3)->dTime = (*iterin2)->Value;
								(*iterp3)->d_TimeTS = (*iterp3)->dTime;
							}
						}
					
					}
				}
			}
		}
		if(!(*iterp3)->bisUserDef_ctrl) //如果控制库所的激活值需要获取
		{
			CString str_namevalue = (*iterp3)->str_Inputport_ctrl; //获取控制库所的激活值，数据源的名字（变量name）
			for(vector<IOValue*>::iterator iterin2 = (*iterp3)->arryIOputData.begin();iterin2 != (*iterp3)->arryIOputData.end();iterin2++)// 遍历库所的输入变量表，找到该变量，完成赋值
			{
				if((**iterin2).IOType == "input" | (*iterin2)->IOType=="in/output")//++5.27
				{
					if((**iterin2).Name == str_namevalue)
					{
						for(vector<CPlace*>::iterator iterp4 = vector_place.begin();iterp4 != vector_place.end();iterp4++)
						{
							if((*iterp4)->m_caption == (*iterin2)->FromModel && (*iterp4)->bisAssigned)// 找到源库所,同时要保证源库所的bisAssigned标识为真
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





	// 在值更新完成后，将所有库所的bisAssigned标识再次设置为false
	for(vector<CPlace*>::iterator iterp3 = vector_place.begin();iterp3!=vector_place.end();iterp3++)
	{
		(**iterp3).bisAssigned = FALSE;
	}
}

// 孤立点检查算法
vector<CString> CHSTPN_SYSSim::IsLtdNodeDetect(vector<CPlace*> vector_place,vector<CTransition*> vector_trans)
{
	
	vector<CString> IsLtdNodeSet;
	IsLtdNodeSet.clear();
	// --遍历所有库所
	for(vector<CPlace*>::iterator iterp=vector_place.begin();iterp!=vector_place.end();iterp++)
	{
		if((*iterp)->arryPlacePre.size()==0 && (*iterp)->arryPlacePst.size()==0)// 找到前置和后置均为0的库所
		{
			IsLtdNodeSet.push_back((*iterp)->m_caption);
		}
	}
	// --遍历所有变迁
	for(vector<CTransition*>::iterator itert=vector_trans.begin();itert!=vector_trans.end();itert++)
	{
		if((*itert)->arryTransPre.size()==0 && (*itert)->arryTransPst.size()==0)// 找到前置和后置均为0的变迁
		{
			IsLtdNodeSet.push_back((*itert)->m_caption);
		}
	}
	return IsLtdNodeSet;
}

// 模型检查算法
UINT CHSTPN_SYSSim::ModelCheck(LPVOID m_modelcheckdata)
{
	// 获取传递的参数
	ModelCheckData * modelcheckdata = (ModelCheckData*)m_modelcheckdata;
	HSTPN_DATA * hstpndata = (HSTPN_DATA*)(modelcheckdata->m_hstpnData);
	vector<CPlace*> vector_place = hstpndata->vector_place;
	vector<CTransition*> vector_trans = hstpndata->vector_trans;
	BOOL m_btnIsLtdNode = modelcheckdata->m_btnIsLtdNode;
	BOOL m_btnConflict = modelcheckdata->m_btnConflict;
	BOOL m_btnContact = modelcheckdata->m_btnContact;
	BOOL m_btnDeadlock = modelcheckdata->m_btnDeadlock;
	BOOL m_btnTrap = modelcheckdata->m_btnTrap;
	// 定义存储结构的集合；
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

// 纯网检测算法
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

// 关联矩阵求解
UINT CHSTPN_SYSSim::IncdnceMatCompute(LPVOID psimnet)
{
	HSTPN_DATA * hstpndata = (HSTPN_DATA*)psimnet;
	vector<CPlace*> vector_place = hstpndata->vector_place;
	vector<CTransition*> vector_trans = hstpndata->vector_trans;
	//if()
	vector<CString> arryltdnode = IsLtdNodeDetect(vector_place,vector_trans);//First ,Check Isolated node
	if(arryltdnode.size()!=0)
	{
		AfxMessageBox(L"存在孤立节点，请先进行孤立节点的检测！");
	}
	else
	{
		bool b_ispurenet = isPureNet(vector_place,vector_trans);// Second, check the net that it is a pure net or not.
		if(!b_ispurenet)// if it is not a pure net ,pop-up Message Box , show that it is not a pure net ,can not calculate the incidence matrix.
		{
			AfxMessageBox(L"Petri网模型不是纯网，不能求解关联矩阵！");
		}
		else
		{
			vector<CString> IncdceMat_S_T;// 定义局部变量，用于保存关联矩阵，关联矩阵在此vector中按行保存
			IncdceMat_S_T.clear();

			int m = vector_place.size();//获取库所和变迁的个数，用于定义矩阵的大小
			int n = vector_trans.size();
			vector<int> IncdnceMat_in(n*m);// 定义输入矩阵
			vector<int> IncdnceMat_out(n*m);// 定义输出矩阵

			int i = 1; // 用i表示关联矩阵的列，从1开始
			for(vector<CPlace*>::iterator iter1=vector_place.begin();iter1!=vector_place.end();iter1++)
			{
				// 计算输入矩阵
				for(vector<CTransition*>::iterator iter2=(*iter1)->arryPlacePre.begin();iter2!=(*iter1)->arryPlacePre.end();iter2++)
				{
					int j = 1;// j表示输入矩阵的行
					for(vector<CTransition*>::iterator itert1=vector_trans.begin();itert1!=vector_trans.end();itert1++)// 找到变迁在变迁集中的位置
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
				// 计算输出矩阵
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
			// 做差计算关联矩阵
			int l=0;
			for(vector<int>::iterator iter=IncdnceMat_in.begin();iter!=IncdnceMat_in.end();iter++)
			{
				CString str;
				str.Format(L"%d",IncdnceMat_in[l]-IncdnceMat_out[l]);
				IncdceMat_S_T.push_back(str);
				l++;
			}

			int sizeofP = vector_place.size();// 获取库所和变迁的数量，传递给分析日志窗口
			int sizeofT = vector_trans.size();

			CWndShowMatrix m_wndshowmatrix(IncdceMat_S_T,sizeofP,sizeofT,2,NULL);;
			m_wndshowmatrix.DoModal();
		}
	}
	return 0;
}

// OutputModel模块的输出值赋值函数
void CHSTPN_SYSSim::UpdateOutputModel(vector<CPlace*> vector_place,vector<CCTRLShowMatrix*> vector_outputmodel,CHSTPN_SYSDoc * m_pDoc )
{
	for(vector<CCTRLShowMatrix*>::iterator itero=vector_outputmodel.begin();itero!=vector_outputmodel.end();itero++)
	{
		//if((*itero)->OutputStyle == OUT_TOFILE)// 如果是OUT_TOFILE类型的输出模块，则先为其定义一个list，用于存放每行数字
		//{
			list<string> str_valuerow;
		//}

		int subofOutData = 0;
		for(vector<int>::iterator iter=(*itero)->vec_OutValue.begin();iter!=(*itero)->vec_OutValue.end();iter++)
		{
			if((*itero)->OutputStyle == OUT_SHOWMATRIX)//如果是OUT_SHOWMATRIX模块，则将变量依次保存到vec_OutputData中
			{
				double *outvalue = new double();
				if((*itero)->OutPName == "全局变量") // 如果输出的是全局变量
				{
					(*outvalue) = m_pDoc->arryIOputDataG.at(*iter)->Value;
				}
				else // 如果输出的是库所中的局部变量
				{
					(*outvalue) = vector_place.at((*itero)->i_PposnOValue)->arryIOputData.at(*iter)->Value; //全局运行到此处存在bug 
				}
				(*itero)->vec_OutputData.at(subofOutData)->push_back(outvalue);
			}
			else                                      // 如果是OUT_TOFILE模块，则先将一行的变量数值保存到list中，在调用WriteDatatoFile写入文件
			{
				//CString str;
				//str.Format(_T("%d"),vector_place.at((*itero)->i_PposnOValue)->arryIOputData.at(*iter)->Value);
				ostringstream os_value;
				if((*itero)->OutPName == "全局变量")
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

// Output-OUT_TOFILE写入文件操作
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

// 关闭OUT_TOFILE模块所有的数据文件
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

// 打开OUT_TOFILE模块中所有的datafile
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

// 当库所从“未激活”状态转换到“激活（未使能）”状态时，检测库所中的局部变量是否引用了全局变量，如果引用了，则由全局变量表首先为局部变量表赋值；
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
					if((*iter_io)->isQuoteG) // 若变量引用了全局，则变量全局表
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
	//去除重复变量
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
	
	//模拟数据
	//如果基地生命值为0
	if (mapMoveDataTmp["health_bjx"] == 0 && mapMoveDataTmp["health_dlbl"] == 0) {
		return;
	}
	//第一波战斗
	//mapMoveDataTmp["plane_hzj_bjx"] = 12;
	//mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	//mapMoveDataTmp["plane_hzj_yc"] = 0;
	//mapMoveDataTmp["plane_dzj_bjx"] = 6;
	//mapMoveDataTmp["plane_dzj_dlbl"] = 6;
	//mapMoveDataTmp["plane_dzj_yc"] = 6;
	//mapMoveDataTmp["plane_gjj_bjx"] = 8;
	//mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	//mapMoveDataTmp["plane_gjj_yc"] = 0;
	////雷达生命值
	//mapMoveDataTmp["health_radar_bjx"] = 100;
	//mapMoveDataTmp["health_radar_dlbl"] = 100;
	//mapMoveDataTmp["health_radar_yc"] = 100;
	////战机数量
	//mapMoveDataTmp["plane_hzj"] = 36;
	//mapMoveDataTmp["plane_gjj"] = 20;
	//mapMoveDataTmp["plane_dzj"] = 12;
	////导弹数量
	//mapMoveDataTmp["missile_bjx"] = 100;
	//mapMoveDataTmp["missile_dlbl"] = 100;
	//mapMoveDataTmp["missile_yc"] = 100;
	////基地生命值
	//mapMoveDataTmp["health_bjx"] = 100;
	//mapMoveDataTmp["health_dlbl"] = 100;
	//mapMoveData.push_back(mapMoveDataTmp);
	//第二波战斗

	mapMoveDataTmp["plane_hzj_bjx"] = 12;
	mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	mapMoveDataTmp["plane_hzj_yc"] = 0;
	mapMoveDataTmp["plane_dzj_bjx"] = 0;
	mapMoveDataTmp["plane_dzj_dlbl"] = 0;
	mapMoveDataTmp["plane_dzj_yc"] = 0;
	mapMoveDataTmp["plane_gjj_bjx"] = 8;
	mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	mapMoveDataTmp["plane_gjj_yc"] = 0;
	//雷达生命值
	mapMoveDataTmp["health_radar_bjx"] = mapMoveData[0]["health_radar_bjx"] - 25;
	mapMoveDataTmp["health_radar_dlbl"] = mapMoveData[0]["health_radar_dlbl"]-25;
	mapMoveDataTmp["health_radar_yc"] = mapMoveData[0]["health_radar_yc"];
	//战机数量
	mapMoveDataTmp["plane_hzj"] = mapMoveData[0]["plane_hzj"]-6;
	mapMoveDataTmp["plane_gjj"] = mapMoveData[0]["plane_gjj"]-6;
	mapMoveDataTmp["plane_dzj"] = mapMoveData[0]["plane_dzj"]-6;
	//导弹数量
	mapMoveDataTmp["missile_bjx"] = mapMoveData[0]["missile_bjx"] - 30;
	mapMoveDataTmp["missile_dlbl"] = mapMoveData[0]["missile_dlbl"] - 30;
	mapMoveDataTmp["missile_yc"] = mapMoveData[0]["missile_yc"] - 30;
	//基地生命值
	mapMoveDataTmp["health_bjx"] = mapMoveData[0]["health_bjx"] - 45;
	mapMoveDataTmp["health_dlbl"] = mapMoveData[0]["health_dlbl"] - 45;
	mapMoveData.push_back(mapMoveDataTmp);
	//第三波战斗
	mapMoveDataTmp["plane_hzj_bjx"] = 12;
	mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	mapMoveDataTmp["plane_hzj_yc"] = 0;
	mapMoveDataTmp["plane_dzj_bjx"] = 0;
	mapMoveDataTmp["plane_dzj_dlbl"] = 0;
	mapMoveDataTmp["plane_dzj_yc"] = 0;
	mapMoveDataTmp["plane_gjj_bjx"] = 0;
	mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	mapMoveDataTmp["plane_gjj_yc"] = 0;
	//雷达生命值
	mapMoveDataTmp["health_radar_bjx"] = mapMoveData[1]["health_radar_bjx"] - 25;
	mapMoveDataTmp["health_radar_dlbl"] = mapMoveData[1]["health_radar_dlbl"] - 25;
	mapMoveDataTmp["health_radar_yc"] = mapMoveData[1]["health_radar_yc"];
	//战机数量
	mapMoveDataTmp["plane_hzj"] = mapMoveData[1]["plane_hzj"] - 6;
	mapMoveDataTmp["plane_gjj"] = mapMoveData[1]["plane_gjj"] - 6;
	mapMoveDataTmp["plane_dzj"] = mapMoveData[0]["plane_hzj"] ;
	//导弹数量
	mapMoveDataTmp["missile_bjx"] = mapMoveData[1]["missile_bjx"] - 30;
	mapMoveDataTmp["missile_dlbl"] = mapMoveData[1]["missile_dlbl"] - 30;
	mapMoveDataTmp["missile_yc"] = mapMoveData[1]["missile_yc"] - 30;
	//基地生命值
	mapMoveDataTmp["health_bjx"] = mapMoveData[1]["health_bjx"] - 45;
	mapMoveDataTmp["health_dlbl"] = mapMoveData[1]["health_dlbl"] - 45;
	mapMoveData.push_back(mapMoveDataTmp);
}

