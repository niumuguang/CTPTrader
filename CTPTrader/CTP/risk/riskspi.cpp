#include "string.h"
#include "math.h"
#include "windows.h"
#include "api/risk/public/FtdcRiskUserApi.h"
#include "risk/riskspi.h"
#include "win_event.h"

#include <iostream>
#include <vector>
using namespace std;

extern TShfeFtdcBrokerIDType appId;		// 经纪公司代码
extern TShfeFtdcUserIDType userId;		// 操作员

extern int requestId;
extern HANDLE g_hEvent;

vector<CShfeFtdcSequencialPositionField*> posList;
vector<CShfeFtdcRiskSyncAccountField* > capList;
//连接成功
void CtpRiskSpi::OnFrontConnected()
{
	cerr<<" 连接交易前置...成功"<<endl;
  SetEvent(g_hEvent);
}

//请求登录
void CtpRiskSpi::ReqUserLogin(  TShfeFtdcBrokerIDType	vAppId,
      TShfeFtdcUserIDType	vUserId,  TShfeFtdcPasswordType	vPasswd)
{
	CShfeFtdcReqRiskUserLoginField req;
  memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, vAppId); strcpy(appId, vAppId); 
	strcpy(req.UserID,   vUserId);strcpy(userId,vUserId);	
	strcpy(req.Password, vPasswd);
	req.Version=2;  
	int ret=pUserApi->ReqRiskUserLogin(&req,++requestId);	
  cerr<<" 请求 | 发送登录..."<<((ret == 0) ? "成功" :"失败") << endl;	
}

///请求登录响应
void CtpRiskSpi:: OnRspRiskUserLogin(
      CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, 
      CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{	 
  if( !IsErrorRspInfo(pRspInfo) && pRspRiskUserLogin){
    cerr<<" 响应 | 登录成功...当前交易日:"
      <<pRspRiskUserLogin->TradingDay<<endl;
  }
  if(bIsLast) SetEvent(g_hEvent);
}

///客户端结算组数据同步开始
//将同步: 时间,产品信息,合约信息,投资者组, 随后自动返回
void CtpRiskSpi:: OnRtnClientSGDataSyncStart(
       CShfeFtdcSettlementSessionField *pSettlementSession) {	}
///时间同步
void CtpRiskSpi::OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime){}
///交易所信息
void CtpRiskSpi::OnRtnExchange(CShfeFtdcExchangeField *pExchange){ }
///产品信息
void CtpRiskSpi::OnRtnProduct(CShfeFtdcProductField *pProduct) { }
///合约信息
void CtpRiskSpi::OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument){}
///投资者组
void CtpRiskSpi::OnRtnInvestorGroup(
            CShfeFtdcInvestorGroupField *pInvestorGroup){ }
//组织架构名称
void CtpRiskSpi::OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment){}
///客户端结算组数据同步结束
void CtpRiskSpi:: OnRtnClientSGDataSyncEnd(
         CShfeFtdcSettlementSessionField *pSettlementSession) 
{
  cerr<<" 回报 | 终端同步数据...完成"<<endl;  
	ReqUserData();	
  SetEvent(g_hEvent);
}

//查询或订阅自己所需要的数据
void CtpRiskSpi::ReqUserData()
{
  //查询投资者信息
	CShfeFtdcInvestorIDRangeField reqField={0};
	pUserApi->ReqQryInvestorLinkMan(&reqField,1,++requestId);	

	//查询投资者组织架构信息	
	pUserApi->ReqQryInvestorLinkMan(&reqField,1,++requestId);	

	//订阅报单信息	 
  //注:
  //   如果自己对已收到的数据进行了文件保存,则再次登陆时,
  //    pNotifySequence.SequenceNo可填最后收到的
  //    CShfeFtdcSequencialOrderField::UniqSequenceNo+1;
  //   以减少数据传输量;余皆类似
	CShfeFtdcNotifySequenceField pNotifySequence={0};			
	pNotifySequence.SequenceNo=0;							   
	pUserApi->ReqSubscribeOrder(&pNotifySequence,++requestId);
															   
	//订阅成交信息	
	pNotifySequence.SequenceNo=0;
	pUserApi->ReqSubscribeTrade(&pNotifySequence,++requestId);

	//订阅持仓信息	
	pNotifySequence.SequenceNo=0;
	pUserApi->ReqSubscribePosition(&pNotifySequence,++requestId);

	//订阅用户事件	
	pNotifySequence.SequenceNo=0;
	pUserApi->ReqSubBrokerUserEvent(&pNotifySequence,++requestId);

	//订阅预埋单
	pNotifySequence.SequenceNo=0;
	pUserApi->ReqSubRiskParkedOrder(&pNotifySequence,++requestId);


	//订阅昨资金
	CShfeFtdcRiskNtfSequenceField pNotifySequence1={0};		
	pNotifySequence1.SequenceNo=0;
	pUserApi->ReqSubPreRiskAccount(&pNotifySequence1,++requestId);

	//订阅出入金
	pNotifySequence1.SequenceNo=0;
	pUserApi->ReqSubSeqDeposit(&pNotifySequence1,++requestId);

	//订阅交易编码 	
	pNotifySequence1.SequenceNo=0;
	pUserApi->ReqSubSeqTradingCode(&pNotifySequence1,++requestId);

	//订阅风险通知信息
	pNotifySequence1.SequenceNo=0;
	pUserApi->ReqSubSeqRiskNotifyB(&pNotifySequence1,++requestId);

	//订阅业务通知
	pNotifySequence1.SequenceNo=0;
	strcpy(pNotifySequence1.DataType,"BizNotice");
	pUserApi->ReqSubSeqData(&pNotifySequence1,++requestId);


}
///查询投资者信息应答,对应ReqQryInvestorLinkMan请求
void CtpRiskSpi::OnRspQryInvestorLinkMan(
    CShfeFtdcInvestorLinkManField *pInvestorLinkMan, 
    CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){ }

///查询投资者组织架构应答,对应ReqQryInvestorLinkMan请求
void CtpRiskSpi::OnRspQryInvestorDepartment(
    CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, 
    CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){ }

///有序的报单回报,对应ReqSubscribeOrder订阅
void CtpRiskSpi::OnRtnSequencialOrder(
    CShfeFtdcSequencialOrderField *pSequencialOrder){}

///有序的成交回报,对应ReqSubscribeTrade订阅
void CtpRiskSpi::OnRtnSequencialTrade(
    CShfeFtdcSequencialTradeField *pSequencialTrade){}

///有序的持仓回报,对应ReqSubscribePosition订阅
void CtpRiskSpi::OnRtnSequencialPosition(
    CShfeFtdcSequencialPositionField *pSequencialPosition)
{
  CShfeFtdcSequencialPositionField* pos= new CShfeFtdcSequencialPositionField();
  memcpy(pos,pSequencialPosition,sizeof(CShfeFtdcSequencialPositionField));
  bool founded=false;    unsigned int i=0;
  for(i=0; i<posList.size(); i++){
    if(strcmp(posList[i]->InstrumentID, pos->InstrumentID)==0
      && strcmp(posList[i]->InvestorID, pos->InvestorID)==0
      && posList[i]->PosiDirection == pos->PosiDirection
      && posList[i]->HedgeFlag == pos->HedgeFlag) {
      founded=true;    break;
    }
  }
  if(founded) posList[i]= pos;   
  else  posList.push_back(pos);
  //cerr<<" 回报 | 持仓回报接收...序号:"<<pos->UniqSequenceNo<<endl;
  //SetEvent(g_hEvent);	
}

///有序的经纪公司用户事件回报,对应ReqSubBrokerUserEvent订阅
void CtpRiskSpi::OnRtnSequencialBrokerUserEvent(
    CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent){}

///有序的昨日资金回报，对应订阅请求ReqSubPreRiskAccount
void CtpRiskSpi::OnRtnSeqPreRiskAccount(
    CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount){}

///投资者当日资金回报,不需要订阅就自动返回
void CtpRiskSpi::OnRtnRiskSyncAccount(
    CShfeFtdcRiskSyncAccountField *pRiskSyncAccount){
  CShfeFtdcRiskSyncAccountField* cap= new CShfeFtdcRiskSyncAccountField();
  memcpy(cap, pRiskSyncAccount,sizeof(CShfeFtdcRiskSyncAccountField));
  bool founded=false;    unsigned int i=0;
  for(i=0; i<capList.size(); i++){
    if(capList[i]->InvestorID == cap->InvestorID) {
      founded=true;    break;
    }
  }
  if(founded) capList[i]= cap;   
  else  capList.push_back(cap);
  //cerr<<" 回报 | 资金回报接收...投资者:"<<cap->InvestorID<<endl;
  //SetEvent(g_hEvent);
}

///有序的出入金回报，对应订阅请求ReqSubSeqDeposit
void CtpRiskSpi::OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit){}

///有序的交易编码回报,对应订阅请求ReqSubSeqTradingCode
void CtpRiskSpi::OnRtnSeqTradingCode(
    CShfeFtdcSeqTradingCodeField *pSeqTradingCode){}

///有序的风险通知回报,对应订阅请求ReqSubSeqRiskNotifyB
void CtpRiskSpi::OnRtnSeqRiskNotifyB(
   CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB){ }

///有序的业务通知回报,对应订阅请求ReqSubSeqData
void CtpRiskSpi::OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice){}

///风控深度行情通知,对应订阅请求ReqSubRiskMarketData
void CtpRiskSpi::OnRtnRiskDepthMarketData(
  CShfeFtdcDepthMarketDataField *pDepthMarketData){}

///查询报单排名应答,对应ReqQryOrderStat查询
void CtpRiskSpi::OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat,
  CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

///查询成交排名应答,对应ReqQryTradeStat查询
void CtpRiskSpi::OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, 
  CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

///查询持仓排名应答,对应ReqQryPositionStat查询
void CtpRiskSpi::OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, 
           CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

///合约持仓比例应答,对应ReqQryInstPositionRate查询
void CtpRiskSpi::OnRspInstPositionRate(
  CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, 
  CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

///产品持仓比例应答,对应ReqQryProductPositionRate查询
void CtpRiskSpi::OnRspProductPositionRate(
  CShfeFtdcRspProductPositionRateField *pRspProductPositionRate,
  CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

///以下八个关于保证金率的函数会自动回调,不需要请求
///经纪公司合约保证金率
void CtpRiskSpi::OnRtnInstrumentMarginRate(
  CShfeFtdcInstrumentMarginRateField *pInstrumentMarginRate){}

///经纪公司合约保证金率调整
void CtpRiskSpi::OnRtnInstrumentMarginRateAdjust(
  CShfeFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust){}

///交易所保证金率
void CtpRiskSpi::OnRtnExchangeMarginRate(
  CShfeFtdcExchangeMarginRateField *pExchangeMarginRate){}

///交易所保证金率调整
void CtpRiskSpi::OnRtnExchangeMarginRateAdjust(
  CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust){}

///删除经纪公司合约保证金率
void CtpRiskSpi::OnRtnDelInstrumentMarginRate(
  CShfeFtdcInstrumentMarginRateField *pInstrumentMarginRate){}

///删除经纪公司合约保证金率调整
void CtpRiskSpi::OnRtnDelInstrumentMarginRateAdjust(
  CShfeFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust){}

///删除交易所保证金率
void CtpRiskSpi::OnRtnDelExchangeMarginRate(
    CShfeFtdcExchangeMarginRateField *pExchangeMarginRate){}

///删除交易所保证金率调整
void CtpRiskSpi::OnRtnDelExchangeMarginRateAdjust(
    CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust){}

///下强平单
void CtpRiskSpi::ReqRiskOrderInsert(TShfeFtdcSequenceNoType seqNo,
    TShfeFtdcPriceType price)
{
  bool found=false; unsigned int i=0;
  for(i=0;i<posList.size();i++){
    if(posList[i]->UniqSequenceNo == seqNo){ found = true; break;}
  }
  if(!found){cerr<<" 请求 | 持仓不存在."<<endl; return;} 

	CShfeFtdcRiskForceCloseOrderField field;
  memset(&field,0,sizeof(field));
	strcpy(field.BrokerID,  appId);
	strcpy(field.UserID,  userId);
	strcpy(field.InvestorID,posList[i]->InvestorID);
	strcpy(field.InstrumentID,posList[i]->InstrumentID);	
	
	field.FCType = SHFE_FTDC_FCT_Manual;///风控强平类型
	field.CombHedgeFlag[0]=posList[i]->HedgeFlag;	
	field.Direction= (posList[i]->PosiDirection==SHFE_FTDC_PD_Long) ? 
                        SHFE_FTDC_D_Sell : SHFE_FTDC_D_Buy;
	field.CombOffsetFlag[0]=SHFE_FTDC_OF_Close;//平仓; 对上期所合约也可平今
	field.UserForceClose=1;//必须为1
	field.LimitPrice= price;
	field.VolumeTotalOriginal=1;	
	field.ContingentCondition=SHFE_FTDC_CC_Immediately;	
	field.OrderPriceType=SHFE_FTDC_OPT_LimitPrice;//限价单，上期所也支持市价单
	field.TimeCondition=SHFE_FTDC_TC_GFD;
	field.VolumeCondition=SHFE_FTDC_VC_AV;
	field.ForceCloseReason=SHFE_FTDC_FCC_NotForceClose;
	
	int ret=pUserApi->ReqRiskOrderInsert(&field,++requestId);	
  cerr<< " 请求 | 发送强平单..." <<((ret == 0)?"成功":"失败") << endl;
}

///下强平单请求响应
void CtpRiskSpi::OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, 
   CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( !IsErrorRspInfo(pRspInfo) && pInputOrder){
		cerr <<" 响应 | 下强平单成功..."<<endl;
	}
  if(bIsLast) SetEvent(g_hEvent);
}
//下强平预埋单
void CtpRiskSpi::ReqRiskParkedOrderInsert(
  CShfeFtdcSequencialPositionField *pData)
{
  CShfeFtdcRiskParkedOrderField field={0};	
	field.UserType=SHFE_FTDC_RUT_SuperUser;
	field.TriggerType=SHFE_FTDC_OTT_Customize;//指定按时间触发;也可按交易阶段触发
	field.TradeSegment=0;
	strcpy(field.Time2,"14:30:00");
	strcpy(field.ExchangeID,"SHFE");//真实环境中应该从合约信息中取得该字段
	
	strcpy(field.BrokerID,appId);
	strcpy(field.UserID,userId);
	strcpy(field.InvestorID,  pData->InvestorID);
	strcpy(field.InstrumentID,  pData->InstrumentID);
	
	field.FCType = SHFE_FTDC_FCT_Manual;///风控强平类型
	field.CombHedgeFlag[0] = pData->HedgeFlag;	
	field.Direction= (pData->PosiDirection==SHFE_FTDC_PD_Long ) ?
                      SHFE_FTDC_D_Sell : SHFE_FTDC_D_Buy;
	field.CombOffsetFlag[0]=SHFE_FTDC_OF_Close;//平仓; 对上期所合约也可平今
	field.UserForceClose=1; //必须为1
	field.LimitPrice=  1000;
	field.VolumeTotalOriginal=1;	
	field.ContingentCondition=SHFE_FTDC_CC_Immediately;	
	field.OrderPriceType=SHFE_FTDC_OPT_LimitPrice;//限价单，上期所也支持市价单
	field.TimeCondition=SHFE_FTDC_TC_GFD;
	field.VolumeCondition=SHFE_FTDC_VC_AV;
	field.ForceCloseReason=SHFE_FTDC_FCC_NotForceClose;
	field.ForceCloseRelease=SHFE_FTDC_FCR_RealTime;
	int ret=pUserApi->ReqRiskParkedOrderInsert(&field,++requestId);
  cerr<< " 请求 | 发送预埋强平单..." <<((ret == 0)?"成功":"失败") << endl;
}

//下预埋单请求响应
void CtpRiskSpi::OnRspRiskParkedOrderInsert(
  CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, 
  CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( !IsErrorRspInfo(pRspInfo) && pRiskParkedOrder)	{
		cerr <<" 响应 | 预埋强平单...成功"<<endl;
	}
  if (bIsLast) SetEvent(g_hEvent);
}
//撤单
void CtpRiskSpi::ReqRiskOrderAction(CShfeFtdcSequencialOrderField *pData)
{
	CShfeFtdcInputOrderActionField req={0};
	strcpy(req.BrokerID,pData->BrokerID);
	strcpy(req.InvestorID,pData->InvestorID);
	req.OrderActionRef=0;
	strcpy(req.OrderRef,pData->OrderRef);
	req.RequestID=pData->RequestID;
	req.FrontID=pData->FrontID;
	req.SessionID=pData->SessionID;
	strcpy(req.ExchangeID,pData->ExchangeID);
	strcpy(req.OrderSysID,pData->OrderSysID);
	req.ActionFlag=SHFE_FTDC_AF_Delete;//暂时不支持改单
	req.LimitPrice=pData->LimitPrice;
	req.VolumeChange=0;
	strcpy(req.UserID,userId);
	strcpy(req.InstrumentID,pData->InstrumentID);
  
  int ret=	pUserApi->ReqRiskOrderAction(&req,++requestId);
  cerr<< " 请求 | 发送撤单..." <<((ret == 0)?"成功":"失败") << endl;
}

///撤单响应
void CtpRiskSpi::OnRspRiskOrderAction(
    CShfeFtdcInputOrderActionField *pInputOrderAction, 
    CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( !IsErrorRspInfo(pRspInfo) && pInputOrderAction){
    cerr <<" 响应 | 撤单成功...交易所:"
      <<pInputOrderAction->ExchangeID
      <<" 报单编号:"<<pInputOrderAction->OrderSysID<<endl;
	}
  if (bIsLast) SetEvent(g_hEvent);
}
//撤预埋单
void CtpRiskSpi::ReqRemoveRiskParkedOrder(
    CShfeFtdcRemoveRiskParkedOrderField* pData){ }

///撤预埋单响应
void CtpRiskSpi::OnRspRemoveRiskParkedOrder(
    CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, 
    CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//席位资金查询
void CtpRiskSpi::ReqRiskQryBrokerDeposit(TShfeFtdcExchangeIDType	exchangeId)
{
	CShfeFtdcQueryBrokerDepositField field={0};
	strcpy(field.BrokerID,appId);
	strcpy(field.ExchangeID,exchangeId);
	int ret = pUserApi->ReqRiskQryBrokerDeposit(&field,++requestId);
  cerr<<" 请求 | 发送会员资金查询..."<<((ret == 0) ? "成功" :"失败")<<endl;	
}

///查询席位资金应答
void CtpRiskSpi::OnRspRiskQryBrokerDeposit(
    CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, 
    CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{	
	if (!IsErrorRspInfo(pRspInfo) && pQueryBrokerDeposit)	{
		cerr<<" 响应 | 查询席位资金...成功"<<endl;
	}
  if (bIsLast) SetEvent(g_hEvent);
}

///席位资金回报,由ReqRiskQryBrokerDeposit触发
void CtpRiskSpi::OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit)
{
  cerr<<" 回报 | 会员:"<<pBrokerDeposit->ParticipantID
    <<" 结算准备金:"<<pBrokerDeposit->Balance
    <<" 当前保证金:"<<pBrokerDeposit->CurrMargin
    <<" 可用资金:"<<pBrokerDeposit->Available
    <<" 平仓盈亏:"<<pBrokerDeposit->CloseProfit<<endl;
  SetEvent(g_hEvent);	
}

bool CtpRiskSpi::IsErrorRspInfo(CShfeFtdcRspInfoField *pRspInfo)
{
  bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    cerr<<" 响应 | "<<pRspInfo->ErrorMsg<<endl;
  }
  return ret;
}

///连接断开
void CtpRiskSpi::OnFrontDisconnected(int nReason)
{
  cerr<<" 响应 | 连接中断..." 
    << " reason=" << nReason << endl;
}

///心跳
void CtpRiskSpi::OnHeartBeatWarning(int nTimeLapse)
{
  cerr<<" 响应 | 心跳超时警告..." 
    << " TimerLapse = " << nTimeLapse << endl;
}

///错误应答	(response to wrong/invalid quest)
void CtpRiskSpi:: OnRspError(CShfeFtdcRspInfoField *pRspInfo, 
    int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}
void CtpRiskSpi::PrintAcounts(){
  CShfeFtdcRiskSyncAccountField* cap; 
  for(unsigned int i=0; i<capList.size(); i++){
    cap = capList[i];
    cerr<<" 资金 | 投资者:"<<cap->InvestorID
      <<" 权益:"<< cap->Balance
      <<" 可用:"<<cap->Available
      <<" 当前保证金:"<< cap->CurrMargin 
      <<" 冻结保证金:"<< cap->FrozenMargin      
      <<" 冻结手续费:"<< cap->FrozenCommission
      <<" 平仓盈亏:"<<cap->CloseProfit 
      <<" 持仓盈亏:"<<cap->PositionProfit<<endl;
  }
  SetEvent(g_hEvent);
}
void CtpRiskSpi::PrintPositions(){
  CShfeFtdcSequencialPositionField* pos; 
  for(unsigned int i=0; i<posList.size(); i++){
    pos = posList[i];
    cerr<<" 持仓 | 序号:"<<pos->UniqSequenceNo
      <<" 投资者:"<<pos->InvestorID 
      <<" 合约:"<< pos->InstrumentID
      <<" 方向:"<< pos->PosiDirection 
      <<" 总持仓:"<<pos->Position
      <<" 今仓:"<< pos->TodayPosition
      <<" 持仓盈亏:"<<pos->PositionProfit
      <<" 占用保证金:"<<pos->UseMargin<<endl;
  }
  SetEvent(g_hEvent);
}
