/**
 * Created by Filip Dabkowski on 19/05/2024.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "cstddef"


namespace dst {

	template<typename T>
	class Vector {
	private:
		size_t size; ///< Number of element inside
		size_t capacity; ///< Actually reserved capacity

		T* data; ///< Pointer to table of data

		void performResize() {
			T* newValues = new T[capacity];
			for (int i = 0; i < size; ++i) {
				newValues[i] = data[i];
			}

			delete[] data;
			data = newValues;
		}


		void resize() {
			capacity = (capacity == 0) ? 1 : capacity * 2;
			performResize();
		}


		void shift_left(size_t startIndex) {
			for (size_t i = startIndex; i < size - 1; ++i) {
				data[i] = data[i + 1];
			}
		}


	public:
		Vector() : size(0), capacity(0), data(nullptr) {}

		explicit Vector(size_t initialSize) : size(0), capacity(initialSize), data(new T[initialSize]) {}


		// Copy Constructor
		Vector(const Vector& other) : size(other.size), capacity(other.capacity), data(new T[other.capacity]) {
			for (int i = 0; i < other.size; ++i)
				data[i] = other.data[i];
		}


		// Copy Assignment
		Vector& operator=(const Vector& other) {
			if (this == &other)
				return *this;

			delete[] data;
			data = new T[other.capacity];
			for (int i = 0; i < other.size; ++i)
				data[i] = other.data[i];

			size = other.size;
			capacity = other.capacity;

			return *this;
		}


		// Default destructor
		~Vector() {
			if (capacity > 0) {
				delete[] data;
			}
		}


		void resize(size_t newCapacity) {
			capacity = newCapacity;
			performResize();
		}


		void pushBack(const T& newElement) {
			if (size == capacity) {
				resize();
			}

			data[size++] = newElement;
		}

		template<typename... Args>
		void emplaceBack(Args&&... args) {
			if (size == capacity) {
				resize();
			}

			data[size++] = T(static_cast<Args&&>(args)...);
		}


		void remove(size_t index) {
			shift_left(index);
			--size;
		}


		T* getRawPointer() { return data; }

		void setSize(int newSize){this->size = newSize;};


		[[nodiscard]] T& get(size_t index) { return data[index]; }


		void pop() { size--; }


		[[nodiscard]] size_t getSize() const { return size; }
		[[nodiscard]] size_t getCapacity() const { return capacity; }


		void clear() { size = 0; }


		[[nodiscard]] const T& get(size_t index) const { return data[index]; }


		[[nodiscard]] T& operator[](size_t index) { return get(index); }


		[[nodiscard]] const T& operator[](size_t index) const { return get(index); }


		// Iterator
		class Iterator {
			T* ptr;

		public:
			explicit Iterator(T* ptr) : ptr(ptr) {}

			bool operator!=(const Iterator& other) const { return ptr != other.ptr; }

			const Iterator& operator++() {
				++ptr;
				return *this;
			}

			T& operator*() { return *ptr; }
		};


		// Begin and end for iterator
		Iterator begin() { return Iterator(data); }

		Iterator end() { return Iterator(data + size); }
	};
} // namespace dst

#endif // VECTOR_H
