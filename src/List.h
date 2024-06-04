/**
 * Created by Filip Dabkowski on 30/04/2024.
 */

#ifndef LIST_H
#define LIST_H

namespace dst {
	template<typename T>
	class List {
	public:
		struct Node {
			T value;
			Node* next = nullptr;
		};

		List() = default;

		List(const List& other) : t_size(other.t_size) {
			t_head = nullptr;
			t_tail = nullptr;
			if (other.t_head == nullptr)
				return;

			t_head = new Node{other.t_head->value};

			Node* toCopy = other.t_head->next;
			Node* current = t_head;
			while (toCopy != other.t_tail) {
				Node* newNode = new Node{toCopy->value};
				current->next = newNode;
				current = newNode;
				toCopy = toCopy->next;
			}
			current->next = nullptr;
			t_tail = current;
		}

		List& operator=(const List& rhs) {
			if (this == &rhs) {
				return *this;
			}
			clear();

			t_size = rhs.t_size;
			if (rhs.t_head == nullptr)
				return *this;

			t_head = new Node{rhs.t_head->value};

			Node* toCopy = rhs.t_head->next;
			Node* current = t_head;
			while (toCopy != rhs.t_tail) {
				Node* newNode = new Node{toCopy->value};
				current->next = newNode;
				current = newNode;
				toCopy = toCopy->next;
			}
			current->next = nullptr;
			t_tail = current;

			return *this;
		}

		void clear() {
			Node* toDelete = t_head;
			while (toDelete != nullptr) {
				t_head = toDelete->next;
				delete toDelete;
				toDelete = t_head;
			}
			t_size = 0;
			t_head = nullptr;
			t_tail = nullptr;
		}

		void insertFront(const T& value) {
			auto* newNode = new Node{value};
			newNode->next = t_head;
			t_head = newNode;

			t_size++;

			if (t_tail == nullptr) {
				t_tail = newNode;
			}
		}

		void insertBack(const T& value) {
			auto* newNode = new Node{value};

			t_size++;
			if (t_tail == nullptr) {
				t_head = newNode;
				t_tail = newNode;
				return;
			}

			t_tail->next = newNode;
			t_tail = newNode;
		}

		T& front() {
			return t_head->value;
		}

		T& back() {
			return t_tail->value;
		}

		T removeFront() {
			Node* toDelete = t_head;
			t_head = toDelete->next;
			if (t_head == nullptr) {
				t_tail = nullptr;
			}

			t_size--;

			T retrievedValue = toDelete->value;
			delete toDelete;
			return retrievedValue;
		}

		[[nodiscard]]
		bool isEmpty() const {
			return t_size <= 0;
		}

		[[nodiscard]]
		int getSize() const {
			return t_size;
		}

		T& get(int index) {
			Node* currect = t_head;
			for (int i = 0; i < index; ++i) {
				currect = currect->next;
			}
			return currect->value;
		}

		T& operator[](int index) {
			return get(index);
		}

		Node* getHead() {
			return t_head;
		}

		// Stack Methods

		void put(const T& value) {
			insertFront(value);
		}

		T pop() {
			return removeFront();
		}

		T& top() {
			return front();
		}

		// Queue Methods

		void push(const T& value) {
			insertBack(value);
		}

		~List() {
			clear();
		}

	private:

		Node* t_head = nullptr;
		Node* t_tail = nullptr;

		int t_size = 0;

	public:
		class Iterator {
			Node* t_currentNode;

		public:
			explicit Iterator(Node* node) : t_currentNode(node) {}

			Iterator& operator++() {
				t_currentNode = t_currentNode->next;
				return *this;
			}

			bool operator!=(const Iterator& other) const {
				return t_currentNode != other.t_currentNode;
			}

			T& operator*() const {
				return t_currentNode->value;
			}
		};

		Iterator begin() {
			return Iterator(t_head);
		}

		Iterator end() {
			return Iterator(nullptr);
		}
	};

	template<typename T>
	using Stack = List<T>;

	template<typename T>
	using Queue = List<T>;

}

#endif //LIST_H
