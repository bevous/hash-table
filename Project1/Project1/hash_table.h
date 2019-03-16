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
		explicit hash_table(int size = 7) : array(next_prime(size))
		{
			this->make_empty();
		}

		bool contains(const T & value) const
		{
			return this->is_active(this->find_position(value));
		}

		void make_empty()
		{
			this->current_size = 0;
			for (auto & entry : this->array)
			{
				entry.type = kEmpty;
			}
		}

		bool insert(const T & value,const K & key)
		{
			auto current_position = this->find_position(value);
			if (this->is_active(current_position))
			{
				return false;
			}
			else
			{
				this->array[current_position] = new entry{ value,key,kActive };
			}

			if (++this->current_size > this->array.size() / 2)
			{ // this has a load factor of 50%
				this->rehash();
			} // else we are within the load factor do_nothing();

			return true;
		}

		 bool insert(T && value,K && key)
		 {
		 	auto current_position = this->find_position(value);
		 	if (this->is_active(current_position))
		 	{
		 		return false;
		 	}
		 	else
		 	{
				this->array[current_position] = new entry{ std::move(value),std::move(key),kActive };
		 	}
		
		 	if (++this->current_size > this->array.size() / 2)
		 	{
		 		this->rehash();
		 	} // else, still within the load factor, do_nothing();
		
		 	return true;
		 }

		bool remove(const T & value)
		{
			auto current_position = this->find_position(value);
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

		enum entry_type { kActive, kEmpty, kDeleted };

		void print(std::ostream & out = std::cout)
		{
			for (auto entry : this->array)
			{
				out << entry.element << " {" << this->get_type(entry) << "} | ";
			}
			out << std::endl;
		}

	private:

		static bool is_prime(const int number)
		{
			if (number == 2 || number == 3) return true;
			if (number == 1 || number % 2 == 0) return false;
			for (auto counter = 3; counter * counter <= number; counter += 2)
				if (number % counter == 0) return false;

			return true;
		}

		static int next_prime(int number)
		{
			if (number % 2 == 0) ++number;
			while (!is_prime(number)) number += 2;
			return number;
		}

		struct entry
		{
			T element;
			K key;
			entry_type type;

			entry(const T & e = T{}, const K& k = K{}, entry_type t = kEmpty)
				: element{ e }, key{ k }, type{ t } { }

			entry(T && e,const K&& k, entry_type t = kEmpty)
				: element{ std::move(e) }, key{ std::move(k) }, type{ t } { }

			entry & operator=(const entry & rhs)
			{
				auto copy = rhs;
				std::swap(*this, copy);
				return *this;
			}
			/**
			 *swaps the right hand side with the left.
			 *@param rhs
			 *@returns hash_map
			 */
			entry & operator=(entry && rhs)
			{
				std::swap(*this, rhs);
				return *this;
			}
		};

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

		std::vector<entry> array;

		std::size_t current_size;

		bool is_active(int current_position) const
		{
			return this->array[current_position].type == kActive;
		}

		int find_position(const T & value) const
		{
			auto off_set = 1;
			auto current_position = this->hash(value);

			while (this->array[current_position].type != kEmpty &&
				this->array[current_position].element != value)
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
					this->insert(std::move(entry.element));
				} // else, the entry is not active, do_nothing();
			}

		}

		std::size_t hash(const T & value) const
		{
			static std::hash<T> hash_object;
			return hash_object(value) % this->array.size();
		}
	};
}

#endif