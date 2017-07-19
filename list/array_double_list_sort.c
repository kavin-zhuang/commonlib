/*
 * If directly call list_del, we cannot call list_add, as it points to NULL.
 * As the items can be active dynamiclly, if not active, we can move it to another list.
 */

#include "list.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define PRINT(fmt, ...) \
	printf(fmt, ##__VA_ARGS__)

#define TRACE(fmt, ...) \
	printf("TRACE: %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define EXCEPTION(fmt, ...) \
	printf("EXCEPTION: %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

typedef struct {
	struct list_head list;

	int id;
} item_t;

item_t items[10];
struct list_head active_list;
struct list_head wait_list;

void display_active_data(void)
{
	struct list_head *pos;
	item_t *temp;

	list_for_each(pos, &active_list) {
		temp = list_entry(pos, item_t, list);
		printf("%d ", temp->id);
	}
}

void move_to_wait_list(item_t *item)
{
	list_add(&item->list, &wait_list);
}

void move_to_active_list(item_t *item)
{
	struct list_head *pos;
	item_t *temp;

	if (list_empty(&active_list)) {
		list_add(&item->list, &active_list);
		goto end;
	}

	list_for_each(pos, &active_list) {
		temp = list_entry(pos, item_t, list);

		if (item->id < temp->id) {
			list_add_tail(&item->list, pos);
			goto end;
		}
	}

	/* now pos point to header: active_list */

	/* this item id is the max one */
	list_add_tail(&item->list, pos);

end:
	printf("now: ");
	display_active_data();
	printf("\n");
	return;
}

static void data_init(void)
{
	int i;

	srand(time(0));

	for (i = 0; i < 10; i++) {
		INIT_LIST_HEAD(&items[i].list);
		items[i].id = rand() % 100;

		printf("%d ", items[i].id);
	}

	INIT_LIST_HEAD(&active_list);
}

void add_all_data(void)
{
	int i;

	for (i = 0; i < 10; i++) {
		move_to_active_list(&items[i]);
	}
}

void test_list(void)
{
	data_init();
	add_all_data();
}

