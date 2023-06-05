#include <fcntl.h>
#include <stdio.h>
#include <sys/msg.h>

struct message {
    long type;
    char text[100];
} message_snd, message_rcv;

int main(void) {
    int option, priority;
    struct msqid_ds info;
    open("/tmp/queue", O_CREAT, 0777);
    key_t queueKey = ftok("/tmp/queue", 1);
    int queueId = msgget(queueKey, IPC_CREAT | 0777);

    while (1) {
        printf("Send 1 to add a message to the queue, 2 to remove the first "
               "message with a given priority or 0 to exit: ");
        scanf("%d", &option);
        switch (option) {
        case 0:
            return 0;
        case 1:
            printf("Add new message: ");
            scanf("%99s", message_snd.text);
            printf("Set a priority between 1 and 99: ");
            do {
                scanf("%ld", &message_snd.type);
            } while (message_snd.type < 1 || message_snd.type > 99);
            msgsnd(queueId, &message_snd, sizeof(message_snd.text), 0);
            break;
        case 2:
            msgctl(queueId, IPC_STAT, &info);
            if (info.msg_qnum == 0) {
                printf("No messages in the queue!\n");
                break;
            }
            printf("Set a priority and the first message with that priority "
                   "will be deleted: ");
            scanf("%d", &priority);
            if (msgrcv(queueId, &message_rcv, sizeof(message_rcv.text),
                       priority, IPC_NOWAIT) != -1)
                printf("Message eliminated: %s\n", message_rcv.text);
            else
                printf("No messages in the queue with a priority of %d!\n",
                       priority);
            break;
        default:
            printf("Invalid option\n");
        }
    }
}
