class Ilimiter {
public:
    virtual ~Ilimiter() = default;
    virtual bool Allow() = 0;
};