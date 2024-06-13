#include <iostream>
#include <vector>

int main() {
    std::string s = "2423243234432143223422432433213242324323432314324322323432310";
      //               5756576567765176556755765766526575657656765637657655656765640

	for(int i = 0; i < s.length(); i++) {
        if (s[i]== '0' || s[i] == '1') std::cout << s[i];
        else std::cout << (char) (s[i] + 3);
    }

    std::cout << std::endl;

    return 0;
} 
