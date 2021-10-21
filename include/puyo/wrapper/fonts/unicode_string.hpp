#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <vector>

namespace puyo
{
	namespace sdl
	{
		using unicode = std::uint16_t;

		class unicode_string final
		{
		public:
			using value_type = std::vector<unicode>::value_type;

			using pointer = std::vector<unicode>::pointer;
			using const_pointer = std::vector<unicode>::const_pointer;

			using reference = std::vector<unicode>::reference;
			using const_reference = std::vector<unicode>::const_reference;

			using iterator = std::vector<unicode>::iterator;
			using const_iterator = std::vector<unicode>::const_iterator;

			using reverse_iterator = std::vector<unicode>::reverse_iterator;
			using const_reverse_iterator = std::vector<unicode>::const_reverse_iterator;

			using size_type = std::vector<unicode>::size_type;
			using difference_type = std::vector<unicode>::difference_type;

			unicode_string(std::initializer_list<unicode> codes)
			{
				reserve(codes.size() + 1);
				_data.insert(_data.end(), codes.begin(), codes.end());
				_data.push_back(0);
			}

			void reserve(const size_type n)
			{
				_data.reserve(n);
			}

			void aappend(const unicode ch)
			{
				_data.insert(_data.end() - 1, ch);
			}

			template <typename... Character>
			void append(Character... code)
			{
				(append(code), ...);
			}

			void operator+=(const unicode ch)
			{
				append(ch);
			}

			void pop_back()
			{
				if (!empty())
					_data.erase(_data.end() - 2);
			}

			[[nodiscard]] size_type size() const noexcept
			{
				return _data.size() - 1;
			}

			[[nodiscard]] size_type capacity() const noexcept
			{
				return _data.capacity();
			}

			[[nodiscard]] bool empty() const noexcept
			{
				return _data.size() == 1;
			}

			[[nodiscard]] pointer data() noexcept
			{
				return _data.data();
			}

			[[nodiscard]] const_pointer data() const noexcept
			{
				return _data.data();
			}

			[[nodiscard]] iterator begin() noexcept
			{
				return _data.begin();
			}

			[[nodiscard]] const_iterator begin() const noexcept
			{
				return _data.begin();
			}
			
			[[nodiscard]] iterator end() noexcept
			{
				return _data.end() - 1;
			}

			[[nodiscard]] const_iterator end() const noexcept
			{
				return _data.end() - 1;
			}

			[[nodiscard]] reference at(const size_type idx)
			{
				return _data.at(idx);
			}

			[[nodiscard]] const_reference at(const size_type idx) const
			{
				return _data.at(idx);
			}

			[[nodiscard]] reference operator [](const size_type idx)
			{
				assert(idx < _data.size());
				return _data[idx];
			}

			[[nodiscard]] const_reference operator [](const size_type idx) const
			{
				assert(idx < _data.size());
				return _data[idx];
			}

		private:
			std::vector<unicode> _data;
		};
	}
}
