#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <sys/select.h>


using namespace std;

map <int32_t, function<void(string)>> msgCenter;

void proc()
{


}
class Process
{
    public:
        void reg(){
            auto _cb = [this]( string msg ){
                cout << "[" << this->id << "] "<< "I received msg " << msg << endl;        
            };
            msgCenter.insert(make_pair(id, _cb));
        }

    private:
        int32_t id;

};
void test()
{

}

int main(int argc, char* argv[])
{

    test();


    return 0;
}
