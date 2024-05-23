/**
 * Created by Filip Dabkowski on 20/05/2024.
 */

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

namespace dst {
	template<typename T>
	class PriorityQueue {
		int t_size;
		int t_capacity;
		T* t_heapArray;

		static int parentIndex(int index) { return index / 2; }
		static int leftIndex(int index) { return index * 2; }
		static int rightIndex(int index) { return index * 2 + 1; }

		void swap(int indexA, int indexB) {
			T temp = t_heapArray[indexA];
			t_heapArray[indexA] = t_heapArray[indexB];
			t_heapArray[indexB] = temp;
		}

		void silfUp(int index) {
			while (index != 1 && t_heapArray[parentIndex(index)] < t_heapArray[index]) {
				swap(parentIndex(index), index);
				index = parentIndex(index);
			}
		}

		void slifDown(int index) {
			while (index < t_size && leftIndex(index) <= t_size && rightIndex(index) <= t_size) {
				int left = t_heapArray[leftIndex(index)];
				int right = t_heapArray[rightIndex(index)];
				if (t_heapArray[index] < left || t_heapArray[index] < right) {
					if (right < left) {
						swap(leftIndex(index), index);
						index = leftIndex(index);
					} else {
						swap(rightIndex(index), index);
						index = rightIndex(index);
					}
				} else {
					break;
				}
			}
		}

	public:
		explicit PriorityQueue(int size) {
			t_size = 0;
			t_capacity = size;
			t_heapArray = new T[t_size + 1];
		}

		explicit PriorityQueue(int size, bool (*compareFunction)(const T&, const T&)) {
			t_size = 0;
			t_capacity = size;
			t_heapArray = new T[t_size + 1];
		}

		void insert(const T& value) {
			if (t_size >= t_capacity)
				return;

			t_heapArray[++t_size] = value;
			silfUp(t_size);
		}

		T pop() {
			T removed = t_heapArray[1];
			t_heapArray[1] = t_heapArray[t_size--];
			slifDown(1);

			return removed;
		}

		bool isEmpty() {
			return t_size == 0;
		}
	};
} // namespace dst

#endif // PRIORITYQUEUE_H
