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
		T* t_heapData;

		int* t_indexTable = nullptr;
		void (*t_indexTableUpdate)(const T&, int, int*) = nullptr;

		void udpateIndexes(int prevIndex, int newIndex) {
			if (t_indexTableUpdate != nullptr) {
				t_indexTableUpdate(t_heapData[prevIndex], prevIndex, t_indexTable);
				t_indexTableUpdate(t_heapData[newIndex], newIndex, t_indexTable);
			}
		}

		static int parentIndex(int index) { return index / 2; }
		static int leftIndex(int index) { return index * 2; }
		static int rightIndex(int index) { return index * 2 + 1; }

		void swap(int indexA, int indexB) {
			T temp = t_heapData[indexA];
			t_heapData[indexA] = t_heapData[indexB];
			t_heapData[indexB] = temp;
		}

		void slifUp(int index) {
			while (index != 1 && t_heapData[parentIndex(index)] < t_heapData[index]) {
				int pIndex = parentIndex(index);
				swap(pIndex, index);
				udpateIndexes(index, pIndex);
				index = pIndex;
			}
		}

		void slifDown(int index) {
			while (index < t_size && leftIndex(index) <= t_size && rightIndex(index) <= t_size) {
				int lIndex = leftIndex(index);
				int rIndex = rightIndex(index);
				T left = t_heapData[lIndex];
				T right = t_heapData[rIndex];
				if (t_heapData[index] < left || t_heapData[index] < right) {
					if (right < left) {
						swap(lIndex, index);
						udpateIndexes(index, lIndex);
						index = lIndex;
					} else {
						swap(rIndex, index);
						udpateIndexes(index, rIndex);
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
			t_heapData = nullptr;
		}

		explicit PriorityQueue(int size) {
			t_size = 0;
			t_capacity = size+1;
			t_heapData = new T[size+1];
		}


		void insert(const T& value) {
			if (t_size >= t_capacity)
				return;

			t_size++;
			t_heapData[t_size] = value;
			slifUp(t_size);
		}

		void updateAtIndex(int index, const T& newValue) {
			t_heapData[index] = newValue;
			if (index > 1 && t_heapData[parentIndex(index)] < t_heapData[index]) {
				slifUp(index);
			} else {
				slifDown(index);
			}
		}

		T pop() {
			T removed = t_heapData[1];
			t_heapData[1] = t_heapData[t_size--];
			if (t_indexTableUpdate != nullptr) {
				t_indexTableUpdate(t_heapData[1], 1, t_indexTable);
			}
			slifDown(1);

			return removed;
		}

		void setIndexTableCallback(void (*callback)(const T&, int, int*), int* indexTable) {
			t_indexTable = indexTable;
			t_indexTableUpdate = callback;
		}

		T* _getRawTable() {
			return this->t_heapData;
		}

		int getSize() {
			return t_size;
		}

		bool isEmpty() {
			return t_size == 0;
		}

		void clear() {}

		~PriorityQueue() {
			clear();
		}
	};
} // namespace dst

#endif // PRIORITYQUEUE_H
