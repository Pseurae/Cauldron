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
    std::vector<int> vectorT = { 1, 2 }, vectorT1;
    std::string s = "Test";

    oarchive(vectorT, t{12}, s);

    for (auto &i : oarchive.GetData())
        std::cout << (unsigned int)i << std::endl;
        
    t t1{10};
    std::string s1;
    Tonic::Serialization::Getter iarchive(oarchive.GetData());

    std::cout << "Deserializing" << std::endl;
    iarchive(vectorT1, t1, s1);

    for (auto &i : vectorT1)
        std::cout << i << std::endl;

    std::cout << t1.i << std::endl;
    std::cout << s1 << std::endl;

    return 0;
}