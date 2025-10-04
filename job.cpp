#include <iostream>
#include <cmath> // for pow() and fmod()
using namespace std;

int main() {
    double num1, num2, result;
    char op;
    char choice;

    do {
        cout << "Enter h number: ";
        cin >> num1;

        cout << "Enter operator (+, -, *, /, ^, %): ";
        cin >> op;

        cout << "Enter second number: ";
        cin >> num2;

        switch(op) {
            case '+': 
                result = num1 + num2; 
                break;
            case '-': 
                result = num1 - num2; 
                break;
            case '*': 
                result = num1 * num2; 
                break;
            case '/':
                if(num2 != 0)
                    result = num1 / num2;
                else {
                    cout << "❌ Error! Division by zero.\n";
                    return 1;
                }
                break;
            case '^': // power feature
                result = pow(num1, num2);
                break;
            case '%': // modulus (for integers)
                if((int)num2 != 0)
                    result = fmod(num1, num2);
                else {
                    cout << "❌ Error! Modulus by zero.\n";
                    return 1;
                }
                break;
            default:
                cout << "❌ Invalid operator!\n";
                return 1;
        }

        cout << "✅ Result: " << result << endl;

        cout << "\nDo you want to perform another calculation? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    cout << "👋 Thank you for using the calculator!\n";
    return 0;
}
