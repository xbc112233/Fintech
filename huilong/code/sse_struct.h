#pragma once

#define MD_TYPE_SH_TRADE 0x0005
#define MD_TYPE_SH_ORDER 0x0008

#pragma pack(push, 1)
struct SSEOrder
{
	int32_t		msg_type;                   //消息类型             
	char			security_id[8];       //产品代码        

	int16_t		channel;							//通道
	int64_t		order_index;                //委托序号，从1开始，按channel连续
	int32_t		order_time;                 //委托时间，e.g.14302506表示14:30:25.06（百分之一秒）
	char			order_type;                 //订单类型，A-委托订单增加，D-委托订单删除
	int64_t		order_no;                 //原始订单号
	int64_t		order_price;                //委托价格（元）
	int64_t		order_balance;            //剩余委托量
	char			order_bs_flag;              //委托标识，B-买单，S-卖单
	int64_t		order_biz_index;          //业务序列号，与成交统一编号，从1开始，按channel连续

};


struct SSETrade
{
	int32_t		msg_type;                	//消息类型
	char			security_id[8];    	//产品代码
							
	int16_t 	channel;                 //成交通道
	int64_t 	trade_index;                   //成交序号
	int32_t 	trade_time;                    //成交时间, e.g.10302019表示10:30:20.19（百分之一秒）
	int64_t 	trade_price;                   //成交价格,除以1000   
	int64_t 	trade_qty;               //成交数量,除以1000
	int64_t 	trade_money;             //成交金额,除以100000
	int64_t 	trade_buy_no;            //买方订单号
	int64_t 	trade_sell_no;           //卖方订单号
	char 			trade_bs_flag;									//内外盘标志
	int64_t		trade_biz_index;								//业务序列号，与委托统一编号，从1开始，按channel连续
};
#pragma pack(pop)