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
// 进行一趟快排,并返回分界的基准
int partition(TreeVGNode *array, int left, int right)
{
	TreeVGNode x = array[left]; // 找基准点，之后array[left]相当空单元
	/* 或者使用下面的改进代码,原理和上面一样，只不过代码少点 */
	int p_left = left;
	int p_right = right + 1;

	// 找分界点，
	for (;;) {
		while (lessTreeVerify(array[++p_left], x) && p_left < right)
			; //left-->right find one bigger than x
		while (!lessTreeVerify(array[--p_right], x)) 
			; //right-->left find one smaller than x
		if (p_left >= p_right)
			break;
		Swap(array[p_left], array[p_right]);//交换后左边小，右边大
	} // end for 

	// 找到的数 array[p_right] 一定 <= x，found one middle position for x
	array[left] = array[p_right];
	array[p_right] = x; // insert to middle position 

	return p_right; //返回分界下标
}

void quikSort(TreeVGNode *array, int pos_start, int pos_end)// pos_start,pos_end为下标
{
	if (pos_start < pos_end)
	{
		int partition_pos;
		partition_pos = partition(array, pos_start, pos_end);//一趟快排，获得基准点
		quikSort(array, pos_start, partition_pos - 1); //递归分解，对左边进行快排
		quikSort(array, partition_pos + 1, pos_end); //递归分解，对右边进行快排
	}
}
/* ------------------------------------------------------------------------ */
/* 我们容易发现，快排算法的性能取决于划分的对称性。通过修改partition函数可以设计出采用
* 随机选择策略的快排算法。也就是说找基准时从array[left,right]中随机找一个作为划分的基准
* 而不是只以第一个[left]作为基准。
*/
//随机生成某范围的随机整数,调用前用srand设置种子
int range_random(int start, int end)
{
	return   (start + rand() % (end - start + 1));
}

int Random_part(TreeVGNode *array, int left, int right)
{
	int rd = range_random(left, right);// 从left到right中取随机下标
	Swap(array[rd], array[left]);
	return partition(array, left, right);
}

void random_quikSort(TreeVGNode *array, int pos_start, int pos_end)// pos_start,pos_end为下标
{
	if (pos_start < pos_end) 
	{
		int partition_pos;
		partition_pos = Random_part(array, pos_start, pos_end);//一趟快排，获得基准点

		random_quikSort(array, pos_start, partition_pos - 1); //递归分解，对左边进行快排
		random_quikSort(array, partition_pos + 1, pos_end); //递归分解，对右边进行快排
	}
}

#endif