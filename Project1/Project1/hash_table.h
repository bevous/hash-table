#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

namespace nwacc {

	template <typename T,typename K>
	class hash_table
	{
	public:
		// note made this seven so we could see this work when printing. 
		/**
		 * initializes the array with the proper size;
		 * 
		 * \param size
		 */
		explicit hash_table(int size = 7) : array(next_prime(size))
		{
			this->make_empty();
		}
		/**
		 * checks to see is the hashmap contains the given value
		 * 
		 * \param value
		 * \return bool
		 */
		bool contains_value(const T & value) const
		{
			return this->is_active(this->find_value(value));
		}
		/**
		  * checks to see is the hashmap contains the given value
		 * 
		 * \param key
		 * \return bool 
		 */
		bool contains_key(const K & key)
		{
			return this->is_active(this->find_position(key));
		}
		/*!
		 * empties out the map
		 * 
		 */
		void make_empty()
		{
			this->current_size = 0;
			for (auto & entry : this->array)
			{
				entry.type = kEmpty;
			}
		}
		/**
		 * inserts the given value into the map
		 * 
		 * \param value
		 * \param key
		 * \return bool
		 */
		bool insert(const T & value,const K & key)
		{
			auto current_position = this->find_position(key);
			if (this->is_active(current_position))
			{
				this->array[current_position].element = value;
			}
			else
			{
				this->array[current_position].element = value;
				this->array[current_position].key = key;
				this->array[current_position].type = kActive;
				this->my_size++;
			}

			if (++this->current_size > this->array.size() / 2)
			{ // this has a load factor of 50%
				this->my_size = 0;
				this->rehash();
			} // else we are within the load factor do_nothing();

			return true;
		}
		/**
		 * inserts the given value into the map
		 *
		 * \param value
		 * \param key
		 * \return bool
		 */
		 bool insert(T && value,K && key)
		 {
		 	auto current_position = this->find_position(key);
		 	if (this->is_active(current_position))
		 	{
				this->array[current_position].element = std::move(value);
		 	}
		 	else
		 	{
				this->array[current_position].element = std::move(value);
				this->array[current_position].key = std::move(key);
				this->array[current_position].type = kActive;
				this->my_size++;
		 	}
		
		 	if (++this->current_size > this->array.size() / 2)
		 	{
				my_size = 0;
		 		this->rehash();
		 	} // else, still within the load factor, do_nothing();
		
		 	return true;
		 }
		 /**
		  * removes a value from the map
		  * 
		  * \param value
		  * \return bool
		  */
		bool remove(const T & value)
		{
			auto current_position = this->find_value(value);
			if (!this->is_active(current_position))
			{
				return false;
			}
			else
			{
				this->array[current_position].type = kDeleted;
				return true;
			}
		}
		/**
		 *removes the given key from the map
		 * 
		 * 
		 * \param key
		 * \return 
		 */
		bool remove(const K & key)
		{
			auto current_position = this->find_position(key);
			if (!this->is_active(current_position))
			{
				return false;
			}
			else
			{
				this->array[current_position].type = kDeleted;
				return true;
			}
		}
		/**
		 * returns the value stored with the given key
		 * 
		 * \param key
		 * \return T
		 */
		T get(const K& key)
		{
			return contains_key(key)? this->array[this->find_position(key)].element: throw std::length_error("key does not exist");
		}
		/**
		 * the differant states a node in the map and have
		 * 
		 */
		enum entry_type { kActive, kEmpty, kDeleted };
		/**
		 * prints the map to the given out stream
		 * 
		 * \param out
		 */
		void print(std::ostream & out = std::cout)
		{
			for (auto entry : this->array)
			{
				if (entry.type == kActive)
				{
					out << entry.element << ", " <<entry.key<<std::endl;
				}
			}
			out << std::endl;
		}
		/**
		 * returns a reference to the value stored with the key
		 * 
		 * \param key
		 * \return T&
		 */
		T& operator[](const K& key)
		{
			return this->array[this->find_position(key)].element;
		}
		/**
		 * returns a reference to the value stored in the map
		 *
		 * \param value
		 * \return T&
		 */
		T& operator[](const T& value)
		{
			return this->array[this->find_value(value)].element;
		}
		/**
		 * 
		 * 
		 * \return int the number of entries in the map
		 */
		int size()const
		{
			return this->my_size;
			
		}

	private:
		/**
		 * checks if a number is prime
		 * 
		 * \param number
		 * \return bool
		 */
		static bool is_prime(const int number)
		{
			if (number == 2 || number == 3) return true;
			if (number == 1 || number % 2 == 0) return false;
			for (auto counter = 3; counter * counter <= number; counter += 2)
				if (number % counter == 0) return false;

			return true;
		}
		/*!
		 * give the next prime number in sequence
		 * 
		 * \param number
		 * \return int
		 */
		static int next_prime(int number)
		{
			if (number % 2 == 0) ++number;
			while (!is_prime(number)) number += 2;
			return number;
		}
		/**
		 * the definition of an entry in the map
		 * 
		 */
		struct entry
		{
			T element;
			K key;
			entry_type type;

			entry(const T & e = T{}, const K& k = K{}, entry_type t = kEmpty)
				: element{ e }, key{ k }, type{ t } { }

			entry(T && e,const K&& k, entry_type t = kEmpty)
				: element{ std::move(e) }, key{ std::move(k) }, type{ t } { }
			
		};
		/**
		 * gives you information about the state of the given entry
		 * 
		 * \param place
		 * \return string
		 */
		std::string get_type(const entry & place) const
		{
			if (place.type == kEmpty)
			{
				return "E";
			}
			
			if (place.type == kActive)
			{
				return "A";
			}

			return "D";
		}
		/**
		 * the vector for storing the entries
		 * 
		 */
		std::vector<entry> array;
		/**
		 * the current size of the array
		 * 
		 */
		std::size_t current_size;
		/**
		 * the current number of entries in the map
		 * 
		 */
		int my_size;
		/**
		 *checks if the current state of an entry is active 
		 * 
		 * 
		 * \param current_position
		 * \return Bool
		 */
		bool is_active(int current_position) const
		{
			return this->array[current_position].type == kActive;
		}
		/**
		 * finds the index where the given value is stored
		 * 
		 * \param value
		 * \return 
		 */
		int find_value(const T& value) const
		{
			for (auto entry : this->array)
			{
				if(entry.element == value)
				{
					return this->find_position(entry.key);
				}
			}
			return {};
		}
		/**
		 * finds the index where the given key is stored
		 *
		 * \param key
		 * \return
		 */
		int find_position(const K & key) const
		{
			auto off_set = 1;
			auto current_position = this->hash(key);

			while (this->array[current_position].type != kEmpty &&
				this->array[current_position].key != key)
			{
				current_position += off_set;
				off_set += 2;
				if (current_position >= this->array.size())
				{
					current_position -= this->array.size();
				} // else, we have not ran outside the size, do_nothing();
			}
			return current_position;
		}
		/**
		 * resizes the array to keep the load below 50%
		 * 
		 */
		void rehash()
		{
			auto old_array = this->array;
			this->array.resize(next_prime(2 * old_array.size()));
			for (auto & entry : this->array)
			{ // step 1 empty all the elements in the new array. 
				entry.type = kEmpty;
			}

			// rehash all the inserted items. 
			for (auto & entry : old_array)
			{
				if (entry.type == kActive)
				{
					this->insert(std::move(entry.element), std::move(entry.key));
				} // else, the entry is not active, do_nothing();
			}

		}
		/**
		 * calculates where in the array the key should be stored
		 * 
		 * \param key
		 * \return 
		 */
		std::size_t hash(const K & key) const
		{
			static std::hash<T> hash_object;
			return hash_object(key) % this->array.size();
		}
	};
}

#endif