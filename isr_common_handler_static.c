
typedef void (*isr_handler)(void *arg);

isr_handler isr_handler_array[MAX_SUPPORT];

int register_isr_handler(int num, isr_handler *handler)
{
  isr_handler_array[num].count = 0;
  isr_handler_array[num].handler = handler;
}

volatile u32 isr_nest_level_up = 0;

void isr_common_handler(int vecnum)
{
  isr_nest_level_up++;
  
  if(isr_nest_level_up) {
    WARNING;
  }
  
  if((vecnum >= 0) && (vecnum < irqcount)) {
    
    if(nestable) {
      ENABLE_INT;
    }
    
    isr_handler_array[vecnum].count++; /* 统计该中断自启动以来处理的次数 */
    isr_handler_array[vecnum].handler();
    
    if(nestable) {
      DISABLE_INT;
    }
  }
  
  isr_nest_level_up--;
}
