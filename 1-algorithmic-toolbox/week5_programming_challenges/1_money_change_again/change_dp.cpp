#include <iostream>

int32_t GetChange(int32_t money) {
  //write your code here
  return money / 4;
}

int32_t main() {
  int32_t money;
  std::cin >> money;
  std::cout << GetChange(money) << '\n';
}
