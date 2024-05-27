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

		int* t_indexTable = nullptr;
		void (*t_indexTableUpdate)(const T&, int, int*) = nullptr;

		static int parentIndex(int index) { return index / 2; }
		static int leftIndex(int index) { return index * 2; }
		static int rightIndex(int index) { return index * 2 + 1; }

		void swap(int indexA, int indexB) {
			T temp = t_heapArray[indexA];
			t_heapArray[indexA] = t_heapArray[indexB];
			t_heapArray[indexB] = temp;
		}

		void slifUp(int index) {
			while (index != 1 && t_heapArray[parentIndex(index)] < t_heapArray[index]) {
				int pIndex = parentIndex(index);
				swap(pIndex, index);
				if (t_indexTableUpdate != nullptr) {
					t_indexTableUpdate(t_heapArray[index], index, t_indexTable);
					t_indexTableUpdate(t_heapArray[pIndex], pIndex, t_indexTable);
				}
				index = pIndex;
			}
		}

		void slifDown(int index) {
			while (index < t_size && leftIndex(index) <= t_size && rightIndex(index) <= t_size) {
				int lIndex = leftIndex(index);
				int rIndex = rightIndex(index);
				T left = t_heapArray[lIndex];
				T right = t_heapArray[rIndex];
				if (t_heapArray[index] < left || t_heapArray[index] < right) {
					if (right < left) {
						swap(lIndex, index);
						if (t_indexTableUpdate != nullptr) {
							t_indexTableUpdate(t_heapArray[lIndex], lIndex, t_indexTable);
							t_indexTableUpdate(t_heapArray[index], index, t_indexTable);
						}
						index = lIndex;
					} else {
						swap(rIndex, index);
						if (t_indexTableUpdate != nullptr) {
							t_indexTableUpdate(t_heapArray[index], index, t_indexTable);
							t_indexTableUpdate(t_heapArray[rIndex], rIndex, t_indexTable);
						}
						index = rIndex;
					}
				} else {
					break;
				}
			}
		}

	public:
		explicit PriorityQueue() {
			t_size = 0;
			t_capacity = 0;
			t_heapArray = nullptr;
		}

		explicit PriorityQueue(int size) {
			t_size = 0;
			t_capacity = size;
			t_heapArray = new T[t_size + 1];
		}


		void insert(const T& value) {
			if (t_size >= t_capacity)
				return;

			t_heapArray[++t_size] = value;
			slifUp(t_size);
		}

		void repairAtIndex(int index) {
			if (parentIndex(index) < index) {
				slifUp(index);
			} else {
				slifDown(index);
			}
		}

		T pop() {
			T removed = t_heapArray[1];
			t_heapArray[1] = t_heapArray[t_size--];
			if (t_indexTableUpdate != nullptr) {
				t_indexTableUpdate(t_heapArray[t_size + 1], 1, t_indexTable);
			}
			slifDown(1);

			return removed;
		}

		void setIndexTableCallback(void (*callback)(const T&, int, int*), int* indexTable) {
			t_indexTable = indexTable;
			t_indexTableUpdate = callback;
		}

		T* _getRawTable() {
			return this->t_heapArray;
		}

		int getSize() {
			return t_size;
		}

		bool isEmpty() {
			return t_size == 0;
		}
	};
} // namespace dst

#endif // PRIORITYQUEUE_H
