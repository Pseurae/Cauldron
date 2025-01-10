#include <Tonic/Serialization.h>
#include <iostream>

struct t 
{
    int i = 2;

    void Serialize(Tonic::Serialization::Setter &s)
    {
        s(i);
    }

    void Deserialize(Tonic::Serialization::Getter &s)
    {
        s(i);
    }
};

int main(int argc, char *argv[])
{
    Tonic::Serialization::Setter oarchive;

    t t2;
    oarchive(1, 2, t2);

    for (auto &i : oarchive.GetData())
        std::cout << (unsigned int)i << std::endl;

    Tonic::Serialization::Getter iarchive(oarchive.GetData());
    int a, b;
    t t1;

    iarchive(a, b, t1);

    std::cout << (unsigned int)a << std::endl;
    std::cout << (unsigned int)b << std::endl;
    std::cout << (unsigned int)t1.i << std::endl;

    return 0;
}