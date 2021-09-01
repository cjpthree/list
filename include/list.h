/**
 * ʵ��˫��ѭ������
 * ��ֲ��linux֮list.h�����и���ϸ˵����linux֮list.h������������������
 * ���ĵ���ͷentry�������ʾ����˼һ����������ͷentry��ʾ����
 * ����������_entryһ�ɱ���entry����_entryһ�ɱ����ڵ�
 * ���������������˵����Ϊָ��
 *
 * cjpthree@126.com 2011-4-7 Create
 * v1: 2014-4-10 �޸�Ϊ��ͨ�õ���ʽ��ȥ������inline������ʵ��Ϊ�꣬ȥ��typeof�������û�����type
 *
 * ʹ��ʾ��:
 * struct mynode {
 *     struct list_head list;
 *     // other members
 * };
 * struct list_head mylist;
 * struct mynode node1;
 * INIT_LIST_HEAD(&mylist);
 * list_add(&(node1.list), &mylist);
 */

#ifndef _LINUX_LIST_H_
#define _LINUX_LIST_H_

#include <stdio.h>

#define LIST_POISON1 NULL
#define LIST_POISON2 NULL

#ifndef offsetof
#define offsetof(type, member) \
    ((unsigned long)(&((type *)0)->member))
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *) ((char *)(ptr) - offsetof(type, member)))
#endif

/**
 * ��ϵͳ֧��Ԥ����prefetch������˺꣬��Ԥ���棬�����Ͽ�������ٶ�
 */
//#define SUPPORT_PREFETCH
#ifndef SUPPORT_PREFETCH
#undef prefetch
#define prefetch
#endif

/**
 * ˫��ѭ������ṹ����֮Ϊentry��Ƕ��ڵ���ʹ�á�������ͷentry
 *
 * ע��: Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */
typedef struct list_head {
    struct list_head *next; /* ָ������entry */
    struct list_head *prev; /* ָ��ǰ���entry */
} list_head_t;

/**
 * ��ʼ������
 * @name: ����ı�ʶ��
 *
 * ע�⣺nameΪ�����ʶ��������ָ�룬���ĵ����ֵ�head = &name
 */
#define LIST_HEAD_INIT(name) \
    {&(name), &(name)}

/**
 * ���岢��ʼ������
 * @name: ����ı�ʶ��
 */
#define LIST_HEAD(name) \
    list_head_t (name) = LIST_HEAD_INIT(name)

/**
 * ��ʼ������
 * @head: ��Ҫ��ʼ��������
 */
#define INIT_LIST_HEAD(head) \
do { \
    (head)->next = (head); \
    (head)->prev = (head); \
} while (0)

/**
 * ������������entry�����һ���µ�entry
 * @new_entry: Ҫ�����entry
 * @prev_entry: ������entry��ǰ���Ǹ�
 * @next_entry: ������entry�ĺ����Ǹ�
 *
 * ���ڵ�ʵ��list_add_tail��Ҫ��ʱ�����������������λ�ñ任�ˣ���list_add��Ҫ������ʱ����
 * �ʶ�listδ�㹻�˽ⲻҪ���Ե��ô˺��������޸Ĵ˺���
 */
#define __list_add(new_entry, prev_entry, next_entry) \
do { \
    (next_entry)->prev = (new_entry); \
    (new_entry)->next = (next_entry); \
    (new_entry)->prev = (prev_entry); \
    (prev_entry)->next = (new_entry); \
} while (0)

/**
 * ͷ�巨������entry
 * @new_entry: new entry to be added
 * @head: ͷentry
 */
#define list_add(new_entry, head) \
    __list_add((new_entry), (head), (head)->next);

/**
 * β�巨������entry
 * @new_entry: new entry to be added
 * @head: list head to add it before
 */
#define list_add_tail(new_entry, head) \
do { \
    struct list_head *prev = (head)->prev; \
    __list_add((new_entry), prev, (head)); \
} while (0)

/**
 * ������ɾ��prev_entry��next_entry֮���entry
 * @prev_entry: ��Ҫɾ����entry��ǰ���Ǹ�entry
 * @next_entry: ��Ҫɾ����entry�ĺ����Ǹ�entry
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
#define __list_del(prev_entry, next_entry) \
do { \
    (next_entry)->prev = (prev_entry); \
    (prev_entry)->next = (next_entry); \
} while (0)

/**
 * ɾ��һ��entry
 * @entry: Ҫɾ����entry
 *
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
#define list_del(entry) \
do { \
    __list_del((entry)->prev, (entry)->next); \
    (entry)->next = LIST_POISON1; \
    (entry)->prev = LIST_POISON2; \
} while (0)

/**
 * �ж��Ƿ�Ϊ������������ֻ��ͷ���
 * @head: the list to test.
 */
#define list_empty(head) \
    ((head)->next == (head))

/**
 * ��С�ĵ��ж������Ƿ�Ϊ��
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
#define list_empty_careful(head) \
    ((head)->next == (head)) && ((head)->next == (head)->prev)

/**
 * ��ȡ������entry�Ľṹ��ָ��
 * @entry:   �����ڽṹ���е�entry
 * @type:    Ҫ��ȡ�Ľṹ�������
 * @member:  entry�ڽṹ���еı�ʶ��
 *
 * old code: container_of(entry, type, member)
 */
#define list_entry(entry, type, member) \
    ((type *)((char *)(entry) - (unsigned long)(&((type *)0)->member)))

/**
 * �������������ڱ������޸�����
 * @pos:    ���������α��entry
 * @head:   the head of your list.
 */
#define list_for_each(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/**
 * ��ȫ�ر�������֧�ֱ������޸�����
 * @pos:        the &struct list_head to use as a loop cursor.
 * @pos_tmp:    ��Ϊ��ʱ�����ռ��entry���ܻ���posǰ����̽һ��
 * @head:       the head for your list.
 */
#define list_for_each_safe(pos, pos_tmp, head) \
    for ((pos) = (head)->next, (pos_tmp) = (pos)->next; \
        (pos) != (head); \
        (pos) = (pos_tmp), (pos_tmp) = (pos)->next)

/**
 * ѭ����������ڵ㣬�����ڱ������޸�����
 * @pos:        ����entry�Ľڵ�ṹ���������α�ڵ�
 * @head:       the head of your list.
 * @member:     �ڽڵ�ṹ��entry�ı�ʶ��
 */
#define list_for_each_entry(pos, head, type, member)  \
    for ((pos) = list_entry((head)->next, type, member);  \
         &(pos)->member != (head); \
         (pos) = list_entry((pos)->member.next, type, member))

/**
 * �԰�ȫ�ķ�ʽѭ����������ڵ㣬֧�ֱ������޸�����
 * @pos:        the type * to use as a loop cursor.
 * @pos_tmp:       ��Ϊ��ʱ�����ռ�Ľڵ㣬�ܻ���posǰ����̽һ��
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, pos_tmp, head, type, member)     \
    for ((pos) = list_entry((head)->next, type, member),    \
        (pos_tmp) = list_entry((pos)->member.next, type, member); \
         &(pos)->member != (head);                     \
         (pos) = (pos_tmp), \
         (pos_tmp) = list_entry((pos_tmp)->member.next, type, member))

/**
 * ���µ�entry����ɵ�entry�������е�λ��
 * @old_entry : the element to be replaced
 * @new_entry : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
#define list_replace(old_entry, new_entry) \
do { \
    (new_entry)->next = (old_entry)->next; \
    (new_entry)->next->prev = (new_entry); \
    (new_entry)->prev = (old_entry)->prev; \
    (new_entry)->prev->next = (new_entry); \
} while (0)

/**
 * ���µ�entry����ɵ�entry�������е�λ�ã������³�ʼ���ɵ�entry
 * @old_entry : the element to be replaced
 * @new_entry : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
#define list_replace_init(old_entry, new_entry) \
do { \
    list_replace((old_entry), (new_entry)); \
    INIT_LIST_HEAD(old_entry); \
} while (0)

/**
 * ɾ��entry�����³�ʼ����
 * @entry: the element to delete from the list.
 */
#define list_del_init(entry) \
do { \
    __list_del((entry)->prev, (entry)->next); \
    INIT_LIST_HEAD(entry); \
} while (0)

/**
 * ��һ��entry��һ��������ɾ��������ͷ�巨������һ������
 * @entry:  the entry to move
 * @head:   the head that will precede our entry
 */
#define list_move(entry, head) \
do { \
    __list_del((entry)->prev, (entry)->next); \
    list_add((entry), (head)); \
} while (0)

/**
 * ��һ��entry��һ��������ɾ��������β�巨������һ������
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
#define list_move_tail(entry, head) \
do { \
    __list_del((entry)->prev, (entry)->next); \
    list_add_tail((entry), (head)); \
} while (0)

/**
 * �ж�һ��entry�Ƿ��Ǳ�β
 * @entry: the entry to test
 * @head: the head of the list
 */
#define list_is_last(entry, head) \
    ((entry)->next == head)

/**
 * ��ȡ����ĵ�һ���ڵ�
 * @head:    the list head to take the element from.
 * @type:    the type of the struct this is embedded in.
 * @member:  the name of the list_struct within the struct.
 */
#define list_first_entry(head, type, member) \
    (list_empty(head) ? NULL : list_entry((head)->next, type, member))

/**
 * ��ȡ��������һ���ڵ�
 * @head:    the list head to take the element from.
 * @type:    the type of the struct this is embedded in.
 * @member:  the name of the list_struct within the struct.
 */
#define list_last_entry(head, type, member) \
    (list_empty(head) ? NULL : list_entry((head)->prev, type, member))

/**
 * �ж������Ƿ�ֻ��һ��entry
 * @head: the list to test.
 */
#define list_is_singular(head) \
    (!list_empty(head) && ((head)->next == (head)->prev))

/**
 * ��ԭ��head�����еĵ�һ��entry�����entry֮���entry�ƶ���new_head����
 * @head:       ԭ����
 * @entry:      ��Ҫ�����������һ��entry��Ӧ��head�����е�һ��entry
 * @new_head:   ���ձ�������entry������Ӧ����һ�������������������ݶ�ʧ����
 */
#define __list_cut_position(new_head, head, entry) \
do { \
    struct list_head *new_first = entry->next; \
    (new_head)->next = (head)->next; \
    (new_head)->next->prev = (new_head); \
    (new_head)->prev = (entry); \
    (entry)->next = (new_head); \
    (head)->next = new_first; \
    new_first->prev = (head); \
} while (0)

/**
 * �ѱ�ֽ����������ԭ��head�����еĵ�һ��entry�����entry֮���entry�ƶ���new_head����
 * @new_head:   a new list to add all removed entries
 * @head:       a list with entries
 * @entry:      an entry within head, could be the head itself
 *    and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 */
#define list_cut_position(new_head, head, entry) \
do { \
    if (list_empty(head)) { \
        /* retrun here */ \
    } else if (list_is_singular(head) && ((head)->next != (entry) && (head) != (entry))) { \
        /* retrun here */ \
    } else if ((entry) == (head)) { \
        INIT_LIST_HEAD(new_head); \
    } else { \
        __list_cut_position((new_head), (head), (entry)); \
    } \
} while (0)

/**
 * ������������һ������嵽��һ�������head_prev��head_next֮��
 * @new_head:   ��Ҫ���������
 * @head_prev:  �����������λ��ǰһ��entry
 * @head_next:  �����������λ�ú�һ��entry
 */
#define __list_splice(new_head, head_prev, head_next_) \
do { \
    struct list_head *first = (new_head)->next; \
    struct list_head *last = (new_head)->prev; \
    struct list_head *head_next = head_next_; \
    first->prev = (head_prev); \
    (head_prev)->next = first; \
    last->next = (head_next); \
    (head_next)->prev = last; \
} while (0)

/**
 * �ϲ���������һ������嵽��һ�������ͷ��
 * @new_head:   ��Ҫ���������
 * @head:       ��������������γ��µĸ���������
 */
#define list_splice(new_head, head) \
do { \
    if (!list_empty(new_head)) { \
        __list_splice((new_head), (head), head_next); \
    } \
} while (0)

/**
 * ������������һ������嵽��һ�������β��
 * @new_head:   the new list to add.
 * @head:       the place to add it in the first list.
 */
#define list_splice_tail(new_head, head) \
do { \
    if (!list_empty(new_head)) { \
        __list_splice((new_head), (head)->prev, (head)); \
    } \
} while (0)

/**
 * ͷ�巨�����������������³�ʼ����յ�����
 * @new_head:   the new list to add.
 * @head:       the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
#define list_splice_init(new_head, head) \
do { \
    if (!list_empty(new_head)) { \
        __list_splice((new_head), head, (head)->next); \
        INIT_LIST_HEAD(new_head); \
    } \
} while (0)

/**
 * β�巨�����������������³�ʼ����յ�����
 * @list:       the new list to add.
 * @head:       the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
#define list_splice_tail_init(new_head, head) \
do { \
    if (!list_empty(new_head)) { \
        __list_splice((new_head), (head)->prev, head); \
        INIT_LIST_HEAD(new_head); \
    } \
} while (0)

/**
 * �������������ڱ������޸�����
 * @pos:        the &struct list_head to use as a loop cursor.
 * @head:       the head for your list.
 *
 * This variant differs from list_for_each() in that it's the
 * simplest possible list iteration code, no prefetching is done.
 * Use this for code that knows the list to be very short (empty
 * or 1 entry) most of the time.
 *
 * ���ĵ�����ǰ���__list_for_each()��ȫһ������Ϊ�����ӿ�
 */
#define __list_for_each(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/**
 * ��������������ɱ������޸�����
 * @pos:        the &struct list_head to use as a loop cursor.
 * @head:       the head for your list.
 */
#define list_for_each_prev(pos, head) \
    for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)

/**
 * ��ȫ�ط����������֧�ֱ������޸�����
 * @pos:        the &struct list_head to use as a loop cursor.
 * @pos_tmp:    another &struct list_head to use as temporary storage
 * @head:       the head for your list.
 */
#define list_for_each_prev_safe(pos, pos_tmp, head) \
    for ((pos) = (head)->prev, (pos_tmp) = (pos)->prev; \
         (pos) != (head); \
         (pos) = (pos_tmp), (pos_tmp) = (pos)->prev)

/**
 * ѭ������������ڵ㣬�����ڱ������޸�����
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @type:   the type of pos
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_reverse(pos, head, type, member)  \
    for ((pos) = list_entry((head)->prev, type, member); \
         &(pos)->member != (head);  \
         (pos) = list_entry((pos)->member.prev, type, member))

/**
 * ׼��һ���ڵ㣬����Ϊlist_for_each_entry_continue�Ⱥ��������pos
 * @pos:    the type * to use as a start point
 * @head:   the head of the list
 * @type:   the type of pos
 * @member: the name of the list_struct within the struct.
 *
 * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
 */
#define list_prepare_entry(pos, head, type, member) \
    ((pos) ? (pos) : list_entry((head), type, member))

/**
 * ��pos��Ϊͷ��������������ڵ㣬����pos����һ��λ�ÿ�ʼ����
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @type:   the type of pos
 * @member: the name of the list_struct within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define list_for_each_entry_continue(pos, head, type, member) \
    for ((pos) = list_entry((pos)->member.next, type, member); \
         &(pos)->member != (head); \
         (pos) = list_entry((pos)->member.next, type, member))

/**
 * ��pos��Ϊͷ�����������������ڵ�
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @type:       the type of pos
 * @member:     the name of the list_struct within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define list_for_each_entry_continue_reverse(pos, head, type, member)        \
    for ((pos) = list_entry((pos)->member.prev, type, member);    \
         &(pos)->member != (head);    \
         (pos) = list_entry((pos)->member.prev, type, member))

/**
 * ��posλ������������ڵ�
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 @ @type:       the type of pos
 * @member:     the name of the list_struct within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define list_for_each_entry_from(pos, head, type, member)             \
    for (; &(pos)->member != (head);    \
         (pos) = list_entry((pos)->member.next, type, member))

/**
 * ��pos��Ϊͷ����԰�ȫ�ķ�ʽ������������ڵ㣬����pos����һ��λ�ÿ�ʼ����
 * @pos:        the type * to use as a loop cursor.
 * @pos_tmp:    another type * to use as temporary storage
 * @head:       the head for your list.
 * @type:       the type of pos
 * @member:     the name of the list_struct within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define list_for_each_entry_safe_continue(pos, pos_tmp, head, type, member)     \
    for ((pos) = list_entry((pos)->member.next, type, member),         \
        (pos_tmp) = list_entry((pos)->member.next, type, member);        \
         &(pos)->member != (head);  \
         (pos) = (pos_tmp), \
         (pos_tmp) = list_entry((pos_tmp)->member.next, type, member))

/**
 * ��posλ�����԰�ȫ�ķ�ʽ���������ڵ�
 * @pos:        the type * to use as a loop cursor.
 * @pos_tmp:    another type * to use as temporary storage
 * @head:       the head for your list.
 * @type:       the type of pos
 * @member:     the name of the list_struct within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define list_for_each_entry_safe_from(pos, pos_tmp, head, type, member)             \
    for ((pos_tmp) = list_entry((pos)->member.next, type, member);        \
         &(pos)->member != (head);   \
         (pos) = (pos_tmp), \
         (pos_tmp) = list_entry((pos_tmp)->member.next, type, member))

/**
 * �԰�ȫ�ķ�ʽ��������ڵ�
 * @pos:        the type * to use as a loop cursor.
 * @pos_tmp:    another type * to use as temporary storage
 * @head:       the head for your list.
 * @type:       the type of pos
 * @member:     the name of the list_struct within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define list_for_each_entry_safe_reverse(pos, pos_tmp, head, type, member)        \
    for ((pos) = list_entry((head)->prev, type, member),    \
        (pos_tmp) = list_entry((pos)->member.prev, type, member);    \
         &(pos)->member != (head);                     \
         (pos) = (pos_tmp), \
         (pos_tmp) = list_entry((pos_tmp)->member.prev, type, member))

#endif /* _LINUX_LIST_H_ */

