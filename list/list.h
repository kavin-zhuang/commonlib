#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#if defined(WIN32) && !defined(__cplusplus)  
#define inline __inline  
#endif

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

static inline void __list_add(struct list_head *new1,
	struct list_head *prev,
	struct list_head *next)
{
	next->prev = new1;
	new1->next = next;
	new1->prev = prev;
	prev->next = new1;
}

static inline void list_add(struct list_head *new1, struct list_head *head)
{
	__list_add(new1, head, head->next);
}

static inline void list_add_tail(struct list_head *new1, struct list_head *head)
{
	__list_add(new1, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = 0;
	entry->prev = 0;
}

static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_entry(pos, head, member)                                \
for (pos = list_entry((head)->next, typeof(*pos), member);                        \
	&pos->member != (head);                                         \
	pos = list_entry(pos->member.next, typeof(*pos), member))

#endif