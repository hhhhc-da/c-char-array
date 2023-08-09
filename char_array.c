#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// 类型重定义部分

// v->v函数指针类型
typedef void (*pvFuncType)(void);
// 存储结构类型指针
typedef char *pDataType;

// 数组结构体类型
typedef struct
{
	// 存储总量
	size_t ulSize;
	// 存储头指针
	pDataType pxStorage;
	// 存储尾指针
	pDataType pxDataHead;
} Array;

// 添加 char 类型函数指针
typedef void (*pvAddCharType)(Array *, char);

// 声明部分

// 设置数组存储大小
void vSetCapacity(Array *array, size_t ulSetSize);
// 向数组添加 char 类型数据
void vAdd(Array *array, char newChar);

// 默认初始化数组宏定义
#define InitArray     \
	{                 \
		0, NULL, NULL \
	}

// 向数组添加 char 类型数据
void vAdd(Array *array, char newChar)
{
	// 越界检测，[Head,Tail)模式
	if (array->pxStorage - array->pxDataHead < array->ulSize)
		// 复写并移动指针
		*(array->pxStorage)++ = newChar;
	else
	{
		// 越界错误报告
		printf("Error: char array overlay in line: %d\n", __LINE__);
		abort();
	}
}

// 设置数组存储大小
void vSetCapacity(Array *array, size_t ulSetSize)
{
	// 待删除的旧指针
	pDataType pxDeleteTmp = NULL;
	// 申请内存大小应该大于1，否则报错（size_t 是 unsigned long，不会是负数）
	assert(ulSetSize && "Array size should be up to 1.");

	if (array->pxDataHead == NULL)
	{
		// 如果没初始化过，直接给一块内存
		array->pxDataHead = (pDataType)malloc(sizeof(char) * ulSetSize);
		array->pxStorage = array->pxDataHead;
		array->ulSize = ulSetSize;

		// 内存如果申请失败，请检查堆的使用率
		if (array->pxDataHead == NULL)
		{
			printf("Check for your heap.\n");
			abort();
		}
	}
	else
	{
		// 初始化过的话
		if (ulSetSize == array->ulSize)
			return;

		// 更新前头指针地址
		pxDeleteTmp = array->pxDataHead;
		// 更新前头指针缓存
		pDataType pxTmp = pxDeleteTmp;

		// 更新新分配的指针地址
		array->pxDataHead = (pDataType)malloc(sizeof(char) * ulSetSize);
		// 内存如果申请失败，请检查堆的使用率
		if (array->pxDataHead == NULL)
		{
			printf("Check for your heap.\n");
			abort();
		}

		// 更新后头指针缓存
		pDataType pDataTmp = array->pxDataHead;

		int i = 0;
		// 更大的话就换更大的然后刷指针
		if (ulSetSize > array->ulSize)
		{
			for (; i < array->ulSize; ++i)
				// 缓存指针刷写
				*pDataTmp++ = *pxTmp++;
			// 更新尾迭代器
			array->pxStorage = array->pxDataHead + array->ulSize;
		}
		else
		{
			// 如果小了就截断
			for (; i < ulSetSize; ++i)
				// 缓存指针刷写
				*pDataTmp++ = *pxTmp++;
			// 更新尾迭代器
			array->pxStorage = array->pxDataHead + ulSetSize;
			// printf("Array trunk\n");
		}
		// 更新大小
		array->ulSize = ulSetSize;
		// 释放旧内存
		free(pxDeleteTmp);
	}
}

void vPrintArray(Array *array)
{
	pDataType it = array->pxDataHead;
	for (; it < array->pxStorage; ++it)
		printf("%c", *it);
	printf("\n");
}

int main(void)
{
	// 测试字符
	char chTest = 'c';
	Array arrTest = InitArray;

	// 初始化数组
	vSetCapacity(&arrTest, 10);
	int i = 0;
	for (; i < 10; ++i)
	{
		// 字母顺序赋值
		vAdd(&arrTest, chTest + i);
		vPrintArray(&arrTest);
	}

	printf("--------------------------------\n");

	// 更新数组大小
	vSetCapacity(&arrTest, 5);
	vPrintArray(&arrTest);

	return 0;
}