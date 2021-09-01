/**
 * Copyright(C) 2014. CJP All rights reserved.
 *
 * link_queue.cpp
 * Original Author : cjpthree@126.com, 2014-7-3.
 *
 * Description
 */

#include "link_queue.h"
#include <stdio.h>

#define _ENABLE_MIAN_ 1

// connet two command to one at precompile time
#define PREC_CONN_CMD(str1, str2) (str1##str2)

#define PREC_TO_STR_TMP(s)   (#s)
// to string at precompile time
#define PREC_TO_STR(s)       PREC_TO_STR_TMP(s)

// itoa, must use immediately
char *itoa_lui(int num)
{
    static char str[128];

    memset(str, 0, sizeof(str) / sizeof(str[0]));
    sprintf(str, "%s%d", str, num);
    return str;
}


typedef struct test_data_s
{
    int it;
    long reserved;
} test_data_t;

link_queue_t lq = link_queue_init(lq);

static int print_item(link_queue_node_t *node, void *args)
{
	printf("get item %d\n", link_queue_parse_data(node, test_data_t)->it);

	return 0;
}

static void show_all()
{
    link_queue_traverse(&lq, print_item, NULL);
}

static int enqueue(int it)
{
	link_queue_node_t *pt = NULL;
	test_data_t new_data;

	new_data.it = it;
#if 0
	link_queue_alloc_node(pt, sizeof(test_data_t));
	if (!pt) {
		printf("alloc fail\n");
		return -1;
	}
	link_queue_init_node(pt, &new_data, sizeof(new_data));
#else
    link_queue_create_node(pt, &new_data, sizeof(new_data));
#endif
	link_queue_enqueue(&lq, pt);

	return 0;
}

static int dequeue()
{
	link_queue_node_t *pt = NULL;

	link_queue_dequeue(&lq, pt);
    if (pt) {
        printf("dequeue %d\n", link_queue_parse_data(pt, test_data_t)->it);
    }
    link_queue_free_node(pt);

	return 0;
}

static int peek_first_node()
{
	link_queue_node_t *pt = NULL;

	pt = link_queue_peek_first(&lq);
    if (pt) {
        printf("peek %d\n", link_queue_parse_data(pt, test_data_t)->it);
    }

	return 0;
}

static int peek_last_node()
{
	link_queue_node_t *pt = NULL;

	pt = link_queue_peek_last(&lq);
    if (pt) {
        printf("peek %d\n", link_queue_parse_data(pt, test_data_t)->it);
    }

	return 0;
}

void test_link_queue()
{
    link_queue_node_t *pt = NULL;

    if (link_queue_is_empty(&lq)) {
        printf("list is empty \n");
    }

	link_queue_set_max_len(&lq, 3);

	enqueue(11);
	enqueue(22);
	enqueue(33);
	enqueue(44);

	dequeue();
	dequeue();
	dequeue();
	dequeue();
	dequeue();
	if (link_queue_is_empty(&lq)) {
        printf("list is empty \n");
    }

	enqueue(55);
	enqueue(66);

	peek_first_node();
	peek_last_node();

    if (!link_queue_is_empty(&lq)) {
        printf("list is not empty \n");
    }

	show_all();
}

void stress_test_dequeue(void)
{
    int i, j;
    const static int tmax = 100000;

    //link_queue_t stress = link_queue_init(stress);
    printf("%s: start enqueue \n", __FUNCTION__);
    link_queue_set_max_len(&lq, tmax);
    for (i = 0; i < tmax; i++) {
        enqueue(i);
    }
    for (i = 0; i < tmax; i++) {
        dequeue();
    }
    printf("%s: end enqueue \n", __FUNCTION__);
}

static int enqueue_man(link_queue_t *queue, int it)
{
	link_queue_node_t *pt = NULL;
	test_data_t new_data;

	new_data.it = it;
    link_queue_create_node(pt, &new_data, sizeof(new_data));
	link_queue_enqueue(queue, pt);

	return 0;
}

static int dequeue_man(link_queue_t *queue)
{
	link_queue_node_t *pt = NULL;

	link_queue_dequeue(queue, pt);
    if (pt) {
        printf("dequeue %d\n", link_queue_parse_data(pt, test_data_t)->it);
    }
    link_queue_free_node(pt);

	return 0;
}

#define TMAX  5000000

void stress_test_init_destrory(void)
{
    int i, j;
    printf("%s: init %s<i> %s\n", __FUNCTION__, PREC_TO_STR(stress), PREC_TO_STR(TMAX));

    printf("%s: start init_destrory \n", __FUNCTION__);
    for (i = 0; i < TMAX; i++) {
        link_queue_t stress = link_queue_init(stress);
        printf("%s: init %s%s: %p\n", __FUNCTION__, PREC_TO_STR(stress), itoa_lui(i), &stress);
        enqueue_man(&stress, i);

        printf("%s: enqueue %s%s\n", __FUNCTION__, PREC_TO_STR(stress), itoa_lui(i));
        for (j = 0; j < 2; j++) {
            dequeue_man(&stress);
        }
        link_queue_destroy(&stress);
    }
    printf("%s: end init_destrory \n", __FUNCTION__);
}

void stress_test(void)
{
    //stress_test_dequeue();
    stress_test_init_destrory();
}

void test1()
{
    printf("test1-----------------\n");
}
void teststr()
{
    printf("----------------\n");
    PREC_CONN_CMD(test, 1)();
    PREC_CONN_CMD(, test1)();
    PREC_CONN_CMD(test1, )();
    printf("-----------------\n");
}


#if _ENABLE_MIAN_
int main(int argc, char **argv)
{
	test_link_queue();
    //stress_test();

    return 0;
}
#endif

