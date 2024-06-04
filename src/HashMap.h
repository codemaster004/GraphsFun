/**
 * Created by Filip Dabkowski on 29/05/2024.
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include "List.h"
#include "Vector.h"

namespace dst {
	template<typename T>
	class HashMap {
		int t_cappacity;
		int t_size;

		struct KeyValue {
			int key;
			T value;
		};

		Vector<List<KeyValue>> t_data;

		int hash(int intValue) { return intValue % t_cappacity; }

	public:
		HashMap() : t_cappacity(50), t_size(0) { t_data.resize(t_cappacity); }

		explicit HashMap(int capacity) : t_cappacity(capacity), t_size(0) { t_data.resize(t_cappacity); }

		void setValue(int key, T value) {
			int hashedKey = hash(key);
			t_data[hashedKey].insertBack({key, value});
			t_size++;
		}

		T get(int key) {
			int hashedKey = hash(key);
			for (KeyValue element: t_data[hashedKey]) {
				if (element.key == key) {
					return element.value;
				}
			}
			return T();
		}

		bool existsIn(int key) {
			int hashedKey = hash(key);
			for (KeyValue element: t_data[hashedKey]) {
				if (element.key == key) {
					return true;
				}
			}
			return false;
		}

		[[nodiscard]] int getSize() const { return t_size; }
	};
} // namespace dst

#endif // HASHMAP_H
