// RCUserSpiImpl.h : interface of the CRiskSpi class
//

#ifndef RISK_RISKSPI_H_
#define RISK_RISKSPI_H_
#pragma once
#include "api/risk/public/FtdcRiskUserApi.h"

class CtpRiskSpi : public CShfeFtdcRiskUserSpi
{
public:

  CtpRiskSpi(CShfeFtdcRiskUserApi *p):pUserApi(p){};
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void OnHeartBeatWarning(int nTimeLapse);	

	///错误应答
	virtual void OnRspError(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	
	///客户端结算组数据同步开始
	virtual void OnRtnClientSGDataSyncStart(CShfeFtdcSettlementSessionField *pSettlementSession) ;	

	///登录请求响应
	virtual void OnRspRiskUserLogin(CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	
	///时间同步
	virtual void OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime);

	///交易所信息
	virtual void OnRtnExchange(CShfeFtdcExchangeField *pExchange);
	
	///产品信息
	virtual void OnRtnProduct(CShfeFtdcProductField *pProduct);

	///合约信息
	virtual void OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument);

	///投资者组
	virtual void OnRtnInvestorGroup(CShfeFtdcInvestorGroupField *pInvestorGroup);

	///组织架构名称
	virtual void OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment);

	///客户端结算组数据同步结束
	virtual void OnRtnClientSGDataSyncEnd(CShfeFtdcSettlementSessionField *pSettlementSession) ;

	///查询投资者信息应答
	virtual void OnRspQryInvestorLinkMan(CShfeFtdcInvestorLinkManField *pInvestorLinkMan, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///查询投资者组织架构应答
	virtual void OnRspQryInvestorDepartment(CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///有序的报单回报
	virtual void OnRtnSequencialOrder(CShfeFtdcSequencialOrderField *pSequencialOrder);

	///有序的成交回报
	virtual void OnRtnSequencialTrade(CShfeFtdcSequencialTradeField *pSequencialTrade);

	///有序的持仓回报
	virtual void OnRtnSequencialPosition(CShfeFtdcSequencialPositionField *pSequencialPosition);

	///有序的经纪公司用户事件回报
	virtual void OnRtnSequencialBrokerUserEvent(CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent);

	///有序的昨日资金回报
	virtual void OnRtnSeqPreRiskAccount(CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount);
	
	///投资者当日资金回报
	virtual void OnRtnRiskSyncAccount(CShfeFtdcRiskSyncAccountField *pRiskSyncAccount);

	///有序的出入金回报
	virtual void OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit);

	///有序的交易编码回报
	virtual void OnRtnSeqTradingCode(CShfeFtdcSeqTradingCodeField *pSeqTradingCode);

	///有序的风险通知回报
	virtual void OnRtnSeqRiskNotifyB(CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB);

	///有序的业务通知回报
	virtual void OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice);

	///风控深度行情通知
	virtual void OnRtnRiskDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData);

	///查询报单排名应答
	virtual void OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///查询成交排名应答
	virtual void OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///查询持仓排名应答
	virtual void OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///合约持仓比例应答
	virtual void OnRspInstPositionRate(CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///产品持仓比例应答
	virtual void OnRspProductPositionRate(CShfeFtdcRspProductPositionRateField *pRspProductPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	
	///经纪公司合约保证金率
	virtual void OnRtnInstrumentMarginRate(CShfeFtdcInstrumentMarginRateField *pInstrumentMarginRate);

	///经纪公司合约保证金率调整
	virtual void OnRtnInstrumentMarginRateAdjust(CShfeFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust);

	///交易所保证金率
	virtual void OnRtnExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate);

	///交易所保证金率调整
	virtual void OnRtnExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust);

	///删除经纪公司合约保证金率
	virtual void OnRtnDelInstrumentMarginRate(CShfeFtdcInstrumentMarginRateField *pInstrumentMarginRate);	

	///删除经纪公司合约保证金率调整
	virtual void OnRtnDelInstrumentMarginRateAdjust(CShfeFtdcInstrumentMarginRateAdjustField *pInstrumentMarginRateAdjust);
	
	///删除交易所保证金率
	virtual void OnRtnDelExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate);

	///删除交易所保证金率调整
	virtual void OnRtnDelExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust);

	///下强平单请求响应
	virtual void OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	//下预埋单请求响应
	virtual void OnRspRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///撤单响应
	virtual void OnRspRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		
	///撤预埋单响应
	virtual void OnRspRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///查询席位资金应答
	virtual void OnRspRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///席位资金回报
	virtual void OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit);

public:	
	//请求登录
	void ReqUserLogin( TShfeFtdcBrokerIDType	appId,
      TShfeFtdcUserIDType	userId,  TShfeFtdcPasswordType	passwd);
	//请求数据
	void ReqUserData();
	//下强平单
	void ReqRiskOrderInsert(TShfeFtdcSequenceNoType seqNo,
              TShfeFtdcPriceType price);
	//下强平预埋单
	void ReqRiskParkedOrderInsert(CShfeFtdcSequencialPositionField *pData);
	//撤单
	void ReqRiskOrderAction(CShfeFtdcSequencialOrderField *pData);
	//撤预埋单
	void ReqRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField* pData);
	//席位资金查询
	void ReqRiskQryBrokerDeposit(TShfeFtdcExchangeIDType	ExchangeID);
	//是否收到成功回应
	bool IsErrorRspInfo(CShfeFtdcRspInfoField *pRspInfo);
    
  void PrintAcounts();
  void PrintPositions();

private:
  CShfeFtdcRiskUserApi *pUserApi;
};



#endif
