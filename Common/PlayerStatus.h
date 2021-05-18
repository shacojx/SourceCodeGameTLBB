

#ifndef __PLAYERSTATUS_H__
#define __PLAYERSTATUS_H__


enum PLAYER_STATUS
{
	/////////////////////////////
	//游戏服务器中的玩家状态
	/////////////////////////////

	//玩家数据在服务器端的初始状态
	PS_SERVER_EMPTY = 100 ,		

	//玩家网络连接成功后的状态，下一个需要发送的消息是：
	//CGConnect，如果N秒内没收到这个消息，则断开此连接
	PS_SERVER_CONNECT ,		

	//如果进入场景时候，用户是从另外一个服务器的某个场景里过来的，
	//则需要向源场景请求用户数据，在用户数据发送过来前的玩家状态
	PS_SERVER_WAITING_FOR_SERVER_DATA ,
	PS_SERVER_WAITING_FOR_SERVER_DATA_INCOMING ,

	// 经过服务器的验证后等待客户端发送CGEnterScene消息
	// 如果一段时间内没有收到CGEnterScene消息，则断开连接
	PS_SERVER_WAITING_FOR_ENTER ,	
								
	//对于刚接入的客户端连接，服务器接收到CGEnterScene消息后从
	//IncomingPlayerManager移除信息准备加入Scene中
	//对于已经在场景中的客户端连接，客户端需要转换场景
	//服务器收到CGEnterScene消息后从原场景里移除信息，准备加入到新场景
	PS_SERVER_READY_TO_ENTER ,	

	//玩家加入场景后需要向服务器发送 CGEnvRequset消息请求周围的相关信息
	//此时玩家状态为 ：
	PS_SERVER_WAITTING_FOR_ENVREQUEST ,


	//客户端玩家状态进入正常游戏状态
	PS_SERVER_NORMAL ,

	//客户端发送了CGAskChangeScene消息后，服务器在向世界数据服务器发送验证
	//此时客户端继续和游戏服务器保持网络连接，但是处于特殊状态中
	PS_SERVER_WAITING_FOR_CHANGESCENE ,
	PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE ,

	//玩家所用的GUID已经被另外一个新的登陆使用，此连接必须被踢掉
	PS_SERVER_ANOTHER_GUID_ENTER ,
	PS_SERVER_ANOTHER_GUID_ALSO_KICK ,

	//玩家已经从服务器上断开，并且进入数据保存阶段
	PS_SERVER_ENTER_RECYCLE ,
	PS_SERVER_LEAVE_RECYCLE ,


	///////////////////////////////
	//游戏客户端中的玩家状态
	///////////////////////////////
	PS_CLIENT_LOAD = 200 ,
	PS_CLIENT_CONNECT ,

	///////////////////////////////
	//登陆服务器中的玩家状态
	///////////////////////////////
	PS_LOGIN_EMPTY			= 300 , //池中数据
	PS_LOGIN_CONNECT,				//连接并且发送了数据		
	PS_LOGIN_WAIT_AUTH,				//等待login 服务器验证
	PS_LOGIN_AUTHED,				//验证通过的客户端(合法),以后发送GUID就可以
	PS_LOGIN_WAIT_PROCESS_TURN,		//等待加入逻辑操作排队队列
	PS_LOGIN_PROCESS_TURN,			//已经在逻辑操作排队队列中客户端
	PS_LOGIN_NORMAL ,				//可以逻辑操作客户端。
	PS_LOGIN_SERVER_READY,			//可以进入Server 的客户端
	

	///////////////////////////////
	//计费系统中的玩家状态
	///////////////////////////////
	PS_BILLING_CONNECT = 400 ,

	///////////////////////////////
	//世界数据服务器中的连接者状态
	///////////////////////////////
	PS_WORLD_CONNECT = 500 ,
	PS_WORLD_NORMAL ,




};

#endif
