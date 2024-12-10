#ifndef _SAFESTK_H_
#define _SAFESTK_H_

struct Allocator {
    virtual void *AllocBytes(size_t size) = 0;
};

template <size_t Capacity>
struct SafeStackAllocator : public Allocator {
    SafeStackAllocator() : m_BufferSize(0), m_LinkedList(nullptr) {
    }
    ~SafeStackAllocator() {
        LinkedList *l = this->m_LinkedList;
        while (l != NULL) {
            LinkedList *n = l->Next;
            delete[] l;
            l = n;
        }
        this->m_BufferSize = 0;
        this->m_LinkedList = nullptr;
    }

    template <typename T> T *Alloc(size_t count) {
        return (T *)AllocBytes(sizeof(T) * count);
    }
    void *AllocBytes(size_t count) {
        if (count > ~0x8) {
            return NULL;
        }
        size_t stack_size = this->m_BufferSize;
        count = (count + 3) & ~0x3;
        if (count <= Capacity - stack_size) {
            this->m_BufferSize = stack_size + count;
            return this->m_Buffer + stack_size;
        }
        LinkedList *list = (LinkedList *) new char[sizeof(LinkedList) + count];
        if (list == NULL) {
            return nullptr;
        }
        list->Next = m_LinkedList;
        m_LinkedList = list;
        return list->Data;
    }
private:
    struct LinkedList {
        LinkedList *Next;
        unsigned char Data[];
    };
    size_t m_BufferSize;
    LinkedList *m_LinkedList;
    unsigned m_Buffer[Capacity];
};

#endif
