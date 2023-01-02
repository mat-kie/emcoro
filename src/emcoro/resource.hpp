#ifndef MKCORO_RESOURCE_HPP
#define MKCORO_RESOURCE_HPP
#include <stddef.h>
#include "interface.hpp"
namespace emcoro
{

    /// @brief very simple linear allocator into static memory.
    /// After calling lock, the allocator will call its exception handler
    /// if a call to allocate happens.
    template <ExceptionHandler Error, size_t size_words>
    class SequentialAllocator
    {

        using word_t = size_t;

        static word_t m_storage[size_words];
        static size_t current_level;
        static bool m_isActive;

        /// @brief align and convert to words.
        /// @param num_bytes
        /// @return number of words needed to store the num_bytes
        static constexpr size_t bytesToWords(size_t num_bytes)
        {
            size_t rsw = num_bytes / sizeof(word_t);
            return (rsw * sizeof(word_t) == num_bytes) ? rsw : rsw + 1;
        }

    public:
        /// @brief lock the allocator to ensure no allocations can happen after this point.
        static void lock()
        {
            m_isActive = false;
        }

        /// @brief allocate storage for the required_size
        /// @param required_size size in bytes to allocate.
        /// @return pointer to reserved memory.
        static void *allocate(size_t required_size)
        {
            if (!m_isActive)
            {
                Error::fatal();
            }
            auto block_size_words = bytesToWords(required_size);
            if (block_size_words <= (size_words - current_level))
            {
                void *block_ptr = &(m_storage[current_level]);

                current_level += block_size_words;
                return block_ptr;
            }
            Error::fatal();
            return nullptr;
        }

        /// @brief mock for deallocation. actually does nothing since dynamic reallocation is not
        /// allowed for the usecase.
        /// @param data pointer to free...
        static void deallocate(void *data) {}
    };

    // initializing the static members.
    template <ExceptionHandler Error, size_t size_words>
    size_t SequentialAllocator<Error, size_words>::m_storage[size_words]{};
    template <ExceptionHandler Error, size_t size_words>
    size_t SequentialAllocator<Error, size_words>::current_level{};
    template <ExceptionHandler Error, size_t size_words>
    bool SequentialAllocator<Error, size_words>::m_isActive{true};
}
#endif
/// @file resource.hpp
/// @author Mattis Kieffer (mattis.kieffer@hotmail.de)
/// @brief contains a very simple linear allocator.
/// @version 1.0
/// @date 2023-01-02
/// @copyright Copyright (c) 2023
