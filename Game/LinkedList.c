#include "LinkedList.h"

LinkedList* CreateLinkedList(void)
{
	return calloc(1, sizeof(LinkedList));
}

void PrintLinkedList(const LinkedList* const _list)
{
	Element* element = _list->first;
	while (element)
	{
		printf("Value : %d\n", element->value);
		element = element->next;
	}
}

unsigned GetSizeList(const LinkedList* const _list)
{
	return _list->count;
}

Element* GetElement(const LinkedList* const _list, unsigned _index)
{
	unsigned i = 0;

	Element* element = _list->first;
	while (i < _index && element)
	{
		i++;
		element = element->next;
	}

	return element;
}

void InsertElement(LinkedList* const _list, Element* const _element, unsigned _index)
{
	if (_index == 0)
	{
		_element->next = _list->first;
		_list->first = _element;
	}
	else
	{
		Element* previous = GetElement(_list, _index - 1);
		_element->next = previous->next;
		previous->next = _element;
	}

	_list->count++;
}

void RemoveElement(LinkedList* const _list, unsigned _index)
{
	if (_index == 0)
	{
		Element* elementToDelete = _list->first;
		_list->first = elementToDelete->next;
		DeleteElement(elementToDelete);
	}
	else
	{
		Element* previous = GetElement(_list, _index - 1);
		Element* elementToDelete = previous->next;
		previous->next = elementToDelete->next;
		DeleteElement(elementToDelete);
	}

	_list->count--;
}

void RemoveLinkedList(LinkedList* _list)
{
	while (_list->first)
	{
		RemoveElement(_list, 0);
	}
	free(_list);
}

Element* CreateElement(void)
{
	return calloc(1, sizeof(Element));
}

void DeleteElement(Element* _element)
{
	free(_element);
}