#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "Common.h"

typedef struct Element
{
	int value;

	struct Element* next;
}Element;

typedef struct LinkedList
{
	Element* first;
	unsigned count;
}LinkedList;

LinkedList* CreateLinkedList(void);
void PrintLinkedList(const LinkedList* const _list);
unsigned GetSizeList(const LinkedList* const _list);
Element* GetElement(const LinkedList* const _list, unsigned _index);
void InsertElement(LinkedList* const _list, Element* const _element, unsigned _index);
void RemoveElement(LinkedList* const _list, unsigned _index);
void RemoveLinkedList(LinkedList* _list);
Element* CreateElement(void);
void DeleteElement(Element* _element);

#endif
