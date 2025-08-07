class Ilimiter {
public:
    virtual ~Ilimiter() = default;
    virtual bool Allow() = 0;
    virtual bool TryReserve(int) = 0;
};