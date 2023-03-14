#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

/*Define your Wallet class here*/

int notes_denom[8] = {2000, 500, 200, 100, 50, 20, 10, 5};
float coins_denom[7] = {20, 10, 5, 2, 1, 0.5, 0.25};

string notes_names[8] = {"2000", "500", "200", "100", "50", "20n", "10n", "5n"};
string coins_names[7] = {"20c", "10c", "5c", "2", "1", "0.5", "0.25"};

class Wallet
{
    private:
        int * notes; // notes
        int * coins; // coins
        double value;
        void calculate_value();
        void add_notes_coins(double d);
        void clear_notes_coins();
    
    public:
        Wallet();
        friend istream & operator >> (istream & in, Wallet & w);
        friend ostream & operator << (ostream & out, Wallet & w);
        operator double () const;
        int operator [] (double denom) const;
        void operator + (Wallet w);
        void operator + (double d);
        void operator - (Wallet w);
        void operator * (Wallet w);
        void operator * (double d);
};

void Wallet::calculate_value()
{
    value = 0;
    for(int i=0; i<8; i++) value += notes[i] * notes_denom[i];
    for(int i=0; i<7; i++) value += coins[i] * coins_denom[i];   
}

void Wallet::add_notes_coins(double d)
{
    for(int i=0; i<8; i++)
    {
        notes[i] += floor(d / notes_denom[i]);
        d -= ((floor(d / notes_denom[i])) * notes_denom[i]);
        if(d==0) return;
    }
    for(int i=0; i<7; i++)
    {
        coins[i] += floor(d / coins_denom[i]);
        d -= ((floor(d / coins_denom[i])) * coins_denom[i]);
        if(d==0) return;
    }
}

void Wallet::clear_notes_coins()
{
    for(int i=0; i<8; i++)
    {
        notes[i] = 0;
        if(i<7) coins[i] = 0;
    }
}

Wallet :: Wallet()
{
    notes = new int[8];
    coins = new int[7];   
    value = 0;
}

istream & operator >> (istream & in, Wallet & w)
{    
    for(int i=0; i<8; i++) in >> w.notes[i];
    for(int i=0; i<7; i++) in >> w.coins[i]; 
    
    w.calculate_value();
    
    return in;
}

ostream & operator << (ostream & out, Wallet & w)
{
    for(int i=0; i<8; i++) out << "(" << notes_names[i] << " " << w.notes[i] << ") ";
    for(int i=0; i<7; i++) out << "(" << coins_names[i] << " " << w.coins[i] << ") ";
    
    return out;
}

Wallet::operator double() const
{
    return value;
}

int Wallet::operator [] (double denom) const
{
    if(denom==2000) return notes[0];
    else if(denom==500) return notes[1];
    else if(denom==200) return notes[2];
    else if(denom==100) return notes[3];
    else if(denom==50) return notes[4];
    else if(denom==20) return (notes[5] + coins[0]);
    else if(denom==10) return (notes[6] + coins[1]);
    else if(denom==5) return (notes[7] + coins[2]);
    else if(denom==2) return coins[3];
    else if(denom==1) return coins[4];
    else if(denom==0.5) return coins[5];
    else if(denom==0.25) return coins[6];
    else return -1;
}

void Wallet::operator + (Wallet w)
{
    for(int i=0; i<8; i++)
    {
        notes[i] += w.notes[i];
        if(i<7)
        {
            coins[i] += w.coins[i];
        }
    }
    calculate_value();
}

void Wallet::operator + (double d)
{
    add_notes_coins(d);
    calculate_value();
}

void Wallet::operator - (Wallet w)
{
    for(int i=0; i<8; i++)
    {
        notes[i] -= w.notes[i];
        if(i<7)
        {
            coins[i] -= w.coins[i];
        }
    }
    calculate_value();
}

void Wallet::operator * (Wallet w)
{
    value *= w.value;
    clear_notes_coins();
    add_notes_coins(value);
}

void Wallet::operator * (double d)
{
    value *= d;
    clear_notes_coins();
    add_notes_coins(value);
}

int main()
{
    int N;
    cin >> N;
    Wallet w,w1;
    int command;
    double d;
    for (int i = 0; i < N; i++)
    {
        cin >> command;
        switch (command)
        {
            case 1: /*initialize wallet*/
                cin >> w;
                break;
            
            case 2: /*print wallet*/
                cout << "Wallet contains: " << w << endl;
                break;
            
            case 3: /*wallet balance*/
                cout << "Wallet has balance: " << (double) w << endl;
                break;
            
            case 4: /*denomination query*/
                cin >> d;
                cout << "Wallet contains " << w[d] << " number of notes/coins of denomination " << d << endl;
                break;
            
            case 5: /*add two wallets*/
                cin >> w1;
                w + w1;
                break;
            
            case 6: /*add money to wallet*/
                cin >> d;
                w + d;
                break;

            case 7: /*subtract from wallet*/
                cin >> w1;
                w - w1;
                break;
            
            case 8: /*multiply wallets*/
                cin >> w1;
                w * w1;
                break;
            
            case 9: /*multiply wallet and double*/
                cin >> d;
                w * d;
                break;

            default:
                break;
        }
    }
}