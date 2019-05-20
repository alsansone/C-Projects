#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CYCLES 20 /* max number of cycles */
#define MAN 'M' /* signifies a man */
#define WOMAN 'W' /* signifies a woman */


int bathroom = 0; // number of men or women in bathroom
int bathroomSize = 3; // amount of people allowed in bathroom at one time
int manCount = 0; // counts number of men in the bathroom currently
int womanCount = 0; // counts number of women in bathroom currently
int bathroomLock = 1; // 0 = unlocked, 1 = locked
int manLock = 1;// 0 = unlocked, 1 = locked
int womanLock = 1; // 0 = unlocked, 1 = locked
int queueLength = 0; // holds number of people in line


/************************************************************/
/* Queue Implementation using Linked List */

/* Node constructor */
struct Node
{
    char gender; // holds gender of person
    struct Node* next; // pointer to next node in list
};

/* pointers to head and tail of linked list */
struct Node* head = NULL;
struct Node* tail = NULL;

/* add a person to the queue */
void enqueue(char g)
{
    // create new person node */
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->gender = g;
    temp->next = NULL;

    // check if queue is empty and update queue*/
    if (head == NULL && tail == NULL) {
        head = tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }

    queueLength++; // increment queue length
}

/* removes a person from the queue */
void dequeue()
{
    /* update linked list head and tail pointers */
    if (head == tail) {
        head = tail = NULL;
    } else {
        head = head->next;
    }

    queueLength--; // decrement queue length
}
/* check person gender at front of queue */
char peek()
{
    if (queueLength > 0) {
        return head->gender;
    }
}

/* displays the number of men and women in the queue */
void displayQueue()
{
    int countMen = 0;
    int countWomen = 0;

    struct Node* temp = head; // temp pointer to hold head of list

    printf("The order of the queue: ");

    /* iterate through the linked list and print gender */
    while (temp != NULL) {
        if (temp->gender == MAN) {
            countMen++;
            printf("Man ");
        }

        if (temp->gender == WOMAN) {
            countWomen++;
            printf("Woman ");
        }

        temp = temp->next; // assign temp to next node in list
    }

    /* prints # of men and women waiting in queue */
    printf("\nMen waiting: %d Women waiting: %d\n", countMen, countWomen);
}
/************************************************************/

/* LOCK METHODS */
void down(int *lock)
{
    (*lock)--;
}

void up(int *lock)
{
    (*lock)++;
}

/************************************************************/
/* update locks and add man to the bathroom */
void man_enters()
{
    down(&manLock); // unlock man
    down(&bathroomLock); // unlock bathroom
    bathroom++; // increment bathroom count
    manCount++; // increment # of men in bathroom
    up(&bathroomLock); // lock bathroom
}
/* checks if a man can enter the bathroom or needs to wait */
void man_wants_to_enter()
{
    printf("A man wants to enter the bathroom\n");
    if (bathroom < bathroomSize && womanLock == 1 && queueLength == 0) {
        man_enters(); // add man to bathroom
        printf("A man has entered the bathroom\n");
    } else {
        enqueue(MAN); // add man to queue
        printf("A man has entered the queue\n");
    }
}
/* update locks and add woman to the bathroom */
void woman_enters()
{
    down(&womanLock); // unlock woman
    down(&bathroomLock); // unlock bathroom
    bathroom++; // increment bathroom count
    womanCount++; // increment # of women in bathroom
    up(&bathroomLock); // lock bathroom
}
/* checks if a woman can enter the bathroom or needs to wait */
void woman_wants_to_enter()
{
    printf("A woman wants to enter the bathroom\n");
    if (bathroom < bathroomSize && manLock == 1 && queueLength == 0) {
        woman_enters(); // woman enters bathroom
        printf("A woman has entered the bathroom\n");
    } else {
        enqueue(WOMAN); // add woman to queue
        printf("A woman has entered the queue\n");
    }
}

/* checks queue first and updates bathroom then creates a new person */
void open_bathroom()
{
    /* is there somebody in line and bathroom not full */
    if (queueLength > 0 && bathroom < bathroomSize) {
        char firstGender = peek(); // gender of first in queue

        /* is woman locked and first in line a man */
        if (womanLock == 1 && firstGender == MAN) {
            /* while bathroom isn't full and queue has people in it and that person is a man */
            while (bathroom < bathroomSize && queueLength > 0 && firstGender == MAN) {
                dequeue(); // remove first person in line
                man_enters(); // man enters bathroom
                printf("A man has moved from the queue to the bathroom\n");
                firstGender = peek(); // check next in line
            }
        }
        /* is man locked and first in line a woman */
        if (manLock == 1 && firstGender == WOMAN) {
            /* while bathroom isn't full and queue has people in it and that person is a woman */
            while (bathroom < bathroomSize && queueLength > 0 && firstGender == WOMAN) {
                dequeue(); //remove first in line
                woman_enters(); // woman enters bathroom
                printf("A woman has moved from the queue to the bathroom\n");
                firstGender = peek(); // check next in line
            }
        }
    }

    int person = rand() % 2; // generate random number 0 or 1

    /* 0 == MAN, 1 == WOMAN */
    if (person == 0) {
        man_wants_to_enter();
    } else {
        woman_wants_to_enter();
    }
}
/* remove a man from the bathroom */
void man_leaves()
{
    down(&bathroomLock); // unlock bathroom
    bathroom--; // decrement bathroom
    manCount--; // decrement # of men in bathroom
    up(&bathroomLock); // lock bathroom
    up(&manLock); // lock man
    printf("A man has left the bathroom\n");
}
/* remove a woman from the bathroom */
void woman_leaves()
{
    down(&bathroomLock); // unlock bathroom
    bathroom--; // decrement bathroom
    womanCount--; // decrement # of woman in bathroom
    up(&bathroomLock); // lock bathroom
    up(&womanLock); // lock woman
    printf("A woman has left the bathroom\n");
}
/* removes people from the bathroom */
void empty_bathroom()
{
    /* does bathroom have people in it */
    if (bathroom > 0) {
        int peopleLeaving = (rand() % bathroom) + 1; // decides how many people are leaving the bathroom
        printf("Number of people leaving bathroom: %d\n", peopleLeaving);
        /* while # of people to leave is greater than 0 */
        while (peopleLeaving > 0) {
            /* are there men or women in the bathroom currently */
            if (manCount > 0) {
                man_leaves(); // man leaves
            } else {
                woman_leaves(); // woman leaves
            }
            peopleLeaving--; // decrement # of people to kick out
        }
    }
}
/* resets all variables and pointers */
void bathroom_closed()
{
    printf("The bathroom is now closed. Everyone must go\n");
    printf("%d people left the queue\n", queueLength);
    printf("%d %s left the bathroom\n", bathroom, (bathroom > 0) ? "people" : "person");
}

/* initialize the bathroom program */
void initialize_bathroom()
{
    /* loops through 20 times */
    int i;
    for (i = 0; i < CYCLES; i++) {
        printf("Cycle : %d\n", i+1);

        printf("Bathroom Status: ");
        /* print # of men/women in bathroom */
        if (manCount > 0) {
            printf("%d %s", manCount, (manCount > 1) ? "Men\n" : "Man\n");
        } else if (womanCount > 0) {
            printf("%d %s", womanCount, (womanCount > 1) ? "Women\n" : "Woman\n");
        } else {
            printf("Empty\n");
        }

        /* empty bathroom every even cycle */
        if (i % 2 == 0) {
            empty_bathroom();
        }

        open_bathroom(); // update bathroom and generate a person

        displayQueue(); // display queue stats

    }

    bathroom_closed(); // kick everyone out
}

/* Main Method */
main()
{

    srand(time(NULL)); // set seed for randomization
    initialize_bathroom(); // run bathroom program
}
