#pragma once

template <typename T>
struct LinkedList 
{
	struct Node
	{
		T* value = nullptr;
		Node* next = nullptr;
	};

	Node* head = new Node();

	inline ~LinkedList() {
		Node* ptr = head;
		while (ptr != nullptr) {
			Node* next = ptr->next;
			delete ptr->value;
			delete ptr;
			ptr = next;
		}
	};

	inline Node* get(int pos) {
		Node* ptr = head;
		for (int i = 0; i < pos; i++) {
			ptr = ptr->next;
		}
		return ptr;
	};
	inline int length() {
		int total = 0;
		Node* ptr = head;
		while (ptr != nullptr) {
			total++;
			ptr = ptr->next;
		}
		return total;
	};
};