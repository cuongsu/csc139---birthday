#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list_sort.h>
#include <linux/slab.h>

/* Used as a reference to the head of the list       */
static LIST_HEAD(birthday_list);

/* Linked list within struct that holds all the      *
 * attributes for birthday	                     */
struct birthday {	
	int day;
	int month;
	int year;
	struct list_head list;
	char* name;
};

struct birthday *ptr, *next;

/* Takes the month inside each element and changes the  *
 * int month to a char that is equivalent to its number */
char* setMonth(int month) {
	switch(month){
	case 1:
		return "January";
	case 2:
		return "February";
	case 3:
		return "March";
	case 4:
		return "April";
	case 5:
		return "May";
	case 6: 
		return "June";
	case 7:
		return "July";
	case 8:
		return "August";
	case 9:
		return "September";
	case 10:
		return "October";
	case 11:
		return "November";
	case 12:
		return "December";
	default:
		return "Not a valid month number.";
	}
}

/* Method to compare ages between elements in the linked lists.   *
 * It will return -1 if the A is older than B, or 1 if B is older *
 * If the birthdays are the same, function will return 0.         */
int ageCompare(void *ptr, struct list_head *cur, struct list_head *nxt) {
	struct birthday *bdayA = list_entry(cur, struct birthday, list);
	struct birthday *bdayB = list_entry(nxt, struct birthday, list);

	if(bdayA -> year < bdayB -> year) {
		return -1;
	} else if(bdayA -> year > bdayB -> year){
		return 1;
	} else {
		if(bdayA -> month < bdayB -> month) {
			return -1;
		} else if(bdayA -> month > bdayB -> month){
			return 1;
		} else {
			if(bdayA -> day < bdayB -> day) {
				return -1;
			} else if(bdayA -> day < bdayB -> day){ 
				return 1;
			} else {
				return 0;
			}
		}
	}
}

/* This function is called when the module is loaded. */
int simple_init(void) {
	struct birthday *alice;
	struct birthday *bob;
	struct birthday *mallory;
	struct birthday *nancy;
	struct birthday *kate;
		
        printk(KERN_INFO "Loading Module\n");

	alice = kmalloc(sizeof(*alice), GFP_KERNEL);
	alice -> name = "Alice";
	alice -> day = 9;
	alice -> month = 1;
	alice -> year = 1999;
	INIT_LIST_HEAD(&alice -> list);

	bob = kmalloc(sizeof(*bob), GFP_KERNEL);
	bob -> name = "Bob";
	bob -> day = 8;
	bob -> month = 3;
	bob -> year = 1978;
	INIT_LIST_HEAD(&bob -> list);

	mallory = kmalloc(sizeof(*mallory), GFP_KERNEL);
	mallory -> name = "Mallory";
	mallory -> day = 9;
	mallory -> month = 12;
	mallory -> year = 1958;
	INIT_LIST_HEAD(&mallory -> list);

	nancy = kmalloc(sizeof(*nancy), GFP_KERNEL);
	nancy -> name = "Nancy";
	nancy -> day = 9;
	nancy -> month = 6;
	nancy -> year = 2003;
	INIT_LIST_HEAD(&nancy -> list);

	kate = kmalloc(sizeof(*kate), GFP_KERNEL);
	kate -> name = "Kate";
	kate -> day = 8;
	kate -> month = 3;
	kate -> year = 1978;
	INIT_LIST_HEAD(&kate -> list);

	list_add_tail(&alice -> list, &birthday_list);
	list_add_tail(&bob -> list, &birthday_list);
	list_add_tail(&mallory -> list, &birthday_list);
	list_add_tail(&nancy -> list, &birthday_list);
	list_add_tail(&kate -> list, &birthday_list);

	printk("Unsorted list: \n");
	list_for_each_entry(ptr, &birthday_list,list) {
		printk("%s born on %s %d, %d\n", ptr -> name, 
			setMonth(ptr -> month), ptr -> day, ptr -> year);
	}
	
	list_sort(NULL, &birthday_list, ageCompare);

	printk("Sorted list: \n");
	list_for_each_entry(ptr, &birthday_list,list) {
		printk("%s born on %s %d, %d\n", ptr -> name, 
			setMonth(ptr -> month), ptr -> day, ptr -> year);
	}
        
	return 0; 
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	
	list_for_each_entry_safe(ptr, next, &birthday_list, list) {	
		printk("Deleting element: %s \n", ptr -> name);		
		list_del(&ptr -> list);
		kfree(ptr);
	}
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Birthday linked list sort");
MODULE_AUTHOR("Cuong Su");

