/*
 * Dispatcher.cpp
 *
 *  Created on: 2018年7月10日
 *      Author: panhongfa
 */

#include "Dispatcher.h"
#include "Opctx.h"
#include "Maintainer.h"
#include "Channel.h"
#include "TraderServantImp.h"
#include "servant/TarsLogger.h"

Dispatcher::Dispatcher(Maintainer* pwork) {
	// TODO Auto-generated constructor stub
	_pMaintainer = pwork;
}

Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}

void Dispatcher::doProcess(Event* op) {
	switch (op->getType()) {
		case Event::front_connected_event:
		case Event::front_disconnected_event:
		{
			Opctx* ctx = new Opctx();
			ctx->setEvent(op);
			ctx->setChannel(op->getSrcChannel());
			_pMaintainer->postMessage(ctx);
		}
			break;
		case Event::user_login_event:
		{
			UserLoginEvent* pEvent = dynamic_cast<UserLoginEvent*>(op);

			SessionPtr session = NULL;
			TraderCallbackPtr cb = NULL;
			if (pEvent && op->getSrcChannel()->fetchSession(pEvent->getUserLoginField()->UserID, session)
					&& session->getActionContext(pEvent->getRequestId(), cb)) {

				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		case Event::user_logout_event:
		{
			UserLogoutEvent* pEvent = dynamic_cast<UserLogoutEvent*>(op);

			SessionPtr session = NULL;
			TraderCallbackPtr cb = NULL;
			if (pEvent && op->getSrcChannel()->fetchSession(pEvent->getUserLogoutField()->UserID, session)
					&& session->getActionContext(pEvent->getRequestId(), cb)) {

				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		case Event::order_insert_event:
		{
			OrderInsertEvent* pEvent = dynamic_cast<OrderInsertEvent*>(op);

			SessionPtr session = NULL;
			OrderPtr order = NULL;
			TraderCallbackPtr cb = NULL;
			if (pEvent && op->getSrcChannel()->fetchSession(pEvent->getOrderInsertField()->UserID, session)
					&& session->fetchOrder(pEvent->getRequestId(), order)
					&& order->getActionContext(pEvent->getRequestId(), cb)) {

				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		case Event::order_action_event:
		{

		}
			break;
		case Event::order_rtn_event:
		{

		}
			break;
		case Event::trade_rtn_event:
		{

		}
			break;
		case Event::instrument_commission_event:
		{
			QryInstrumentCommissionEvent* pEvent = dynamic_cast<QryInstrumentCommissionEvent*>(op);

			InstrumentPtr instrument = NULL;
			TraderCallbackPtr cb = NULL;
			if (pEvent && op->getSrcChannel()->fetchInstrument(pEvent->getInstrumentCommissionField()->InstrumentID, instrument)
					&& instrument->getActionContext(pEvent->getRequestId(), cb)) {

				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		case Event::instrument_query_event:
		{
			QryInstrumentEvent* pEvent = dynamic_cast<QryInstrumentEvent*>(op);

			InstrumentPtr instrument = NULL;
			TraderCallbackPtr cb = NULL;
			if (pEvent && op->getSrcChannel()->fetchInstrument(pEvent->getInstrumentField()->InstrumentID, instrument)
					&& instrument->getActionContext(pEvent->getRequestId(), cb)) {

				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		case Event::marketdata_query_event:
		{
			QryDepthMarketDataEvent* pEvent = dynamic_cast<QryDepthMarketDataEvent*>(op);

			InstrumentPtr instrument = NULL;
			TraderCallbackPtr cb = NULL;
			if (pEvent && op->getSrcChannel()->fetchInstrument(pEvent->getDepthMarketDataField()->InstrumentID, instrument)
					&& instrument->getActionContext(pEvent->getRequestId(), cb)) {

				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		case Event::rsp_error_event:
		{
			TraderCallbackPtr cb = NULL;
			do {
				RspErrorEvent* pEvent = dynamic_cast<RspErrorEvent*>(op);
				if (!pEvent) {
					break;
				}

				//首先查找所有session
				SessionPtr session = NULL;
				if (op->getSrcChannel()->fetchSession(pEvent->getRequestId(), session)
						&& session->getActionContext(pEvent->getRequestId(), cb)) {
					break;
				}

				//然后再查找所有instrument
				InstrumentPtr instrument = NULL;
				if (op->getSrcChannel()->fetchInstrument(pEvent->getRequestId(), instrument)
						&& instrument->getActionContext(pEvent->getRequestId(), cb)) {
					break;
				}

				//然后再查找所有Order
				OrderPtr order = NULL;
				if (session && session->fetchOrder(pEvent->getRequestId(), order)
						&& order->getActionContext(pEvent->getRequestId(), cb)) {
					break;
				}

			} while(false);

			//派发给请求的来源线程
			if (cb) {
				TraderServantImpPtr pServant = TraderServantImpPtr::dynamicCast(cb->getServant());
				if(pServant) {
					Opctx* ctx = new Opctx();
					ctx->setEvent(op);
					ctx->setChannel(op->getSrcChannel());
					ctx->setCallback(cb);
					pServant->pushCustomMessage(ctx);
				}
			}
		}
			break;
		default:
			break;
	}
}

void Dispatcher::doTimeout() {
}
