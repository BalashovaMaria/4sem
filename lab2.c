#include <stdio.h>
#include <stdlib.h>

struct Node {
   int data; 
   struct Node* next; 
};

//функция для создания первого элемента односвязного списка
struct Node* createLinkedList(int data) {
   struct Node* head = NULL;
   struct Node* temp = NULL;
   struct Node* p = NULL;

   temp = (struct Node*)malloc(sizeof(struct Node));

   temp->data = data;
   temp->next = NULL;

   if (head == NULL) { 
      head = temp;
   } else { 
      p = head;
      while (p->next != NULL) {
         p = p->next;
      }
      p->next = temp;
   }
   return head;
}

//функция для добавления элемента в конец списка
struct Node* addNode(struct Node* head, int data) {
   struct Node* temp = head;

   if (head == NULL) {
      head = createLinkedList(data);
   } else {
      while (temp->next != NULL) {
         temp = temp->next;
      }

      struct Node* newnode =
         (struct Node*)malloc(sizeof(struct Node));
      newnode->data = data;
      newnode->next = NULL;
      temp->next = newnode;
   }
   return head;
}

//функция для удаления последнего элемента из списка
struct Node* deleteNode(struct Node* head) {
   struct Node* temp = head;
   struct Node* prevnode = NULL;

   if (head == NULL) {
      printf("Список пуст");
   } else {
      while (temp->next != NULL) {
         prevnode = temp;
         temp = temp->next;
      }
      prevnode->next = NULL;
      free(temp);
   }
   return head;
}

//функция для просмотра списка
void display(struct Node* head) {
   struct Node* temp = NULL;

   if (head == NULL) {
      printf("Список пуст\n");
   } else {
      temp = head;
      while (temp != NULL) {
         printf("%d-->", temp->data);
         temp = temp->next;
      }
      printf("NULL\n");
   }
}

// функция для удаления всего списка
struct Node* deleteList(struct Node* head) {
   struct Node* temp = head;
   struct Node* prevnode = NULL;

   while (temp != NULL) {
      prevnode = temp;
      temp = temp->next;
      free(prevnode);
   }
   head = NULL;

   return head;
}

// функция где выполняем последовательный поиск элемента
int searchNode(struct Node* head, int searchkey) {
   int index = 1;
   struct Node* temp = head;
   while (temp != NULL) {
      if (temp->data == searchkey) {
         return index;
      }
      index++;
      temp = temp->next;
   }
   return -1;
}

int main() {
   struct Node* head = NULL;
   int choice, data, result;
   while (1) {
      printf("Выберите действие:\n");
      printf("1. Создать первый элемент односвязного списка\n");
      printf("2. Добавить элемент в конец списка\n");
      printf("3. Удалить элемент с конца списка\n");
      printf("4. Просмотреть список\n");
      printf("5. Удалить весь список\n");
      printf("6. Выполнить последовательный поиск элемента в списке\n");
      printf("7. Выйти из программы\n");
      int k = scanf("%d", &choice);
      if (k != 1) {
        fflush(stdin);
        printf("Значение команды должно быть  цифрой от 1 до 7");
        break;
        }

      switch (choice) {
          case 1:
            printf("Введите данные: ");
            scanf("%d", &data);
            head = createLinkedList(data);
            break;
         case 2:
            printf("Введите данные: ");
            scanf("%d", &data);
            head = addNode(head, data);
            break;
         case 3:
            head = deleteNode(head);
            break;
         case 4:
            display(head);
            break;
         case 5:
            head = deleteList(head);
            break;
         case 6:
            printf("Введите значение элемента, который хотите найти: ");
            scanf("%d", &data);
            result = searchNode(head, data);
            if (result == -1) {
               printf("Элемент не найден\n");
            } else {
               printf("Элемент найден на позиции %d\n", result);
            }
            break;
         case 7:
            exit(0);
         default:
            printf("Неверный выбор, попробуйте еще раз\n");
            break;
      }
      fflush(stdin);
   }
   return 0;
}
