#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *sentinel = (node_t *)calloc(1, sizeof(node_t));
  sentinel->color = RBTREE_BLACK;
  sentinel->left = NULL;
  sentinel->right = NULL;
  sentinel->parent = NULL;

  p->nil = sentinel;
  p->root = sentinel;

  return p;
}

void rightRotate(rbtree *t, node_t *node)
{
  node_t *nl = node->left;
  node->left = nl->right;
  if (nl->right != t->nil)
  {
    nl->right->parent = node;
  }
  nl->parent = node->parent;
  if (node->parent == t->nil)
  {
    t->root = nl;
  }
  else if (node == node->parent->left)
  {
    node->parent->left = nl;
  }
  else
  {
    node->parent->right = nl;
  }
  nl->right = node;
  node->parent = nl;

  t->root->color = RBTREE_BLACK;
}

void leftRotate(rbtree *t, node_t *node)
{
  node_t *nr = node->right;
  node->right = nr->left;
  if (nr->left != t->nil)
  {
    nr->left->parent = node;
  }
  nr->parent = node->parent;
  if (node->parent == t->nil)
  {
    t->root = nr;
  }
  else if (node == node->parent->left)
  {
    node->parent->left = nr;
  }
  else
  {
    node->parent->right = nr;
  }
  nr->left = node;
  node->parent = nr;

  t->root->color = RBTREE_BLACK;
}

void deleteNodeOfTreeRecursive(rbtree *t, node_t *node)
{
  if (node->left != t->nil)
  {
    deleteNodeOfTreeRecursive(t, node->left);
  }
  if (node->right != t->nil)
  {
    deleteNodeOfTreeRecursive(t, node->right);
  }
  free(node);
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t->root == t->nil)
  {
    free(t->root);
  }
  else
  {
    deleteNodeOfTreeRecursive(t, t->root);
    free(t->nil);
  }

  free(t);
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->key = key;
  newNode->right = t->nil;
  newNode->left = t->nil;

  if (t->root == t->nil)
  {
    newNode->parent = t->nil;
    newNode->color = RBTREE_BLACK;
    t->root = newNode;
  }
  else
  {
    node_t *x = t->root;
    node_t *y = t->nil;

    while (x != t->nil)
    {

      y = x;

      if (newNode->key < x->key)
      {
        x = x->left;
      }
      else
      {
        x = x->right;
      }
    }

    newNode->parent = y;
    if (newNode->key < y->key)
    {
      y->left = newNode;
    }
    else
    {
      y->right = newNode;
    }
    newNode->color = RBTREE_RED;

    node_t *z = newNode;

    while (z->parent->color == RBTREE_RED)
    {
      if (z->parent->parent->left == z->parent)
      {
        y = z->parent->parent->right;
        if (y->color == RBTREE_RED)
        {
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->right)
          {
            z = z->parent;
            leftRotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;

          rightRotate(t, z->parent->parent);
        }
      }
      else
      {
        y = z->parent->parent->left;
        if (y->color == RBTREE_RED)
        {
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->left)
          {
            z = z->parent;
            rightRotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;

          leftRotate(t, z->parent->parent);
        }
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *curNode = t->root;

  while (curNode != t->nil)
  {
    if (curNode->key == key)
    {
      return curNode;
    }
    else if (curNode->key > key)
    {
      curNode = curNode->left;
    }
    else
    {
      curNode = curNode->right;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *node = t->root;

  if (node == t->nil)
  {
    return NULL;
  }

  while (node->left != t->nil)
  {
    node = node->left;
  }

  return node;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *node = t->root;

  if (node == t->nil)
  {
    return NULL;
  }

  while (node->right != t->nil)
  {
    node = node->right;
  }

  return node;
}

//  target 노드를 new 노드로 대체하기 위한 프로시저
void transplant(rbtree *t, node_t *new, node_t *target)
{
  //  타겟의 부모가 센티넬 노드인지 확인 -> 맞다면 타겟 노드는 기존 트리의 루트였으므로
  //  트리의 루트를 new 로 바꿔줌 => 이래서 센티넬이 좋구나
  if (target->parent == t->nil)
  {
    t->root = new;
  }
  else if (target == target->parent->right)
  {
    target->parent->right = new;
  }
  else
  {
    target->parent->left = new;
  }
  new->parent = target->parent;
}

void fixYou(rbtree *t, node_t *fixTarget)
{
  //  fix 과정에서 fixTarget 노드의 형제 노드를 추적해야 할 일이 많으므로, 전용 변수 선언
  node_t *brotherNode;
  //  fix (여분의 black 노드를 처리) 해야 할 대상 노드가 red 가 되어서 해당 노드의 color 만 변경하면
  //  상위 노드들의 bh 규칙성이 유지되거나, 여분의 black 노드가 필요한 위치가 root 까지 이동해서 규칙 위배 문제가 없을 때까지
  //  루프를 돌며 트리구조를 fix
  while (fixTarget->color == RBTREE_BLACK && fixTarget != t->root)
  {
    if (fixTarget == fixTarget->parent->right)
    {
      brotherNode = fixTarget->parent->left;
      if (brotherNode->color == RBTREE_RED)
      {
        brotherNode->color = brotherNode->parent->color;
        brotherNode->parent->color = RBTREE_RED;
        rightRotate(t, brotherNode->parent);
        continue;
      }
      else if (brotherNode->right->color == RBTREE_BLACK && brotherNode->left->color == RBTREE_BLACK)
      {
        brotherNode->color = RBTREE_RED;
        fixTarget = fixTarget->parent;
        continue;
      }
      else if (brotherNode->right->color == RBTREE_RED && brotherNode->left->color == RBTREE_BLACK)
      {
        brotherNode->right->color = brotherNode->color;
        brotherNode->color = RBTREE_RED;
        leftRotate(t, brotherNode);
        continue;
      }
      else
      {
        brotherNode->color = brotherNode->parent->color;
        brotherNode->parent->color = RBTREE_BLACK;
        brotherNode->left->color = RBTREE_BLACK;
        rightRotate(t, fixTarget->parent);
        break;
      }
    } else {
      brotherNode = fixTarget->parent->right;
      if (brotherNode->color == RBTREE_RED)
      {
        brotherNode->color = brotherNode->parent->color;
        brotherNode->parent->color = RBTREE_RED;
        leftRotate(t, brotherNode->parent);
        continue;
      }
      else if (brotherNode->left->color == RBTREE_BLACK && brotherNode->right->color == RBTREE_BLACK)
      {
        brotherNode->color = RBTREE_RED;
        fixTarget = fixTarget->parent;
        continue;
      }
      else if (brotherNode->left->color == RBTREE_RED && brotherNode->right->color == RBTREE_BLACK)
      {
        brotherNode->left->color = brotherNode->color;
        brotherNode->color = RBTREE_RED;
        rightRotate(t, brotherNode);
        continue;
      }
      else
      {
        brotherNode->color = brotherNode->parent->color;
        brotherNode->parent->color = RBTREE_BLACK;
        brotherNode->right->color = RBTREE_BLACK;
        leftRotate(t, fixTarget->parent);
        break;
      }
    }
  }
  fixTarget->color = RBTREE_BLACK;
  t->root->color = RBTREE_BLACK;
  return;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  //  위치를 이동하여 삭제될 노드를 대체할 노드 y
  node_t *y = p;
  //  이동한 y 를 대체할 노드 x
  node_t *x;
  //  y가 삭제될 노드를 대체하게 될 때 x의 색을 물려받으므로 y의 색은 사라짐.
  //  만약 사라지는 색이 흑색이면 rb 트리의 5번 규칙을 위반하게 되므로 후 처리를 해줘야 하고,
  //  그를 위해 y의 기존 색상을 저장해야 함.
  color_t OriginalColorOfY = y->color;

  //  아래의 두 조건문은 삭제 대상 노드 (이하 p) 의 자식이 하나 이하일 때의 처리를 수행
  //  왼쪽 자식만 있을 때 혹은 자식이 없을 때
  if (p->right == t->nil)
  {
    //  x는 nil 혹은 왼쪽 자식 노드
    x = p->left;
    //  p의 위치를 p->left 가 대체하도록 transplant 호출
    transplant(t, p->left, p);
  }
  //  오른쪽 자식만 있을 때
  else if (p->left == t->nil)
  {
    //  위와 동일
    x = p->right;
    transplant(t, p->right, p);
  }
  //  아래의 분기에서는 p가 두 개의 자식을 가질 때의 처리를 수행
  else
  {
    //  레드블랙 트리에서 자식이 두 개인 노드를 삭제할 시 해당 위치를 오른쪽 서브트리의 가장 왼쪽 노드
    //  즉, 후임자에게 대체시킴. (항상 그런지 확실하지는 않음)
    y = p->right;
    //  y의 왼쪽 자식이 nil 이 아닐 때 까지 while 루프를 돌아서 가장 왼쪽 노드를 y에 할당
    while (y->left != t->nil)
    {
      y = y->left;
    }
    //  새로 선정한 y의 기존 색 저장
    OriginalColorOfY = y->color;
    //  y를 대체할 x 또한 다시 선정 (후임자 노드는 절대 왼쪽 자식을 갖지 않으므로 이 함수의 첫 if 문과 같이 처리해주면 됨)
    x = y->right;

    //  x, 즉 y->right 의 부모는 아래 transplant 함수 호출에서 변경될 예정이지만, 만약 y가 p의 자식인 경우라면
    //  그렇게 해서는 안됨. 왜나하면 p는 삭제 대상 노드인데, transplant 함수에 그대로 넣어서 돌리면 y->right 의 부모
    //  포인터가 p 를 가르키게 되기 때문임.
    if (y->parent == p)
    {
      //  그러니까 이런 상황에서는 그냥 x의 부모를 y로 해주면 됨. (사실 안해도 될 듯?)
      x->parent = y;
    }
    else
    {
      //  y가 p 의 자식이 아닐 때는 transplant 를 호출해서 y와 y->right (x) 의 위치를 바꿔줌
      transplant(t, y->right, y);
      //  그리고 자식들을 연결해 줘야 하는데 위의 조건, 그러니까 y가 p의 오른쪽 자식인 상황에서는
      //  y의 오른쪽 자식들을 재설정 해 줄 필요가 없으므로 현재 else 분기 내에서 처리
      y->right = p->right;
      y->right->parent = y;
    }
    //  마지막으로 p와 y의 위치를 교체
    transplant(t, y, p);
    //  p의 왼쪽 자식을 y의 왼쪽 자식으로 바꿔주고
    y->left = p->left;
    y->left->parent = y;
    //  color 는 p 의 색을 계승
    y->color = p->color;
  }

  //  하지만 트리에서 사라진 색 (y의 기존 색) 이 black 일 시 위에서 언급했듯 5번 속성을 위반하기 때문에
  //  그 처리를 해줘야 함
  if (OriginalColorOfY == RBTREE_BLACK)
  {
    //  트리에서 사라진 색이 black 인 경우에 총 3가지의 규칙 위배 가능성이 존재하는데,
    //  첫 번째로 y의 조상 노드들의 black height 의 균형 유지가 깨지게 되고, (5번)
    //  두 번째로는 새로 y의 자리를 대체하게 된 x가 red임과 동시에 y의 기존 부모가 red 이면 빨빨 사태가 발생하며, (4번)
    //  마지막으로 뭐 어떻게 해서 루트가 red 가 될 수 있다는데 이건 도저히 이해가 안됨.. 그냥 다시 black 으로 바꿔주면
    //  그만이니까 넘어감.
    //  고로 이 분기 내에서 검사하고 처리해줘야 할 경우는 첫 번째와 두 번째이고, 둘 모두 기존 y의 위치, 즉
    //  x 에서 발생되는 규칙 위반 상황이므로 재구성 함수의 인자로 x를 넘겨줌.
    //  GOGO~
    fixYou(t, x);
  }

  free(p);

  return 0;
}

void makeArrayRecursive(rbtree *t, node_t *node, key_t **arr, size_t n)
{
  if (node == t->nil)
  {
    return;
  }

  makeArrayRecursive(t, node->left, arr, n);
  *(*arr) = node->key;
  (*arr)++;
  makeArrayRecursive(t, node->right, arr, n);
  return;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  makeArrayRecursive(t, t->root, &arr, n);
  return 0;
}
