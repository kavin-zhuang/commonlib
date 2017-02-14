/*
 * Copyright (C) 2017 Jinfeng Zhuang.
 * All Rights Reserved.
 *
 * module_callback.c
 *
 * Jinfeng, 2017.2, create
 *
 * 模块对外的回调函数接口
 */

#define CALLBACK_MAX  (10)

typedef void (*callback)(void *arg);

/* make sure this array be cleared after system start */
static struct callback_array[CALLBACK_MAX] = {
  callback cb;
  void *arg;
} = {0};

int callback_register(callback *cb, void *arg)
{
  int i;
  
  for(i=0; i<CALLBACK_MAX; i++) {
    if(NULL == callback_array[i].cb) {
      callback_array[i].cb = cb;
      callback_array[i].arg = arg;
      break;
    }
  }
  
  if(CALLBACK_MAX == i) {
    return -1;
  }
  
  return 0;
}
