class HandlerBase{
    public:
        virtual void Process(void* param) = 0;
};

//Handler faz um objeto de uma classe qualquer chamar seu próprio método cadastrado.
//EX: se Cachorro tem os metodos void Latir(short) e void Brincar(short),
//Handler handler(dalmata, Latir);
//handler.Process(short); == dalmata.Latir(short);
template <typename TargetT>
class Handler: public HandlerBase{
    private:
        typedef void (TargetT::*method_t)(void*);
        TargetT* mObject;
        method_t mMethod;

    public:
        Handler(TargetT* object, method_t method)
            : mObject(object), mMethod(method) { }

        void Process(void* param) override
        {
            (mObject->*mMethod)(param);
        }
};

