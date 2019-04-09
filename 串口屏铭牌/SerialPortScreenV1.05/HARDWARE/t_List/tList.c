#include "app.h"


/**********************************************************************************************************
** Function name        :   tNodeInit
** Descriptions         :   ��ʼ���������
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void tNodeInit (tNode * node)
{
    node->nextNode = node;
    node->preNode = node;
}

// �����Ǽ򻯴����д��ӵĺ�
#define firstNode   headNode.nextNode
#define lastNode    headNode.preNode

/**********************************************************************************************************
** Function name        :   tListInit
** Descriptions         :   �����ʼ��
** parameters           :   ��
** Returned value       :   ��
***********************************************************************************************************/
void tListInit (tList * list)
{
	list->firstNode = &(list->headNode);
    list->lastNode = &(list->headNode);
    list->nodeCount = 0;
}

/**********************************************************************************************************
** Function name        :   tListCount
** Descriptions         :   ���������н�������
** parameters           :   ��
** Returned value       :   �������
***********************************************************************************************************/
uint32_t tListCount (tList * list)
{
	return list->nodeCount;
}

/**********************************************************************************************************
** Function name        :   tListFirst
** Descriptions         :   ����������׸����
** parameters           :   list ��ѯ������
** Returned value       :   �׸���㣬�������Ϊ�գ��򷵻�0
***********************************************************************************************************/
tNode * tListFirst (tList * list)
{
    tNode * node = (tNode *)0;
	
	if (list->nodeCount != 0) 
	{
		node = list->firstNode;
	}    
    return  node;
}

/**********************************************************************************************************
** Function name        :   tListLast
** Descriptions         :   ������������һ�����
** parameters           :   list ��ѯ������
** Returned value       :   ���Ľ�㣬�������Ϊ�գ��򷵻�0
***********************************************************************************************************/
tNode * tListLast (tList * list)
{
    tNode * node = (tNode *)0;
	
	if (list->nodeCount != 0) 
	{
		node = list->lastNode;
	}    
    return  node;
}

/**********************************************************************************************************
** Function name        :   tListPre
** Descriptions         :   ����������ָ������ǰһ���
** parameters           :   list ��ѯ������
** parameters           :   node �ο����
** Returned value       :   ǰһ����㣬������û��ǰ��㣨����Ϊ�գ����򷵻�0
***********************************************************************************************************/
tNode * tListPre (tList * list, tNode * node)
{
	if (node->preNode == node) 
	{
		return (tNode *)0;
	} 
	else 
	{
		return node->preNode;
	}
}

/**********************************************************************************************************
** Function name        :   tListnextNode
** Descriptions         :   ����������ָ�����ĺ�һ���
** parameters           :   list ��ѯ������
** parameters           :   node �ο����
** Returned value       :   ��һ����㣬������û��ǰ��㣨����Ϊ�գ����򷵻�0
***********************************************************************************************************/
tNode * tListNext (tList * list, tNode * node)
{
	if (node->nextNode == node) 
	{
		return (tNode *)0;
	}
	else 
	{
		return node->nextNode;
	}
}

/**********************************************************************************************************
** Function name        :   tListRemoveAll
** Descriptions         :   �Ƴ������е����н��
** parameters           :   list ����յ�����
** Returned value       :   ��
***********************************************************************************************************/
void tListRemoveAll (tList * list)
{
    uint32_t count;
    tNode * nextNode;
        
    // �������еĽ��
	  nextNode = list->firstNode;
    for (count = list->nodeCount; count != 0; count-- )
    {
    	// �ȼ�¼�µ�ǰ��㣬����һ�����
    	// �����¼��һ���λ�ã���Ϊ�ں���Ĵ����е�ǰ����next�ᱻ����
        tNode * currentNode = nextNode;
        nextNode = nextNode->nextNode;
        
        // ���ý���Լ�����Ϣ
        currentNode->nextNode = currentNode;
        currentNode->preNode = currentNode;
    }
    
    list->firstNode = &(list->headNode);
    list->lastNode = &(list->headNode);
    list->nodeCount = 0;
}

/**********************************************************************************************************
** Function name        :   tListAddFirst
** Descriptions         :   ��ָ�������ӵ������ͷ��
** parameters           :   list ����������
** parameters			:   node ������Ľ��
** Returned value       :   ��
***********************************************************************************************************/
void tListAddFirst (tList * list, tNode * node)    //#define firstNode   headNode.nextNode
                                                   //#define lastNode    headNode.preNode
{
    node->preNode = list->firstNode->preNode;
    node->nextNode = list->firstNode;

    list->firstNode->preNode = node;
    list->firstNode = node;
    list->nodeCount++;
}

/**********************************************************************************************************
** Function name        :   tListAddLast
** Descriptions         :   ��ָ�������ӵ������ĩβ
** parameters           :   list ����������
** parameters			:   node ������Ľ��
** Returned value       :   ��
***********************************************************************************************************/
void tListAddLast (tList * list, tNode * node)
{
	node->nextNode = &(list->headNode);
    node->preNode = list->lastNode;

    list->lastNode->nextNode = node;
    list->lastNode = node;
    list->nodeCount++;
}

/**********************************************************************************************************
** Function name        :   tListRemoveFirst
** Descriptions         :   �Ƴ������еĵ�1�����
** parameters           :   list ���Ƴ�����
** Returned value       :   �������Ϊ�գ�����0������Ļ������ص�1�����
***********************************************************************************************************/
tNode * tListRemoveFirst (tList * list) //#define firstNode   headNode.nextNode
                                        //#define lastNode    headNode.preNode
{
    tNode * node = (tNode *)0;

	if( list->nodeCount != 0 )
    {
        node = list->firstNode;

        node->nextNode->preNode = &(list->headNode);
        list->firstNode = node->nextNode;
        list->nodeCount--;
    }
    return  node;
}

/**********************************************************************************************************
** Function name        :   tListInsertAfter
** Descriptions         :   ��ָ���Ľ����뵽ĳ��������
** parameters           :   list 			�����������
** parameters           :   nodeAfter 		�ο����
** parameters           :   nodeToInsert 	������Ľṹ
** Returned value       :   ��
***********************************************************************************************************/
void tListInsertAfter (tList * list, tNode * nodeAfter, tNode * nodeToInsert)
{
    nodeToInsert->preNode = nodeAfter;
    nodeToInsert->nextNode = nodeAfter->nextNode;

    nodeAfter->nextNode->preNode = nodeToInsert;
    nodeAfter->nextNode = nodeToInsert;

    list->nodeCount++;
}

/**********************************************************************************************************
** Function name        :   tListRemove
** Descriptions         :   ��ָ�������������Ƴ�
** parameters           :   list 	���Ƴ�������
** parameters           :   node 	���Ƴ��Ľ��
** Returned value       :   ��
***********************************************************************************************************/
void tListRemove (tList * list, tNode * node)
{
    node->preNode->nextNode = node->nextNode;
    node->nextNode->preNode = node->preNode;
    list->nodeCount--;
}

