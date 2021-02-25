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
	inline void add(T* data) {
		Node* ptr = head;
		if (head == nullptr) {
			head = new Node();
			head->value = data;
		}
		else {
			while (ptr->next != nullptr) {
				ptr = ptr->next;
			}
			ptr->next = new Node();
			ptr->next->value = data;
		}
	}
};

struct DynamicLinkedList // Do not use unless inheriting, types must be kept track of
{
	static const inline int TYPE_UNKNOWN = -1;
	struct Node
	{
		int type = TYPE_UNKNOWN;
		void* data = nullptr;
		Node* next = nullptr;
	};

	Node* head = nullptr;

	void add(void* data, int type);
	int length();
	Node* get(int index);
};