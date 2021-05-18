#include "stdafx.h"
#include "SqlTemplate.h"

CHAR*		LoadCharList		= 
"select \
charguid,sex,charname,level,haircolor,facecolor,hairmodel,facemodel,scene,menpai, \
headid, camp \
from %s where accname =\'%s\' and isvalid =1";
/*
UINT		ItemType;
SHORT		Pos
*/
CHAR*		LoadCharEquipList	=
"select \
itemtype,pos \
from %s where charguid =%d and pos>=%d and pos<%d and isvalid =1";


/*
 *	角色基本信息
 */
CHAR*		LoadCharFullData	=
"select \
charguid,charname,title,sex,createtime,level,enegry,exp,vmoney,pw,\
haircolor,facecolor,hairmodel,facemodel,scene,xpos,zpos,logintime,logouttime,dbversion,\
camp,menpai,hp,mp,strikepoint,str,spr,con,ipr,dex,points,settings,\
shopinfo,carrypet,guldid,teamid,headid,erecover,vigor,maxvigor,vrecover,energymax,\
pwdeltime,dietime,bankmoney,bankend,bkscene,bkxpos,bkzpos,rage \
from %s where charguid = %d and isvalid = 1";



CHAR*		UpdateCharFullData	=	
"update \
%s  set title=\'%s\',sex=%d,level=%d,enegry=%d,exp=%d,vmoney=%d,pw=\'%s\',\
haircolor=%d,facecolor=%d,hairmodel=%d,facemodel=%d,scene=%d,xpos=%d,zpos=%d,logintime=%d,logouttime=%d, \
camp=\'%s\',menpai=%d,hp=%d,mp=%d,strikepoint=%d,str=%d,spr=%d,con=%d,ipr=%d,dex=%d,points=%d,settings=\'%s\', \
shopinfo = \'%s\',carrypet = \'%s\' ,guldid= %d,teamid =%d,headid=%d,erecover=%d,vigor=%d,maxvigor=%d,vrecover=%d,energymax=%d, \
pwdeltime=%d,dietime=%d,bankmoney=%d,bankend=%d,bkscene=%d,bkxpos=%d,bkzpos=%d,rage=%d,dbversion=%d  \
where charguid = %d and isvalid = 1";

/*
 *	角色物品表加载
 */

CHAR*		LoadCharItemList	=
"select \
charguid,world,server,guid,itemtype,pos,fixattr,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,dbversion,creator,var \
from %s where charguid = %d and isvalid = 1";

/*
 *	角色物品表删除
 */
CHAR*	   DeleteCharItemList  =
"delete from %s where charguid = %d";


/*
 *	角色技能列表
 */

CHAR*	   LoadCharSkillList	=	
"select \
skid,sktime \
from %s where charguid = %d and dbversion=%d and isvalid = 1";

/*
 *	 删除角色技能
 */
CHAR*	   DeleteCharSkillList = 
"delete \
from %s where charguid = %d and dbversion<=%d";


/*
 *	角色心法列表
 */

CHAR*	LoadCharXinFaList	=
"select \
charguid,xinfaid,xinfalvl \
from %s where charguid = %d and dbversion=%d";

/*
 *	删除角色心法
 */
CHAR*  DeleteCharXinFaList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
 *	生活技能列表
 */

CHAR*	LoadCharAbilityList = 
"select \
charguid,abid,ablvl,abexp \
from %s where charguid = %d and dbversion=%d";

CHAR*	LoadCharPresList =
"select \
charguid,pres from %s where charguid = %d and dbversion=%d";



/*
*	删除生活技能列表
*/
CHAR* DeleteCharAbilityList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
 *	任务列表
 */
CHAR*	LoadCharTaskList = 
"select \
charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8 \
from %s where charguid = %d and dbversion=%d";

/*
*	加载任务数据
*/
CHAR*	LoadCharTaskDataAndFlag = 
"select \
charguid,mdata,mflag \
from %s where charguid = %d and dbversion=%d";

/*
 *	加载私人信息
 */
CHAR*	LoadCharPrivateInfo =
"select \
charguid,pinfo from %s where charguid=%d and dbversion=%d";

/*
*	加载称号信息
*/
CHAR*	LoadCharTitleInfo =
"select \
charguid,titleinfo from %s where charguid=%d and dbversion=%d";


/*
*	加载冷却信息
*/
CHAR*	LoadCharCoolDownInfo =
"select \
charguid,cooldown from %s where charguid=%d and dbversion=%d";


/*
 *	删除任务列表
 */
CHAR* DeleteCharTaskList =
"delete from %s where charguid = %d and dbversion<=%d";

/*
 *	关系列表
 */

CHAR*	LoadCharRelationList	=	
"select \
charguid,fguid,fname,fpoint,reflag,groupid,extdata \
from %s where charguid = %d and dbversion=%d";

CHAR*   LoadCharRelationData  =
"select \
charguid,relflag from %s where charguid = %d and isvalid =1 and dbversion=%d";

/*
 *	关系列表删除
 */

CHAR* DeleteCharRelationList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
 *	更新角色关系表
 */
CHAR* UpdateCharRelationData	=
"update %s set relflag = \'%s\' where charguid = %d and isvalid =1 and dbversion=%d";


/*
 *	更新配方表
 */
CHAR* UpdateCharPresList	=
"update %s set pres = \'%s\' where charguid = %d and  isvalid = 1 and dbversion = %d";

/*
 *	Impact列表
 */

CHAR*  LoadCharImpactList	=
"select \
charguid,imdata \
from %s where charguid = %d and dbversion = %d";
/*
 *	Impact列表删除
 */
CHAR* DeleteCharImpactList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
 *	宠物列表
 */

CHAR*	LoadCharPetList	=
"select \
charguid ,hpetguid,lpetguid,dataid,petname,petnick,level,needlevel,atttype,aitype,camp,hp,mp,\
life,pettype,genera,enjoy,strper,conper,dexper,sprper,iprper,gengu,growrate,\
repoint,exp,str,con,dex,spr,ipr,skill \
from %s where charguid =%d and dbversion=%d";
/*
 *	宠物列表删除
 */

CHAR*  DeleteCharPetList =
"delete \
from %s where charguid = %d and dbversion<=%d";


/*
 *	创建新角色
 */
CHAR* CreateNewChar	=
"insert into %s(accname,charguid,charname,title,pw,sex,level,enegry,outlook,scene,xpos,zpos,menpai,\
hp,mp,strikepoint,camp,str,con,dex,spr,ipr,points,logouttime,logintime,createtime,dbversion,haircolor,\
hairmodel,facecolor,facemodel,vmoney,settings,isvalid,exp,pres,\
shopinfo,carrypet,guldid,teamid,headid,erecover,vigor,maxvigor,vrecover,energymax,pwdeltime,\
pinfo,bkscene,bkxpos,bkzpos,titleinfo,dietime,bankmoney,bankend,cooldown) \
 values(\'%s\',%d,\'%s\',\'%s\',\'%s\',\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,\'%s\',\
\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,\
\'%s\',%d,%d,%d,\'%s\',%d,%d,%d,\'%s\')";

/*
 *	获取新guid
 */
CHAR*	FetchNewGuid =
"declare @guid int \r\n\
set nocount on \r\n\
exec @guid = fetch_guid\r\n\
set nocount off \r\n\
select @guid B";

/*
 *	删除角色
 */
CHAR*	DeleteBaseChar =
"update %s set isvalid = 0,charname = charname+'@DELETE_'+CAST(%d AS CHAR) \
where accname= \'%s\' and charguid = %d";

 /*
  *	创建物品
  */
CHAR* NewCharItem	=
"save_iteminfo @pcharguid=%d,@pguid=%d,@pworld=%d,@pserver=%d,@pitemtype=%u,@ppos=%d,@pfixattr=\'%s\',\
@pp1=%d,@pp2=%d,@pp3=%d,@pp4=%d,@pp5=%d,@pp6=%d,@pp7=%d,@pp8=%d,@pp9=%d,@pp10=%d,@pp11=%d,@pp12=%d,@pp13=%d,@pp14=%d,\
@pisvalid=%d,@pdbversion=%d,@pcreator=\'%s\',@pvar=\'%s\'";

//CHAR* NewCharItem	=
//"insert into %s(charguid,world,server,guid,itemtype,pos,fixattr,\
//p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,isvalid,dbversion) \
// values(%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)\n";

/*
 *	创建技能
 */
CHAR*	NewCharSkill =
"insert into %s(charguid,skid,sktime,dbversion,isvalid) \
	values(%d,%d,%d,%d,1) \n";


/*
*	创建心法
*/
CHAR*	NewCharXinFa =
"insert into %s(charguid,xinfaid,xinfalvl,dbversion) \
values(%d,%d,%d,%d)";


/*
*	创建生活技能
*/
CHAR*	NewCharAbility =
"insert into %s(charguid,abid,ablvl,abexp,dbversion) \
values(%d,%d,%d,%d,%d)";

/*
*	创建任务
*/
CHAR*	NewCharTask =
"insert into %s(charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8,dbversion) \
values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

/*
 *	更新任务数据
 */
CHAR*	UpdateCharTaskData =
"update \
%s  set mdata=\'%s\' \
where charguid = %d and isvalid = 1 and dbversion=%d";

CHAR*	UpdateCharTaskFlag =
"update \
%s set mflag=\'%s\' \
where charguid = %d and isvalid = 1 and dbversion=%d";
/*
 *	更新私人信息
 */
CHAR*	UpdateCharPrivateInfo = 
"update \
%s set pinfo=\'%s\' where charguid=%d and isvalid=1 and dbversion=%d";

/*
*	更新称号信息
*/
CHAR*	UpdateCharTitleInfo = 
"update \
%s set titleinfo=\'%s\'\
where charguid=%d and isvalid=1 and dbversion=%d";


/*
*	更新冷却信息
*/
CHAR*	UpdateCharCoolDownInfo = 
"update \
%s set cooldown=\'%s\'\
where charguid=%d and isvalid=1 and dbversion=%d";


/*
 *	创建关系信息
 */

CHAR*	NewCharRelation	 =
"insert into %s(charguid,fguid,fname,fpoint,reflag,groupid,extdata,dbversion) \
values(%d,%d,\'%s\',%d,%d,%d,\'%s\',%d)";

/*
 *	创建Impact信息
 */
CHAR*	NewCharImpact	=
"insert into %s(charguid,imdata,dbversion) \
values(%d,\'%s\',%d)";

/*
 *	创建宠物信息
 */
CHAR* NewCharPet		=
"insert into %s(charguid ,hpetguid,lpetguid,dataid,petname,petnick,level,needlevel,atttype,aitype,camp,hp,mp,\
life,pettype,genera,enjoy,strper,conper,dexper,sprper,iprper,gengu,growrate,\
repoint,exp,str,con,dex,spr,ipr,skill,dbversion) \
values(%d,%d,%d,%d,\'%s\',\'%s\',%d,%d,%d,%d,\'%s\',%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,\'%s\',%d)";

/*
 *	载入服务器物品序列号 
 */

CHAR* LoadServerItemSerial =
"select sid,serial from %s where sid = %u";

/*
 *	更新服务器物品序列号
 */

CHAR* UpdateServerItemSerial = 
"update %s set serial = %u where sid = %u";

/*
 *	载入世界Email信息
 */
CHAR* LoadWorldMailInfo	=
"select pindex, sender,recer,mailinfo,mailcont from %s";

/*
*	新建世界Email信息
*/
CHAR*	SaveWorldMailInfo	=
"save_mailinfo @p1=\'%s\',@p2=\'%s\',@p3=\'%s\',@p4=\'%s\',@p5=%d,@p6=%d";

/*
*	删除世界Email信息
*/
CHAR*	DeleteWorldMailInfo	=
"update t_mail set isvalid = 0 where pindex=%d";

/*
*	载入世界工会信息
*/
CHAR*	LoadWorldGuildInfo	=
"select  guildid,guildname,guildstat,chiefguid,pcount,ucount,mucount,gpoint,guildmoney,cityid,\
time,logevity,contribu,honor,indlvl,agrlvl,comlvl,deflvl,techlvl,ambilvl,admin,guilddesc, \
chiefname,cname,glvl,guilduser \
from %s where isvalid=1";

/*
*	新建世界工会信息
*/
CHAR*	SaveWorldGuildInfo	=
"save_guildinfo @pguildid=%d,@pguildname=\'%s\',@pguildstat=%d,@pchiefguid=%d,@ppcount=%d,\
@pucount=%d,@pmucount=%d,@pgpoint=%d,@pguildmoney=%d,@pcityid=%d,@ptime=%d,@plogevity=%d,\
@pcontribu=%d,@phonor=%d,@pindlvl=%d,@pagrlvl=%d,@pcomlvl=%d,@pdeflvl=%d,@ptechlvl=%d,@pambilvl=%d,\
@padmin=\'%s\',@pguilddesc=\'%s\',@pchiefname=\'%s\',@pcname=\'%s\',@pglvl=%d,@pguilduser=\'%s\',@pisvalid=%d";

/*
*	删除世界工会信息
*/
CHAR*	DeleteWorldGuildInfo	=
"update t_guild set isvalid = 0 where pindex=%d";

/*
*	加载冲值信息
*/
CHAR*	LoadWebShopInfo = 
"select top 1 \
Id,Money,Item_1,ItemCount_1,Item_2,ItemCount_2,Item_3,ItemCount_3,Item_4,ItemCount_4,\
Item_5,ItemCount_5 from %s where UserId=%d and IsSucceed=%d order by CreateTime";

/*
*	保存冲值信息
*/
CHAR*	SaveWebShopInfo = 
"update %s set IsSucceed=%d where Id=%d";
