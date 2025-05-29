#include "PCFG.h"
#include <chrono>
#include <fstream>
#include "md5.h"
#include <iomanip>
using namespace std;
using namespace chrono;

// 编译指令如下：
// g++ correctness.cpp train.cpp guessing.cpp md5.cpp -o main


// 通过这个函数，你可以验证你实现的SIMD哈希函数的正确性
// int main()
// {
//     bit32 state[4];
//     bit32 my_state[4][4];
//     MD5Hash("bvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdva", state);
//     My_MD5Hash("bvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdva", state);
//     for (int i1 = 0; i1 < 4; i1 += 1)
//     {
//         cout << std::setw(8) << std::setfill('0') << hex << state[i1];
//     }
//     for (int i1 = 0; i1 < 4; i1 += 1)
//     {
//         cout << std::setw(8) << std::setfill('0') << hex << my_state[i1];
//     }
//     cout << endl;
// }
int main()
{
    //bit32 state[4][4];
    bit32 state[4];
    bit32 my_state[4][4];
    string input[4] = {"hello98asd7","kasjc987wa","123sdw99s","kdsk9*&ll"};
    
    for (int i = 0; i < 4; i++){
        MD5Hash(input[i],state);
        for (int i1 = 0; i1 < 4; i1 += 1)
        {
        cout << std::setw(8) << std::setfill('0') << hex << state[i1];
        }
        cout<<endl;
    }
    cout<<endl;

    My_MD5Hash(input, my_state);
    for(int k = 0; k < 4; k++){
        for (int i1 = 0; i1 < 4; i1 += 1)
        {
         cout << std::setw(8) << std::setfill('0') << hex << my_state[k][i1];
        }
         cout<<endl;
    }
    /*for(int k = 0; k < 4; k++){
        for (int i1 = 0; i1 < 4; i1 += 1)
        {
        cout << std::setw(8) << std::setfill('0') << hex << state[k][i1];
        }
        cout<<endl;
    }
    for(int k = 0; k < 4; k++){
        for (int i1 = 0; i1 < 4; i1 += 1)
        {
        cout << std::setw(8) << std::setfill('0') << hex << my_state[k][i1];
        }
        cout<<endl;
    }*/
}