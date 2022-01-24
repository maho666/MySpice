#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <bits/stdc++.h>///for reverse
#include <cmath>
#include <sstream>
using namespace std;


struct component//////////////create para
{
string resister;
string head;
string tail;
string value;
};
void input_file(vector <component> temp,vector <vector<component>> &circuit,string &stage,int &stage_num)
{
    string input_name;////name of input data
    component data_get;
    circuit.clear();

    cout << "Please enter the name of the input file: ";
    cin >> input_name;
    ifstream inClientFile(input_name,ios::in);
    while(!inClientFile)
    {
        cerr << "The file is not found."<<endl;
        cout << "Please enter the name of the input file again: ";
        cin >> input_name;
        inClientFile.open(input_name,ios::in);
        if(inClientFile)
        {
            break;
        }

    }
    cout << "Loading the file..."<<endl;
    inClientFile >> stage >> stage_num;

    for(int i = 0; i < 50;i++)
    {
    inClientFile >> data_get.resister>>data_get.head>>data_get.tail>>data_get.value;

    if(i==0)
    {
    temp.push_back(data_get);
    }

    if(data_get.resister == "v1")
    {
        circuit.push_back(temp);
        temp.clear();
    }

    else if(i>1&&data_get.resister[1]!= temp[0].resister[1])
        {
            circuit.push_back(temp);
            temp.clear();
        }
        else if(i>1&&data_get.resister[2]!= temp[0].resister[2])
        {
            circuit.push_back(temp);
            temp.clear();
        }

        if(i>0&&data_get.resister != "end")
        {
        temp.push_back(data_get);
        }
        if (data_get.resister == "end")
        {
            circuit.push_back(temp);
            temp.clear();
            break;
        }
    }
    cout << "The input file successfully loaded!" <<endl;
}
void unit_value(double &temp_value,char unit)
{
    switch (unit)
        {
            case 'u':
                {
                    temp_value = 0.000001*temp_value;
                    break;
                }
            case 'm':
                {
                    temp_value = 0.001*temp_value;
                    break;
                }
            case 'K':
                {
                    temp_value = 1000*temp_value;
                    break;
                }
            case 'g':
                {
                    temp_value = 100000*temp_value;
                    break;
                }
            case 'G':
                {
                    temp_value = 1000000000*temp_value;
                    break;
                }
            default:
                {
                    temp_value = temp_value;
                    break;
                }
        }
}
double rounding(double num, int index)
{

    bool isNegative = false; // whether is negative number or not

    if(num < 0) // if this number is negative, then convert to positive number
    {
        isNegative = true;
        num = -num;
    }

    if(index >= 0)
    {
        int multiplier;
        multiplier = pow(10, index);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }

    if(isNegative) // if this number is negative, then convert to negative number
    {
        num = -num;
    }

    return num;
}


string unit_change(double v,int j)////change unit if out of the range 1~999.999
{
    string v_unit ;
    if(v>=1.000&&v<999.999)
    {

        v = rounding(v,3);
        ostringstream out;
        out.precision(3);
        out << fixed << v;
        v_unit = out.str();
        switch (j)
        {
        case 0:
            return v_unit ;
            break;
        case -1:
            return v_unit.append("m");
            break;
        case -2:
            return v_unit.append("u");
            break;
        case 1:
            return v_unit.append("K");
            break;
        case 2:
            return v_unit.append("Meg");
            break;
        case 3:
            return v_unit.append("G");
            break;
        default:
            return v_unit;
            break;
        }
    }

    else if(v<1)
    {
        v = v*1000;
        j--;
        return unit_change(v,j);
    }
    else if(v>999.999)
    {
        v = v/1000;
        j++;
        return unit_change(v,j);
    }

}

void calculate(vector <vector<component>> &circuit,int &stage_num,vector<string> &voltage_unit,vector<string> &current_unit)
{
    double value_no_unit,temp_value,temp_value2,temp_voltage,temp_current;
    vector<double> one_value,new_value;
    vector<double> voltage,current;
    voltage.clear();
    voltage_unit.clear();
    current.clear();
    current_unit.clear();
    new_value.clear();
    one_value.clear();
    char unit;
    //////////get vdd///////////
    circuit[0][0].value[circuit[0][0].value.size()-2];////get unit
    temp_voltage = stod(circuit[0][0].value);
    unit_value(temp_voltage,unit);
    voltage.push_back(temp_voltage);

    double first_value = temp_voltage;//get vdd into circuit
    circuit[0][0].value = unit_change(first_value,0);
    circuit[0][0].value.append("V");

    cout<<"Calculating..."<<endl;
    for(int i=1;i<circuit.size();i++)//////////////////combine into 1 in each branch order
    {
        value_no_unit = 0;
        for(int j=0;j<circuit[i].size();j++)
        {
        unit = circuit[i][j].value[circuit[i][j].value.size()-1];/////get unit
        temp_value = stod(circuit[i][j].value);
        unit_value(temp_value,unit);/////get value in decimal
        value_no_unit = value_no_unit + temp_value;
        }
        one_value.push_back(value_no_unit);

    }

    temp_value2 = one_value[one_value.size()-2];
    new_value.push_back(temp_value2);
    if(one_value.size()>2)
    {
        for(int i = one_value.size()-2;i>0;i--)/////////combine all R into 1 ~order(parallel serial)/////here -2 for no merging  -1 for merged (ry
        {
            //cout << "llllllllllllllllll"<<endl;
            if(i%2==1)
            {
                temp_value2 = one_value[i-1]+temp_value2;
                //cout << "temp2:   "<<temp_value2<<endl;
            }
            else if(i%2==0)
            {
                temp_value2 = (one_value[i-1]*temp_value2)/(one_value[i-1]+temp_value2);
            }
        new_value.push_back(temp_value2);
        }
    }
    else if(one_value.size()==2)
    {
        for(int i = one_value.size()-1;i>0;i--)/////////combine all R into 1 ~order(parallel serial)/////here -2 for no merging  -1 for merged (ry
        {
            //cout << "llllllllllllllllll"<<endl;
            if(i%2==1)
            {
                temp_value2 = one_value[i-1]+temp_value2;
                //cout << "temp2:   "<<temp_value2<<endl;
            }
            else if(i%2==0)
            {
                temp_value2 = (one_value[i-1]*temp_value2)/(one_value[i-1]+temp_value2);
            }
        new_value.push_back(temp_value2);
        }
    }

    reverse(new_value.begin(),new_value.end());///////reverse the order


    for(int i = 0;i<new_value.size();i++)
    {
        if(i%2==1)
        {
            temp_current = temp_voltage/one_value[i];////down
            current.push_back(temp_current);
            if(i+1<new_value.size())
            {
            temp_current = temp_voltage/new_value[i+1];/////right
            current.push_back(temp_current);
            }
            if(((stage_num-1)*2-1)==i)//////last stage
            {
                temp_voltage = temp_voltage-temp_current*one_value[i+1];
                voltage.push_back(temp_voltage);
                break;
            }
            continue;
        }
        if(i== 0)
        {
            temp_current = temp_voltage/new_value[i];
            current.push_back(temp_current);
        }
        temp_voltage = temp_voltage-temp_current*one_value[i];
        voltage.push_back(temp_voltage);

    }

  //////////盢one_value眔筿瑈临倒甶秨R/////////////////
    int arr=0;
    for (int i = 1;i<circuit.size()-1;i++)
    {
        //cout << "arr : "<<arr<<endl;
        //cout << i <<" *******"<<endl;
        for(int j=0;j<circuit[i].size();j++)
        {
            if(j==circuit[i].size()-1)//
            {
                break;
            }
            if(i==1 || 2*stage_num != i)
            {
                current.insert(current.begin()+arr,current[arr]);
            }
            else if(2*stage_num == i)/////程stageseries
            {
                current.insert(current.begin()+arr,current[arr-1]);/////add infront
            }

            arr++;
        }
        arr++;
    }
    arr--;
    for(int j = 0;j<circuit[circuit.size()-2].size();j++)///////add the last one
    {
        current.push_back(current[arr-1]);/////add end
        arr++;
    }

    for(int i = 0 ; i<voltage.size();i++)
    {
        //cout<<"11222"<<endl;
            //cout << "122"<<endl;
            string a = unit_change(voltage[i],0);
            voltage_unit.push_back(a);
            //cout << "1"<<endl;
    }


    for(int i = 0 ; i<current.size();i++)
    {
        //cout<<"current11222"<<endl;
            //cout<<"in current11222"<<endl;
            string b = unit_change(current[i],0);
            //cout <<"b:    "<<b<<endl;
            current_unit.push_back(b);
            //cout << "me: "<<current_unit[i];

    }

    cout <<"The result is successfully calculated!"<<endl;
    //cout << "***calculation results***"<<endl;
    cout <<"**************************************"<<endl;
    cout <<"**          <<voltage>>             **"<<endl;

    for(int i =0;i<voltage_unit.size();i++)
    {
        if(i==0)
        {
            cout <<"**   vdd"<<setw(13)<<right<<fixed<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(16)<<"**"<<endl;
        }
        else
            {
            if(i<10)
            {
                //outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                if(voltage_unit[i].size()==6)
                {
                   cout <<"**   n"<<i<<setw(15)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(15)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==9)
                {
                    cout <<"**   n"<<i<<setw(18)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(12)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==8)
                {
                    cout <<"**   n"<<i<<setw(17)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(13)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==7)
                {
                    cout <<"**   n"<<i<<setw(16)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(14)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==5)
                {
                    cout <<"**   n"<<i<<setw(14)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(16)<<right<<"**"<<endl;
                }
            }
            else
            {
                if(voltage_unit[i].size()==6)
                {
                   cout <<"**   n"<<i<<setw(14)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(16)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==9)
                {
                    cout <<"**   n"<<i<<setw(17)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(13)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==8)
                {
                    cout <<"**   n"<<i<<setw(16)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(14)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==7)
                {
                    cout <<"**   n"<<i<<setw(15)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(15)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==5)
                {
                    cout <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(17)<<right<<"**"<<endl;
                }
            }

            }

    }
    cout <<"**          <<current>>             **"<<endl;
    int i_current=0;
    for (int i = 1;i<circuit.size()-1;i++)
    {
        for(int j=0;j<circuit[i].size();j++)
        {
            if(circuit[i][j].resister.size()==5)
            {
                //cout <<"**   "<<circuit[i][j].resister<<setw(6)<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<"**"<<endl;
                if(current_unit[i_current].size()==9)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(12)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==8)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==7)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(14)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==6)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                }
            }
            else if(circuit[i][j].resister.size()==3)
            {
                //cout <<"**   "<<circuit[i][j].resister<<setw(8)<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<"**"<<endl;
                if(current_unit[i_current].size()==9)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(12)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==8)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==7)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(14)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==6)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                }
            }
            else
            {
                //cout <<"**   "<<circuit[i][j].resister<<setw(8)<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<"**"<<endl;
                if(current_unit[i_current].size()==9)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(12)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==8)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==7)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(14)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==6)
                {
                    cout <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                }
            }
            i_current++;
        }

    }
    cout<<"**"<<setw(36)<<"**"<<endl;
    cout<<"**"<<setw(29)<<"myspice2021"<<setw(7)<<"**"<<endl;
    cout <<"**************************************"<<endl;

}
void swap_stage(vector <vector<component>> &circuit,int &stage_num)
{
    int first_stage,sec_stage;
    string fore_head_1,fore_head_2;
    do{
    cout << "Please enter the first stage you want to swap: ";
    cin >> first_stage;
    if(first_stage >stage_num || first_stage < 1)
        cout << "The node doesn't exist. Please enter again."<<endl;
    }while(first_stage >stage_num || first_stage < 1);
    do{
    cout << "Please enter the second stage you want to swap: ";
    cin >> sec_stage;
    if(sec_stage >stage_num|| sec_stage < 1)
        cout << "The node doesn't exist. Please enter again."<<endl;
    }while(sec_stage >stage_num|| sec_stage < 1);
    fore_head_1 = circuit[2*first_stage-1][0].head;
    fore_head_2 = circuit[2*sec_stage-1][0].head;
    if(stage_num != 3)
    {
    for(int i = 0;i<2;i++)////传branch 禲0 1 change resistor
    {
        //cout << "changing i: "<<i<<endl;
        //cout << "i: "<<i<<endl;
        //cout <<"circuit[first_stage+i].size(): "<<circuit[first_stage+i].size()<<endl;
        //cout <<"circuit[first_stage+i+2].size(): "<<circuit[sec_stage+i+2].size()<<endl;
        if(circuit[(2*first_stage-1)+i].size()==circuit[(2*first_stage-1)+i+2].size())//////equal length
            {
                //cout << "1"<<endl;
                for(int j = 0;j<circuit[first_stage+i].size();j++)
                {
                    //cout << "2"<<endl;
                    swap(circuit[(2*first_stage-1)+i][j].resister,circuit[(2*first_stage-1)+i+2][j].resister);
                    swap(circuit[(2*first_stage-1)+i][j].tail,circuit[(2*first_stage-1)+i+2][j].tail);
                    swap(circuit[(2*first_stage-1)+i][j].value,circuit[(2*first_stage-1)+i+2][j].value);
                }

            }
        else if(circuit[(2*first_stage-1)+i].size()>circuit[(2*first_stage-1)+i+2].size())/////front < back
        {
            int res = 0;
            for(int j = 0;j<circuit[(2*first_stage-1)+i+2].size();j++)
            {
                    swap(circuit[(2*first_stage-1)+i][j].resister,circuit[(2*first_stage-1)+i+2][j].resister);
                    swap(circuit[(2*first_stage-1)+i][j].tail,circuit[(2*first_stage-1)+i+2][j].tail);
                    swap(circuit[(2*first_stage-1)+i][j].value,circuit[(2*first_stage-1)+i+2][j].value);
                    //cout << "1: "<<circuit[(2*first_stage-1)+i][j].resister<<"\n"<<"2: "<<circuit[(2*first_stage-1)+i+2][j].resister<<endl;
                    res = j+1;
            }
            int k = res;
            while(res < circuit[first_stage+i].size())
            {
                circuit[(2*first_stage-1)+i+2].push_back(circuit[(2*first_stage-1)+i][k]);
                circuit[(2*first_stage-1)+i].pop_back();
                res++;
            }
        }
        else if(circuit[first_stage+i].size()<circuit[first_stage+i+2].size())/////要交換的前者長度較大時
        {
            int res = 0;
            //cout <<"相加"<<(first_stage+i+1)<<endl;
            for(int j = 0;j<circuit[first_stage+i].size();j++)
            {
                    //cout << "44444444444444444444444444444//"<<i<<endl;
                    swap(circuit[first_stage+i+2][j].resister,circuit[first_stage+i][j].resister);
                    swap(circuit[first_stage+i+2][j].tail,circuit[first_stage+i][j].tail);
                    swap(circuit[first_stage+i+2][j].value,circuit[first_stage+i][j].value);
                    if(j==0)
                    {
                        //cout << "4-1"<<endl;
                        //cout <<"i: "<<i<<endl;
                        if((first_stage+i+1)%2==0 /*|| (first_stage+i+1)%2==0*/)//偶數
                        {
                            //cout << "4-2"<<endl;
                            circuit[first_stage+i+1][j].head = circuit[first_stage+i][j].tail;
                        }
                        else if((first_stage+i+1)%2==1 /*|| (first_stage+i+1)%2==0*/)//奇數
                        {
                            //cout << "4-3"<<endl;
                            circuit[first_stage+i+1][j].head = circuit[first_stage+i-1][j].tail;
                        }
                    }
                    res = j+1;
            }
            int k = res;
            while(res < circuit[first_stage+i+2].size())
            {
                circuit[first_stage+i].push_back(circuit[first_stage+i+2][k]);
                circuit[first_stage+i+2].pop_back();
                res++;
            }
        }
        for(int i = 0;i<2;i++)////change the head
        {

        if(circuit[(2*first_stage-1)+i].size()>circuit[(2*first_stage-1)+i+2].size())/////front < back
        {
            //int a = 0;
            for(int j = 0;j<circuit[(2*first_stage-1)+i+2].size();j++)
            {
                    if(j==0)/////change head in each first component
                    {   //cout << "3-1"<<endl;

                        if(((2*first_stage-1)+i+1)%2==0/*|| (first_stage+i+1)%2==0*/)//2n 2~3 2
                        {
                            //cout << "3-2 "<<(circuit[(2*first_stage-1)+i].size())<<endl;
                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i][circuit[(2*first_stage-1)+i].size()-2].tail;
                        }
                        else if(((2*first_stage-1)+i+1)%2==1 /*|| (first_stage+i+1)%2==1*/)//2n-1 2~3 3
                        {
                            //cout << "3-3 "<< " "<<(circuit[(2*first_stage-1)+i].size())<<endl;

                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i-1][circuit[(2*first_stage-1)+i].size()-2].tail;
                        }

                    }

            }
        }
        else if(circuit[(2*first_stage-1)+i].size()<circuit[(2*first_stage-1)+i+2].size())/////front > back
        {
            //cout <<"addition"<<((2*first_stage-1)+i+1)<<endl;
            for(int j = 0;j<circuit[(2*first_stage-1)+i].size();j++)
            {
                    if(j==0)
                    {
                        //cout << "4-1"<<endl;
                        //cout <<"i: "<<i<<endl;
                        if(((2*first_stage-1)+i+1)%2==0 /*|| (first_stage+i+1)%2==0*/)//2n
                        {
                            //cout << "4-2 "<<(circuit[(2*first_stage-1)+i].size())<<endl;
                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i][circuit[(2*first_stage-1)+i].size()-1].tail;
                        }
                        else if(((2*first_stage-1)+i+1)%2==1 /*|| (first_stage+i+1)%2==0*/)//2n-1
                        {
                            //cout << "4-3 "<<(circuit[(2*first_stage-1)+i].size())<<endl;
                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i-1][circuit[(2*first_stage-1)+i].size()-1].tail;
                        }
                    }
            }
        }
        }
//////只要跑一次 最後一階 的最上面 應該不用for 可以使用固定數字(?
        for(int i = 0;i<circuit[2*sec_stage-1].size();i++)
        {
            circuit[2*sec_stage][0].head = circuit[2*sec_stage-1][i].tail;
            //cout << "last position in : "<<i<<" "<<circuit[2*sec_stage][0].head<<endl;
        }



    }
    /*for (int i = 0;i<circuit.size();i++)////prompt 0~5 for input1.txt
    {

        cout << "*******"<<endl;
        for(int j=0;j<circuit[i].size();j++)
        {
        cout << "*******"<<endl;
        cout << left<<"i: "<<i<<" j: "<<j<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<circuit[i][j].tail<<setw(7)<<circuit[i][j].value<<endl;
        //outClientFile << left<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<circuit[i][j].tail<<setw(7)<<circuit[i][j].value<<endl;
        }
    }*/
        cout <<circuit[2*first_stage-1][0].resister<<" is changing node from "<< fore_head_1<<" to "<<circuit[2*first_stage-1][0].tail<<"..."<<endl;
        cout <<circuit[2*first_stage][0].resister<<" is changing node from "<< fore_head_2<<" to "<<circuit[2*first_stage-1][0].head<<"..."<<endl;

    }
    else if(stage_num == 3)
    {
        for(int i = 0;i<2;i++)////传branch 禲0 1 change resistor
    {
        //cout << "changing i: "<<i<<endl;
        //cout << "i: "<<i<<endl;
        //cout <<"circuit[first_stage+i].size(): "<<circuit[first_stage+i].size()<<endl;
        //cout <<"circuit[2*(first_stage-1)+i+2].size(): "<<circuit[2*(first_stage-1)+i+2].size()<<endl;
        //cout <<"circuit[2*(first_stage-1)+i].size(): "<<circuit[2*(first_stage-1)+i].size()<<endl;
        if(circuit[(2*first_stage-1)+i].size()==circuit[(2*first_stage-1)+i+2].size())//////equal length
            {
                //cout << "1"<<endl;
                for(int j = 0;j<circuit[first_stage+i].size();j++)
                {
                    //cout << "2"<<endl;
                    swap(circuit[(2*first_stage-1)+i][j].resister,circuit[(2*first_stage-1)+i+2][j].resister);
                    swap(circuit[(2*first_stage-1)+i][j].tail,circuit[(2*first_stage-1)+i+2][j].tail);
                    swap(circuit[(2*first_stage-1)+i][j].value,circuit[(2*first_stage-1)+i+2][j].value);

                }

            }
        else if(circuit[(2*first_stage-1)+i].size()>circuit[(2*first_stage-1)+i+2].size())/////front < back
        {
            int res = 0;
            for(int j = 0;j<circuit[(2*first_stage-1)+i+2].size();j++)
            {
                    swap(circuit[(2*first_stage-1)+i][j].resister,circuit[(2*first_stage-1)+i+2][j].resister);
                    swap(circuit[(2*first_stage-1)+i][j].tail,circuit[(2*first_stage-1)+i+2][j].tail);
                    swap(circuit[(2*first_stage-1)+i][j].value,circuit[(2*first_stage-1)+i+2][j].value);
                    //cout << "1: "<<circuit[(2*first_stage-1)+i][j].resister<<"\n"<<"2: "<<circuit[(2*first_stage-1)+i+2][j].resister<<endl;
                    res = j+1;
            }
            int k = res;
            while(res < circuit[first_stage+i].size())
            {
                circuit[(2*first_stage-1)+i+2].push_back(circuit[(2*first_stage-1)+i][k]);
                circuit[(2*first_stage-1)+i].pop_back();
                res++;
            }
        }
        else if(circuit[first_stage+i].size()<circuit[first_stage+i+2].size())/////要交換的前者長度較大時
        {
            int res = 0;
            //cout <<"相加"<<(first_stage+i+1)<<endl;
            for(int j = 0;j<circuit[first_stage+i].size();j++)
            {
                    //cout << "44444444444444444444444444444//"<<i<<endl;
                    swap(circuit[first_stage+i+2][j].resister,circuit[first_stage+i][j].resister);
                    swap(circuit[first_stage+i+2][j].tail,circuit[first_stage+i][j].tail);
                    swap(circuit[first_stage+i+2][j].value,circuit[first_stage+i][j].value);
                    if(j==0)
                    {
                        //cout << "4-1"<<endl;
                        //cout <<"i: "<<i<<endl;
                        if((first_stage+i+1)%2==0 /*|| (first_stage+i+1)%2==0*/)//偶數
                        {
                            //cout << "4-2"<<endl;
                            circuit[first_stage+i+1][j].head = circuit[first_stage+i][j].tail;
                        }
                        else if((first_stage+i+1)%2==1 /*|| (first_stage+i+1)%2==0*/)//奇數
                        {
                            //cout << "4-3"<<endl;
                            circuit[first_stage+i+1][j].head = circuit[first_stage+i-1][j].tail;
                        }
                    }
                    res = j+1;
            }
            int k = res;
            int d = circuit[first_stage+i+2].size();
            while(res < d)
            {
                //cout << "circuit[first_stage+i+2][k]: "<<res<<" R:  "<<circuit[first_stage+i+2][res].resister<<endl;
                circuit[first_stage+i].push_back(circuit[first_stage+i+2][res]);
                circuit[first_stage+i+2].pop_back();
                res++;
            }
        }

        for(int i = 0;i<2;i++)////change the head
        {

        if(circuit[(2*first_stage-1)+i].size()>circuit[(2*first_stage-1)+i+2].size())/////front < back
        {
            //int a = 0;
            for(int j = 0;j<circuit[(2*first_stage-1)+i+2].size();j++)
            {
                    if(j==0)/////change head in each first component
                    {   //cout << "3-1"<<endl;

                        if(((2*first_stage-1)+i+1)%2==0/*|| (first_stage+i+1)%2==0*/)//2n 2~3 2
                        {
                            //cout << "3-2 "<<(circuit[(2*first_stage-1)+i].size())<<endl;
                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i][circuit[(2*first_stage-1)+i].size()-2].tail;
                        }
                        else if(((2*first_stage-1)+i+1)%2==1 /*|| (first_stage+i+1)%2==1*/)//2n-1 2~3 3
                        {
                            //cout << "3-3 "<< " "<<(circuit[(2*first_stage-1)+i].size())<<endl;

                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i-1][circuit[(2*first_stage-1)+i].size()-2].tail;
                        }

                    }

            }
        }
        else if(circuit[(2*first_stage-1)+i].size()<circuit[(2*first_stage-1)+i+2].size())/////front > back
        {
            //cout <<"addition"<<((2*first_stage-1)+i+1)<<endl;
            for(int j = 0;j<circuit[(2*first_stage-1)+i].size();j++)
            {
                    if(j==0)
                    {
                        //cout << "4-1"<<endl;
                        //cout <<"i: "<<i<<endl;
                        if(((2*first_stage-1)+i+1)%2==0 /*|| (first_stage+i+1)%2==0*/)//2n
                        {
                            //cout << "4-2 "<<(circuit[(2*first_stage-1)+i].size())<<endl;
                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i][circuit[(2*first_stage-1)+i].size()-1].tail;
                        }
                        else if(((2*first_stage-1)+i+1)%2==1 /*|| (first_stage+i+1)%2==0*/)//2n-1
                        {
                            //cout << "4-3 "<<(circuit[(2*first_stage-1)+i].size())<<endl;
                            circuit[(2*first_stage-1)+i+1][j].head = circuit[(2*first_stage-1)+i-1][circuit[(2*first_stage-1)+i].size()-1].tail;
                        }
                    }
            }
        }
        }

        circuit[2][0].head = circuit[1][0].tail;
        circuit[3][0].head = circuit[1][0].tail;
//////add the tail
        for(int i = 0;i<circuit[2*sec_stage-1].size();i++)
        {
            circuit[2*sec_stage][0].head = circuit[2*sec_stage-1][i].tail;
            //cout << "last position in : "<<i<<" "<<circuit[2*sec_stage][0].head<<endl;
        }



    }
    if (2*first_stage+4 == circuit.size()&&2*sec_stage+2 == circuit.size())
        {
            //cout << "circuit[2*first_stage].size(): "<<circuit[2*first_stage].size()<<endl;
            for(int i = 2*first_stage;i<2*sec_stage;i++)
            {
                if(i == 2*first_stage)
                {
                    swap(circuit[2*first_stage][0].resister,circuit[2*sec_stage][0].resister);
                    swap(circuit[2*first_stage][0].tail,circuit[2*sec_stage][0].tail);
                    //swap(circuit[2*first_stage][0].tail,circuit[2*sec_stage][0].tail);
                    circuit[2*first_stage][0].head=circuit[2*sec_stage-1][2].tail;
                    circuit[2*sec_stage-1][0].head=circuit[2*first_stage][0].head;
                    //circuit[2*first_stage].push_back(circuit[2*sec_stage][1]);
                    circuit[2*sec_stage].pop_back();
                    //cout << "asasadsad   i: "<<i<<endl;
                    circuit[2*first_stage].push_back(circuit[2*sec_stage][1]);
                }
            }

        }
        cout <<circuit[5][0].resister<<" is changing node from "<< circuit[0][0].head<<" to "<<circuit[3][0].head<<"..."<<endl;
        cout <<circuit[1][0].resister<<" is changing node from "<< circuit[6][0].head<<" to "<<circuit[0][0].head<<"..."<<endl;
        cout <<circuit[3][0].resister<<" is changing node from "<< circuit[3][0].head<<" to "<<circuit[6][0].head<<"..."<<endl;
    /*for (int i = 0;i<circuit.size();i++)////prompt 0~5 for input1.txt
    {

        cout << "*******"<<endl;
        for(int j=0;j<circuit[i].size();j++)
        {
        cout << "*******"<<endl;
        cout << left<<"i: "<<i<<" j: "<<j<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<circuit[i][j].tail<<setw(7)<<circuit[i][j].value<<endl;
        //outClientFile << left<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<circuit[i][j].tail<<setw(7)<<circuit[i][j].value<<endl;
        }
    }*/


    }
        cout <<"The stage is Successfully swapped!"<<endl;

}
void merge_stage(vector <vector<component>> &circuit,int &stage_num)
{
    int first_stage,sec_stage;
    double value_no_unit,temp_value,temp_value2;
    vector<double> one_value,new_value;
    char unit;
    do{
    cout << "Please enter the first stage you want to merge: ";
    cin >> first_stage;
    if(first_stage >stage_num || first_stage < 1)
        cout << "The node doesn't exist. Please enter again."<<endl;
    }while(first_stage >stage_num || first_stage < 1);
    do{
    cout << "Please enter the second stage you want to merge: ";
    cin >> sec_stage;
    if(sec_stage >stage_num|| sec_stage < 1)
        cout << "The node doesn't exist. Please enter again."<<endl;
    }while(sec_stage >stage_num|| sec_stage < 1);
    if(first_stage==stage_num || sec_stage==stage_num)//last 2 stage
    {
        for(int i=2*sec_stage-2;i<2*sec_stage+1;i++)//////////////////combine into 1 in each branch order
        {
            value_no_unit = 0;
            //temp_value = 0;
            for(int j=0;j<circuit[i].size();j++)
            {
                unit = circuit[i][j].value[circuit[i][j].value.size()-1];/////get unit
                temp_value = stod(circuit[i][j].value);//get value

                unit_value(temp_value,unit);/////get value in decimal
                //cout << i<<" "<<j<<" "<<" temp "<< temp_value<<endl;
                value_no_unit = value_no_unit + temp_value;
                //cout << "unit: "<<unit<<" value: "<< value_no_unit<<endl;
            }
            one_value.push_back(value_no_unit);
        }
        /*for(int i = 0;i<one_value.size();i++)
        {
            cout << "one_value "<<one_value[i]<<endl;
        }*/
        temp_value2 = one_value[one_value.size()-1];
        new_value.push_back(temp_value2);
        //cout << "size of new: "<<new_value.size()<<endl;
        //cout << "size of one: "<<one_value.size()<<endl;
        //cout << "size of new: "<<new_value.size()<<endl;
        for(int i = one_value.size()-2;i>=0;i--)/////////combine all R into 1 ~order(parallel serial)
        {
            //cout << "llllllllllllllllll"<<endl;
            if(i%2==1)
            {
                temp_value2 = one_value[i]+temp_value2;
            }
            else if(i%2==0)
            {
                temp_value2 = (one_value[i]*temp_value2)/(one_value[i]+temp_value2);
            }
            new_value.push_back(temp_value2);
        }

        reverse(new_value.begin(),new_value.end());
        /*for(int i = 0;i<new_value.size();i++)
        {
            cout << "nw_value "<<new_value[i]<<endl;
        }*/
        circuit.erase(circuit.begin()+2*sec_stage-2,circuit.begin()+2*sec_stage);////delete range between fir and sec
        circuit[2*sec_stage-2].erase(circuit[2*sec_stage-2].begin()+1,circuit[2*sec_stage-2].begin()+circuit[2*sec_stage-2].size());/////////////delete which on the last branch


    }

    /////////////////////////////middle merge/////////////////////////////////


    ////change element
    string value_unit = unit_change(new_value[0],0);//get unit name head(no change) tail value
    circuit[2*sec_stage-2][0].value = value_unit;//value
    circuit[2*sec_stage-2][0].tail = circuit[2*sec_stage-4][circuit[2*sec_stage-4].size()-1].tail;
    circuit[2*first_stage-1][circuit[2*first_stage-1].size()-1].tail = circuit[2*sec_stage-2][0].head;
    string  new_resister;
    //new_resister.append(circuit[2*sec_stage-2][0].head,1,circuit[2*sec_stage-2][0].head.size()-1);
    new_resister.append(to_string(first_stage));
    circuit[2*sec_stage-2][0].resister = "RP";
    circuit[2*sec_stage-2][0].resister.append(new_resister);
    cout << "Resisters are merged into RP"<<first_stage<<"."<<endl;
    cout << "=> RP"<<first_stage<<" "<<circuit[2*sec_stage-2][0].head<<" "<<circuit[2*sec_stage-2][0].tail<<" "<<circuit[2*sec_stage-2][0].value<<endl;
    cout<<"The stage is Successfully merged!"<<endl;
    /////////
    /*for (int i = 0;i<circuit.size();i++)////prompt 0~5 for input1.txt
    {

        cout << "*******"<<endl;
        for(int j=0;j<circuit[i].size();j++)
        {
        cout << "*******"<<endl;
        cout << left<<"i: "<<i<<" j: "<<j<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<circuit[i][j].tail<<setw(7)<<circuit[i][j].value<<endl;
        //outClientFile << left<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<circuit[i][j].tail<<setw(7)<<circuit[i][j].value<<endl;
        }
    }*/

    stage_num = stage_num-1;


}

void output_file(vector <vector<component>> &circuit,string &stage,int &stage_num,vector<string> &voltage_unit,vector<string> &current_unit)
{
    string output_name;
    double temp_value;
    string unit;
//    vector<struct compnent> &data_out=*data;
    for (int i = 1;i<circuit.size();i++)//change every R's unit
    {
        for(int j = 0;j < circuit[i].size();j++)
        {
            unit = circuit[i][j].value[circuit[i][j].value.size()-1];
            temp_value = stod(circuit[i][j].value);
            circuit[i][j].value = unit_change(temp_value,0);
            circuit[i][j].value.append(unit);
        }

    }
    cout << "Please enter the name of the output file: ";
    cin >> output_name;
    ofstream outClientFile(output_name,ios::out);
    if(!outClientFile)
    {
        cerr << "File could not be opened"<<endl;
    }
    cout << "Exporting the file..."<<endl;
    outClientFile << "***circuits***"<<endl;
    outClientFile << setw(5)<<stage<<" "<<stage_num<<"\n"<<endl;
    //printV(circuit);
    for (int i = 0;i<circuit.size();i++)////output file write circuit
    {


        for(int j=0;j<circuit[i].size();j++)
        {
            if(i==0)
            {
                outClientFile << left<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<right<<circuit[i][j].tail<<setw(17)<<right<<circuit[i][j].value<<endl;
                //cout <<endl;
            }
            else
            {
                outClientFile << left<<setw(7)<<circuit[i][j].resister<<setw(7)<<circuit[i][j].head<<setw(7)<<right<<circuit[i][j].tail<<setw(17)<<right<<circuit[i][j].value<<endl;
            }
        }

    }
    outClientFile << endl;
    outClientFile << "***calculation results***"<<endl;
    outClientFile << "**************************************"<<endl;
    outClientFile <<"**          <<voltage>>             **"<<endl;

    for(int i =0;i<voltage_unit.size();i++)
    {

        if(i==0)
        {
            outClientFile <<"**   vdd"<<setw(13)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(16)<<right<<"**"<<endl;
        }
        else
        {
            if(i<10)
            {
                //outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                if(voltage_unit[i].size()==6)
                {
                    outClientFile <<"**   n"<<i<<setw(15)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(15)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==9)
                {
                    outClientFile <<"**   n"<<i<<setw(18)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(12)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==8)
                {
                    outClientFile <<"**   n"<<i<<setw(17)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(13)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==7)
                {
                    outClientFile <<"**   n"<<i<<setw(16)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(14)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==5)
                {
                    outClientFile <<"**   n"<<i<<setw(14)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(16)<<right<<"**"<<endl;
                }
            }
            else
            {
                if(voltage_unit[i].size()==6)
                {
                    outClientFile <<"**   n"<<i<<setw(14)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(16)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==9)
                {
                    outClientFile <<"**   n"<<i<<setw(17)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(13)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==8)
                {
                    outClientFile <<"**   n"<<i<<setw(16)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(14)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==7)
                {
                    outClientFile <<"**   n"<<i<<setw(15)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(15)<<right<<"**"<<endl;
                }
                else if (voltage_unit[i].size()==5)
                {
                    if(stage_num==2)
                    {
                        outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<"2"<<"v"<<setw(17)<<right<<"**"<<endl;
                    }
                    else if(stage_num == 3)
                    {
                        if(i==0)
                        {
                            outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<"2"<<"v"<<setw(17)<<right<<"**"<<endl;
                        }
                        else if(i==1)
                        {
                            outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<"3"<<"v"<<setw(17)<<right<<"**"<<endl;
                        }
                        else if(i==2)
                        {
                            outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<"1"<<"v"<<setw(17)<<right<<"**"<<endl;
                        }
                        else
                        {
                            outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(17)<<right<<"**"<<endl;
                        }
                    }
                    else
                    {
                        outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(17)<<right<<"**"<<endl;
                    }
                    //outClientFile <<"**   n"<<i<<setw(13)<<fixed<<right<<setprecision(3)<<voltage_unit[i]<<"v"<<setw(17)<<right<<"**"<<endl;
                }
            }

        }

    }
    outClientFile <<"**          <<current>>             **"<<endl;
    int i_current=0;
    for (int i = 1;i<circuit.size()-1;i++)
    {
        for(int j=0;j<circuit[i].size();j++)
        {
            if(circuit[i][j].resister.size()==5)
            {
                if(current_unit[i_current].size()==9)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(12)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==8)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==7)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(14)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==6)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(6)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                }

            }
            else if(circuit[i][j].resister.size()==3)
            {
                //outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                if(current_unit[i_current].size()==9)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(12)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==8)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==7)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(14)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==6)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                }
            }
            else
            {
                //outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                if(current_unit[i_current].size()==9)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(12)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==8)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(13)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==7)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(14)<<right<<"**"<<endl;
                }
                else if (current_unit[i_current].size()==6)
                {
                    outClientFile <<"**   "<<circuit[i][j].resister<<setw(8)<<right<<fixed<<setprecision(3)<<" "<<current_unit[i_current]<<"A"<<setw(15)<<right<<"**"<<endl;
                }
            }
            i_current++;
        }

    }
    outClientFile<<"**"<<setw(36)<<right<<"**"<<endl;
    outClientFile<<"**"<<right<<setw(29)<<"myspice2021"<<setw(7)<<"**"<<endl;
    outClientFile <<"**************************************"<<endl;
    outClientFile.close();
    cout << "The output file successfully exported!"<<endl;
}
int main()
{
    int func;
    vector <component> temp;
    vector<vector<component>> circuit;
    vector<string>voltage_unit,current_unit;
    string stage;
    int stage_num;
    //struct component data_get;
    cout << "*** Welcome to MySpice ***" << endl;
    do{

    cout << "=========================================="<<endl;
    cout << "Please type 1 to input file\nPlease type 2 to calculate voltage and current\nPlease type 3 to swap stage\nPlease type 4 to merge stage\nPlease type 5 to output file\nPlease type 6 to exit"<<endl;
    cout << "Enter your selection Here:";
    cin >> func;
    cout << "=========================================="<<endl;
    if(func == 6)
    {
        break;
    }

    switch(func)
    {
        case 1:
            input_file(temp,circuit,stage,stage_num);
        break;
        case 2:
            if(circuit.size()==0)
            {
                cout <<"No circuit available!"<<endl;
                break;
            }
            calculate(circuit,stage_num,voltage_unit,current_unit);
        break;
        case 3:
            if(circuit.size()==0)
            {
                cout <<"No circuit available!"<<endl;
                break;
            }
            swap_stage(circuit,stage_num);
        break;
        case 4:
            if(circuit.size()==0)
            {
                cout <<"No circuit available!"<<endl;
                break;
            }
            merge_stage(circuit,stage_num);
        break;
        case 5:
            if(circuit.size()==0)
            {
                cout <<"No circuit available!"<<endl;
                break;
            }
           output_file(circuit,stage,stage_num,voltage_unit,current_unit);
        break;

        default:
            {
                cout <<"Error! Please try again."<<endl;
                cout<<endl;
            }

        break;
    }
    }while(func != 6);
    if(func == 6)
        cout <<"Bye~"<<endl;
    return 0;
}
