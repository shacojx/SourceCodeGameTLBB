#ifndef _SHAREMEM_MANAGER_H_
#define _SHAREMEM_MANAGER_H_

	
struct  GuildSMU;
struct  MailSMU;

template<typename T> 
class SMUPool;

template<typename T>
class SMUManager;


extern SMUPool<GuildSMU>				g_GuildSMUPool;
extern SMUManager<GuildSMU>				g_GuildSMUManager;			
extern SMUPool<MailSMU>					g_MailSMUPool;
extern SMUManager<MailSMU>				g_MailSMUManager;			





#endif