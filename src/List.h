/**
 * Created by Filip Dabkowski on 30/04/2024.
 */

#ifndef LIST_H
#define LIST_H

namespace dst {
	template<typename T>
	class List {

	public:
		List() = default;

		List(const List& other) : size(other.size) {
			head = nullptr;
			tail = nullptr;
			if (other.head == nullptr)
				return;

			head = new Node{other.head->value};

			Node* toCopy = other.head->after;
			Node* current = head;
			while (toCopy != other.tail) {
				Node* newNode = new Node{toCopy->value};
				current->after = newNode;
				current = newNode;
				toCopy = toCopy->after;
			}
			current->after = nullptr;
			tail = current;
		}

		List& operator=(const List& rhs) {
			if (this == &rhs) {
				return *this;
			}
			clear();

			size = rhs.size;
			if (rhs.head == nullptr)
				return *this;

			head = new Node{rhs.head->value};

			Node* toCopy = rhs.head->after;
			Node* current = head;
			while (toCopy != rhs.tail) {
				Node* newNode = new Node{toCopy->value};
				current->after = newNode;
				current = newNode;
				toCopy = toCopy->after;
			}
			current->after = nullptr;
			tail = current;

			return *this;
		}

		void clear() {
			Node* toDelete = head;
			while (toDelete != nullptr) {
				head = toDelete->after;
				delete toDelete;
				toDelete = head;
			}
			size = 0;
			head = nullptr;
			tail = nullptr;
		}

		void insertFront(const T& value) {
			auto* newNode = new Node{value};
			newNode->after = head;
			head = newNode;

			size++;

			if (tail == nullptr) {
				tail = newNode;
			}
		}

		void insertBack(const T& value) {
			auto* newNode = new Node{value};

			size++;
			if (tail == nullptr) {
				head = newNode;
				tail = newNode;
				return;
			}

			tail->after = newNode;
			tail = newNode;
		}

		T& front() {
			return head->value;
		}

		T& back() {
			return tail->value;
		}

		T removeFront() {
			Node* toDelete = head;
			head = toDelete->after;
			if (head == nullptr) {
				tail = nullptr;
			}

			size--;

			T retrievedValue = toDelete->value;
			delete toDelete;
			return retrievedValue;
		}

		[[nodiscard]]
		bool isEmpty() const {
			return size <= 0;
		}

		[[nodiscard]]
		int getSize() const {
			return size;
		}


		// Stack Methods

		void push(const T& value) {
			insertFront(value);
		}

		T pop() {
			return removeFront();
		}

		T& top() {
			return front();
		}

		// Queue Methods

		void put(const T& value) {
			insertBack(value);
		}

		~List() {
			clear();
		}

	private:
		struct Node {
			T value;
			Node* after = nullptr;
		};

		Node* head = nullptr;
		Node* tail = nullptr;

		int size = 0;

		class Iterator {
			Node* currentNode;

		public:
			explicit Iterator(Node* node) : currentNode(node) {}

			Iterator& operator++() {
				currentNode = currentNode->after;
				return *this;
			}

			bool operator!=(const Iterator& other) const {
				return currentNode != other.currentNode;
			}

			T& operator*() const {
				return currentNode->value;
			}
		};
	public:
		Iterator begin() {
			return Iterator(head);
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
