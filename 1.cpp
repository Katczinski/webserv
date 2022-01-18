#include <fstream>
#include <iostream>
#include <vector>
int main()
{
    long begin, end;
    std::ifstream input;
    input.open("/mnt/c/Users/Alex/Desktop/ft_server/web_serv_14_01/1.txt", std::ios::binary);
    input.seekg(0, (std::ios::end));
    end = input.tellg();
    input.seekg(2);
    begin = input.tellg();
    std::cout << end << " beg " << begin << std::endl;
    // input.close();
    std::vector<char> buffer(3000);
    std::string otvet;
    int how = 0;
    while(input)
    {
        input.read(&buffer[0], end);
        how = input.gcount();
        std::cout << "HOW " << how << std::endl;
        int i = 0;
        while(i < how)
            otvet += buffer[i++];
        
    }
    std::cout << otvet << std::endl;
}
