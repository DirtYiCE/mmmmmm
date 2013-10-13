#ifndef UUID_84382C52_8927_483A_974C_A0724E245669
#define UUID_84382C52_8927_483A_974C_A0724E245669
#pragma once

#include <memory>

template <typename T>
class ClonePtr
{
public:
    constexpr ClonePtr() noexcept = default;
    constexpr ClonePtr(std::nullptr_t) noexcept {}
    constexpr ClonePtr(T* ptr) noexcept : ptr(ptr) {}
    ClonePtr(const ClonePtr& p)
    {
        if (p.ptr) ptr = p.ptr->Clone().release();
    }
    ClonePtr(ClonePtr&& p) noexcept { std::swap(ptr, p.ptr); }
    ~ClonePtr() { reset(); }

    ClonePtr& operator=(const ClonePtr& p)
    {
        ClonePtr tmp(p);
        swap(tmp);
        return *this;
    }
    ClonePtr& operator=(ClonePtr&& p)
    {
        reset(p.release());
        return *this;
    }

    T* release() noexcept
    {
        T* ret = ptr;
        ptr = nullptr;
        return ret;
    }

    void reset(T* nptr = nullptr)
    {
        delete ptr;
        ptr = nptr;
    }

    void swap(ClonePtr& o) noexcept { std::swap(ptr, o.ptr); }
    T* get() const noexcept { return ptr; }
    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }
    explicit operator bool() const noexcept { return ptr; }

    template <typename U>
    bool operator==(const ClonePtr<U>& o) const noexcept { return ptr == o.ptr; }
    template <typename U>
    bool operator!=(const ClonePtr<U>& o) const noexcept { return ptr != o.ptr; }

private:
    T* ptr = nullptr;
};

template <typename T>
void swap(ClonePtr<T>& a, ClonePtr<T>& b)
{ a.swap(b); }

#endif
