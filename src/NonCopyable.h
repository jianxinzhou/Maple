#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H 

class NonCopyable
{
    public:
        NonCopyable();
        ~NonCopyable();

    private:
        NonCopyable(const NonCopyable &);
        void operator=(const NonCopyable &);
};

#endif  /*NONCOPYABLE_H*/
