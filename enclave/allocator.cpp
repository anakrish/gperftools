// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/enclave.h>
// TODO: Uncomment this
// #include <openenclave/internal/allocator.h>
#include <tcmalloc.h>

extern "C"
{
    void* tc_malloc(size_t size);
    void tc_free(void* ptr);
    void* tc_realloc(void* ptr, size_t size);
    void* tc_calloc(size_t nmemb, size_t size);
    void* tc_memalign(size_t __alignment, size_t __size);
    int tc_posix_memalign(void** ptr, size_t align, size_t size);
    size_t tc_malloc_size(void* p);

    void* sbrk(ptrdiff_t increment);

    static uint8_t* _heap_start;
    static uint8_t* _heap_end;
    static uint8_t* _heap_next;
    static int _lock = 0;
    void* sbrk(ptrdiff_t increment)
    {
        void* ptr = (void*)-1;

        //    ACQUIRE_LOCK(&_lock);
        {
            ptrdiff_t remaining;

            if (!_heap_next)
                _heap_next = _heap_start;

            remaining = _heap_end - _heap_next;

            if (increment <= remaining)
            {
                ptr = _heap_next;
                _heap_next += increment;
            }
        }
        //    RELEASE_LOCK(&_lock);

        return ptr;
    }

    void oe_allocator_init(void* heap_start_address, void* heap_end_address)
    {
        _heap_start = static_cast<uint8_t*>(heap_start_address);
        _heap_end = static_cast<uint8_t*>(heap_end_address);
    }

    void oe_allocator_cleanup(void)
    {
    }

    void oe_allocator_thread_init(void)
    {
    }

    void oe_allocator_thread_cleanup(void)
    {
    }

    void* oe_allocator_malloc(size_t size)
    {
        return tc_malloc(size);
    }

    void oe_allocator_free(void* ptr)
    {
        tc_free(ptr);
    }

    void* oe_allocator_calloc(size_t nmemb, size_t size)
    {
        return tc_calloc(nmemb, size);
    }

    void* oe_allocator_realloc(void* ptr, size_t size)
    {
        return tc_realloc(ptr, size);
    }

    void* oe_allocator_aligned_alloc(size_t alignment, size_t size)
    {
        return tc_memalign(alignment, size);
    }

    int oe_allocator_posix_memalign(
        void** memptr,
        size_t alignment,
        size_t size)
    {
        return tc_posix_memalign(memptr, alignment, size);
    }

    size_t oe_allocator_malloc_usable_size(void* ptr)
    {
        return tc_malloc_size(ptr);
    }
}

extern "C"
{
    const int ENCLAVE_CPU_COUNT = 4;
    // From base/logging.cc
    // DEFINE_int32(verbose, EnvToInt("PERFTOOLS_VERBOSE", 0),
    //             "Set to numbers >0 for more verbose output, or <0 for less. "
    //             "--verbose == -4 means we log fatal errors only.");

    const char* TCMallocGetenvSafe(const char* name)
    {
        OE_UNUSED(name);
        return NULL;
    }

    int GetSystemCPUsCount()
    {
        return ENCLAVE_CPU_COUNT;
    }

    size_t write(int fd, const void* buf, size_t count)
    {
        OE_UNUSED(fd);
        OE_UNUSED(buf);
        OE_UNUSED(count);
        return 0;
    }

    int RunningOnValgrind(void)
    {
        return NULL;
    }

    char* getenv(const char* name)
    {
        OE_UNUSED(name);
        return NULL;
    }
}
