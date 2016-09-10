#ifndef _QSORT_H
#define _QSORT_H
#include "stdafx.h"
#include "treeVerifyGraphNode.h"

typedef treeVerifyGraphNode * TreeVGNode;
bool lessTreeVerify(const TreeVGNode &a, const TreeVGNode &b)
{
	return (a->deep + a->labelCost) <= (b->deep + b->labelCost);
}
void Swap(TreeVGNode &x, TreeVGNode &y)
{
	TreeVGNode tmp;

	tmp = x;
	x = y;
	y = tmp;
}
// ����һ�˿���,�����طֽ�Ļ�׼
int partition(TreeVGNode *array, int left, int right)
{
	TreeVGNode x = array[left]; // �һ�׼�㣬֮��array[left]�൱�յ�Ԫ
	/* ����ʹ������ĸĽ�����,ԭ�������һ����ֻ���������ٵ� */
	int p_left = left;
	int p_right = right + 1;

	// �ҷֽ�㣬
	for (;;) {
		while (lessTreeVerify(array[++p_left], x) && p_left < right)
			; //left-->right find one bigger than x
		while (!lessTreeVerify(array[--p_right], x)) 
			; //right-->left find one smaller than x
		if (p_left >= p_right)
			break;
		Swap(array[p_left], array[p_right]);//���������С���ұߴ�
	} // end for 

	// �ҵ����� array[p_right] һ�� <= x��found one middle position for x
	array[left] = array[p_right];
	array[p_right] = x; // insert to middle position 

	return p_right; //���طֽ��±�
}

void quikSort(TreeVGNode *array, int pos_start, int pos_end)// pos_start,pos_endΪ�±�
{
	if (pos_start < pos_end)
	{
		int partition_pos;
		partition_pos = partition(array, pos_start, pos_end);//һ�˿��ţ���û�׼��
		quikSort(array, pos_start, partition_pos - 1); //�ݹ�ֽ⣬����߽��п���
		quikSort(array, partition_pos + 1, pos_end); //�ݹ�ֽ⣬���ұ߽��п���
	}
}
/* ------------------------------------------------------------------------ */
/* �������׷��֣������㷨������ȡ���ڻ��ֵĶԳ��ԡ�ͨ���޸�partition����������Ƴ�����
* ���ѡ����ԵĿ����㷨��Ҳ����˵�һ�׼ʱ��array[left,right]�������һ����Ϊ���ֵĻ�׼
* ������ֻ�Ե�һ��[left]��Ϊ��׼��
*/
//�������ĳ��Χ���������,����ǰ��srand��������
int range_random(int start, int end)
{
	return   (start + rand() % (end - start + 1));
}

int Random_part(TreeVGNode *array, int left, int right)
{
	int rd = range_random(left, right);// ��left��right��ȡ����±�
	Swap(array[rd], array[left]);
	return partition(array, left, right);
}

void random_quikSort(TreeVGNode *array, int pos_start, int pos_end)// pos_start,pos_endΪ�±�
{
	if (pos_start < pos_end) 
	{
		int partition_pos;
		partition_pos = Random_part(array, pos_start, pos_end);//һ�˿��ţ���û�׼��

		random_quikSort(array, pos_start, partition_pos - 1); //�ݹ�ֽ⣬����߽��п���
		random_quikSort(array, partition_pos + 1, pos_end); //�ݹ�ֽ⣬���ұ߽��п���
	}
}

#endif