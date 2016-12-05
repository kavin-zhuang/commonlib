/*
 * 最好用mask方式实现
 */

#ifndef DYNAMIC_LOG_H
#define DYNAMIC_LOG_H

#define KERN_EMERG		0	/* system is unusable			*/
#define KERN_ALERT		1	/* action must be taken immediately	*/
#define KERN_CRIT		2	/* critical conditions			*/
#define KERN_ERR		3	/* error conditions			*/
#define KERN_WARNING	4	/* warning conditions			*/
#define KERN_NOTICE		5	/* normal but significant condition	*/
#define KERN_INFO		6	/* informational			*/
#define KERN_DEBUG		7	/* debug-level messages			*/

extern int dynamic_log_level;

#define dynamic_log(level, fmt,...) do {\
	if(dynamic_log_level > level)\
		printk(fmt, ##__VA_ARGS__);\
	} while(0)

#endif
