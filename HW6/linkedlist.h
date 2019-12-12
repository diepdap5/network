/*
Khai báo node
*/
typedef struct node node;
typedef struct node {
	elementtype element;
	node *next;
}node;
/*
Một node gồm root, cur,prev, tail
*/

typedef struct {
  node * root;
  node * cur;
  node * prev;
  node * tail;
} singleList;
/*
Các hàm khởi tạo
*/

void   createSingleList(singleList * list);
node * makeNewNode(elementtype e);
/*
Thêm 1 node
*/

node * insertAfter(singleList * list, elementtype e);
node * insertBegin(singleList * list, elementtype e);
node * insertEnd(singleList * list, elementtype e);
node * insertAtPosition(singleList * list, elementtype e, int n);

/*
Xóa node
*/

node * deleteBegin(singleList * list);
node * deleteEnd(singleList * list);
node * deleteAtPosition(singleList * list, int n);
node * deleteSingleList(singleList * list);

/*
In ra màn hình tất cả list
*/
void   printSingleList(singleList  list);

/*
Tất cả node
*/
int    totalSingleList(singleList list);


/*
*
*
Chi tiết các hàm
*
*
*/

// Tạo danh sách mới

void createSingleList(singleList * list)
{
  (*list).root = (*list).prev = (*list).cur = (*list).tail  = NULL;
}

// Tạo node mới

node * makeNewNode(elementtype e)
{
  node * new = (node *)malloc(sizeof(node));
  new->element = e;
  new->next = NULL;
  return new;
}

// Thêm vào cuối danh sách

node * insertEnd(singleList *list,elementtype e)
{
	node * new = makeNewNode(e);
	if((*list).root == NULL)
	{
      (*list).root = (*list).tail = new;
	}
	else
	{
		(*list).tail->next = new;
		(*list).tail = new;
	}
	return (*list).tail;
}

// Thêm vào đầu danh sách

node * insertBegin(singleList * list, elementtype e)
{
  node * new = makeNewNode(e);
  if ((*list).root == NULL)
    {
      (*list).root = (*list).tail = new;
    }
  else
    {
      new->next=(*list).root;
      (*list).root = new;
    }
  return (*list).root;
}

// Thêm vào vị trí
node * insertAtPosition(singleList * list,elementtype e,int n)
{
  node *new = makeNewNode(e);
  if((*list).root == NULL)
    {
      (*list).root = (*list).tail = (*list).cur = new;
	}
  else
    {
      if(n <= 1)
        {
          insertBegin(list,new->element);
          return (*list).cur;
        }
      if(n > totalSingleList(*list))
        {
          insertEnd(list,new->element);
          return (*list).cur;
        }
      else
        {
          (*list).cur = (*list).prev = (*list).root;
          int i = 1;
          while(((*list).cur->next != NULL)&&(i <= n-1))
            {
              i++;
              (*list).prev = (*list).cur;
              (*list).cur = (*list).cur->next;
            }
          new->next = (*list).cur;
          (*list).prev->next = (*list).cur = new;
        }
    }
  return (*list).cur;
}

// Xóa node đầu tiên

node * deleteBegin(singleList * list)
{
  if((*list).root != NULL)
	{
      node * new = (*list).root;
		(*list).root = (*list).root->next;
		free(new);
	}
  return (*list).root;
}

// Xóa node cuối cùng

node * deleteEnd(singleList *list)
{
  if((*list).root != NULL)
    {
       (*list).cur = (*list).prev = (*list).root;
          while(((*list).cur->next != NULL))
            {
              (*list).prev =( *list).cur;
              (*list).cur = (*list).cur->next;
            }
        node *new = (*list).cur;
		(*list).cur = (*list).cur->next;
		free(new);
        (*list).tail = (*list).prev;
        (*list).tail->next = NULL;
        return (*list).tail;
    }
}

// Xóa tất cả
node * deleteSingleList(singleList * list)
{
  while((*list).root != NULL) deleteBegin(list);
}

// Toàn bộ danh sách

int totalSingleList(singleList list)
{
  int i=0;
  list.cur = list.root;
	while(list.cur != NULL)
      {
        i++;
        list.cur = list.cur->next;
      }
    return i;
}
