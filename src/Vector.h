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
		T* data; ///< Pointer to table of data

		size_t size; ///< Number of element inside
		size_t capacity; ///< Actually reserved capacity

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
		// Default Constructor
		explicit Vector(size_t initialSize = 0) : size(0), capacity(initialSize), data(new T[initialSize]) {}


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


		T* getRawPointer() {
			return data;
		}


		[[nodiscard]] T& get(size_t index) { return data[index]; }


		void pop() { size--; }


		[[nodiscard]] size_t getSize() const { return size; }


		void clear() { size = 0; }


		[[nodiscard]] const T& get(size_t index) const { return data[index]; }


		[[nodiscard]] T& operator[](size_t index) { return get(index); }


		[[nodiscard]] const T& operator[](size_t index) const { return get(index); }


		// Iterator
		class iterator {
		private:
			T* ptr;

		public:
			explicit iterator(T* ptr) : ptr(ptr) {}


			bool operator!=(const iterator& other) const { return ptr != other.ptr; }


			const iterator& operator++() {
				++ptr;
				return *this;
			}


			T& operator*() { return *ptr; }
		};


		// Begin and end for iterator
		iterator begin() { return iterator(data); }


		iterator end() { return iterator(data + size); }
	};
} // namespace dst

#endif // VECTOR_H
