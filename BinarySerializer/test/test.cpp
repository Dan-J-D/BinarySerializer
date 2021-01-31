#include <iostream>

#include "../BinarySerializer.h"

class test : public Serializable
{
public:
    unsigned int a;
    const char* b;
    unsigned int b_size;

    test()
    {

    }

    test(unsigned int a, const char* b, unsigned int b_size)
    {
        this->a = a;
        this->b = (const char*)malloc(b_size);
        memcpy((void*)this->b, b, b_size);
        this->b_size = b_size;
    }

    ~test()
    {
        delete b;
    }

    unsigned char* Serialize(unsigned int* out_data_size)
    {
        /* Serialize Vars */
        BinarySerializer* ser = new BinarySerializer();
        ser->AddData((unsigned char*)&a, sizeof(unsigned int));
        ser->AddData((unsigned char*)b, b_size);

        /* Output Serialized Characters */
        unsigned char* data = (unsigned char*)malloc(ser->serialized_size);
        memcpy(data, ser->serialized, ser->serialized_size);
        *out_data_size = ser->serialized_size;
        delete ser;

        return data;
    }

    bool Deserialize(unsigned char* data, unsigned int data_size)
    {
        /* Deserialize data */
        BinaryDeserializer* bd = new BinaryDeserializer(data, data_size);
        unsigned int size = 0;

        /* Getting First Data Serialized*/
        unsigned char* d = bd->ReadData(&size);
        if (!d) { delete bd;  return false; }
        memcpy(&a, d, sizeof(unsigned int));

        /* Getting Second Data Serialized */
        d = bd->ReadData(&size);
        if (!d) { delete bd; return false; }
        b = (const char*)malloc(size);
        memcpy((void*)b, d, size);
        b_size = size;

        return true;
    }
};

int main()
{
    const char* msg = "test";

    test* t = new test(12, msg, 4);

    unsigned int size = 0;
    unsigned char* data = t->Serialize(&size);

    test* t1 = new test();

    if (t1->Deserialize(data, size))
    {
        fwrite(data, size, 1, stdout);
        std::cout << std::endl << t1->a << " ";
        fwrite(t1->b, t1->b_size, 1, stdout);
    }
}
